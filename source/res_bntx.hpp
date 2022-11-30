 /*
 *  Copyright (C) W. Michael Knudson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as 
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along with this program; 
 *  if not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

namespace dd::res {

    struct ResBntxTextureData : public ResNintendoWareSubHeader {
        static constexpr u32 Magic = util::TCharCode32("BRTD");
    };
    static_assert(sizeof(ResBntxTextureData) == 0x10);

    struct ResBntxTextureInfo;

    struct ResBntxTextureContainer {
        u32                         magic;
        u32                         texture_count;
        ResBntxTextureInfo        **texture_info_array;
        ResBntxTextureData         *texture_data;
        ResNintendoWareDictionary  *texture_dictionary;
        void                       *texture_memory_pool; /* 0x100 storage */
        void                       *current_memory_pool;
        s32                         base_memory_pool_offset;
        u32                         reserve0;

        static constexpr u32 MagicNX   = util::TCharCode32("NX  ");
        static constexpr u32 MagicDDVK = util::TCharCode32("DDVK");
    };
    static_assert(sizeof(ResBntxTextureContainer) == 0x38);

    struct ResBntxTextureInfo : public ResNintendoWareSubHeader {
        ResGfxTextureInfo           texture_info;
        u8                          texture_layout[0x4];
        u8                          reserve1[0x14];
        u32                         mipmap_total_size;
        u32                         texture_data_alignment;
        GfxChannelSource            channel_sources[0x4];
        GfxImageDimension           image_dimension;
        u8                          reserve2[0x3];
        char                       *name;
        ResBntxTextureContainer    *res_container;
        void                      **mip_offset_table;
        ResGfxUserData             *user_data_array;
        void                       *texture;      /* 0x100 storage */
        void                       *texture_view; /* 0x100 storage */
        u64                        *descriptor_slot;
        ResNintendoWareDictionary  *user_data_dictionary;

        static constexpr u32 Magic = util::TCharCode32("BRTI");
    };
    static_assert(sizeof(ResBntxTextureInfo) == 0xA0);

    struct ResBntx : public ResNintendoWareFileHeader {
        ResBntxTextureContainer container;

        static constexpr u64 Magic = static_cast<u64>(util::TCharCode32("BNTX"));

        ResBntxTextureInfo *FindTexture(const char *name) {
            u32 entry_index = container.texture_dictionary->FindEntryIndex(name);
            if (entry_index == static_cast<u32>(-1)) { return nullptr; }
            return container.texture_info_array[entry_index];
        }

        static ResBntx *ResCast(void *file) {
            ResBntx *texture_file = reinterpret_cast<ResBntx*>(file);
            if (texture_file->IsRelocated() == false) {
                texture_file->GetRelocationTable()->Relocate(); 
            }
            return texture_file;
        }

        static bool IsValid(void *file) {
            return reinterpret_cast<ResBntx*>(file)->ResNintendoWareFileHeader::IsValid(Magic, 4, 1, 0);
        }

        static constexpr ALWAYS_INLINE u64 GetMaxFileAlignment() {
            return 0x20000;
        }
    };
    static_assert(sizeof(ResBntx) == 0x58);
}
