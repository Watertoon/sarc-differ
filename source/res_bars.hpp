 /*
 *  Copyright (C) W. Michael Knudson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along with this program; 
 *  if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#pragma once

namespace dd::res {

    struct ResBars {
        u32 magic;
        u32 file_size;
        u16 endianess;
        u8  minor_version;
        u8  major_version;
        u32 file_count;

        constexpr ALWAYS_INLINE bool IsValid() const {
            if (magic != Magic || major_version != TargetMajorVersion || minor_version != TargetMinorVersion || endianess != ByteOrder_Little) {
                return false;
            }
            return true;
        }

        static constexpr u32 Magic = util::TCharCode32("BARS");
        static constexpr u32 TargetMajorVersion = 1;
        static constexpr u32 TargetMinorVersion = 2;
    };
    static_assert(sizeof(ResBars) == 0x10);

    struct ResBarsEntry {
        u32 amta_offset;
        u32 bwav_offset;
    };

    class BarsExtractor {
        public:
            static constexpr u32 InvalidEntryId = 0xFFFF'FFFF;
        private:
            ResBars      *m_res_bars;
            u32          *m_file_hash_array;
            ResBarsEntry *m_entry_array;
        public:
            constexpr ALWAYS_INLINE BarsExtractor() {/*...*/}

            bool Initialize(void *file) {

                /* Check for basic magic */
                if (file == nullptr || reinterpret_cast<ResBars*>(file)->IsValid() == false) { return false; }

                m_res_bars        = reinterpret_cast<ResBars*>(file);

                m_file_hash_array = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(file) + sizeof(ResBars));

                /* Check file array is sorted lesser to greater */
                u32 cmp = 0;
                for (u32 i = 0; i < m_res_bars->file_count; ++i) {
                    if (m_file_hash_array[i] < cmp) {
                        m_res_bars        = nullptr;
                        m_file_hash_array = nullptr;
                        return false;
                    }
                    cmp = m_file_hash_array[i];
                }

                m_entry_array     = reinterpret_cast<ResBarsEntry*>(reinterpret_cast<uintptr_t>(file) + m_res_bars->file_count * sizeof(u32) + sizeof(ResBars));

                return true;
            }

            u32 FindEntryIndexByName(const char *entry_name) {

                /* Calculate crc32 */
                const u32 hash = util::Crc32bHash(entry_name);

                /* Binary search for index */
                u32 size = m_res_bars->file_count;
                u32 i = 0;
                u32 index = 0;
                while (i < size) {
                    index = i + size;
                    index = index >> 1;

                    if (m_file_hash_array[index] == hash) {
                        return index;
                    }
                    if (m_file_hash_array[index] < hash) {
                        i = index + 1;
                        index = size;
                    }
                    size = index;
                }

                return InvalidEntryId;
            }

            ResAmta *GetAmtaByIndex(u32 index) {
                return reinterpret_cast<ResAmta*>(reinterpret_cast<uintptr_t>(m_res_bars) + m_entry_array[index].amta_offset);
            }

            ResBwav *GetBwavByIndex(u32 index) {
                return reinterpret_cast<ResBwav*>(reinterpret_cast<uintptr_t>(m_res_bars) + m_entry_array[index].bwav_offset);
            }

            const char *ConvertEntryIndexToPath(u32 entry_index) {
                ResAmta *amta = this->GetAmtaByIndex(entry_index);
                return reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(std::addressof(amta->path_offset)) + amta->path_offset);
            }

            constexpr u32 GetFileCount() const { return m_res_bars->file_count; }
    };
}
