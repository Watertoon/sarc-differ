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

    ALWAYS_INLINE ResNintendoWareFileHeader *ResNintendoWareRelocationTable::GetFileHeader() { 
        return reinterpret_cast<ResNintendoWareFileHeader*>(reinterpret_cast<uintptr_t>(this) - this_table_offset);
    }

    void ResNintendoWareRelocationTable::Relocate() {
        ResNintendoWareFileHeader *file_header = GetFileHeader();

        /* Check there are sections */
        if (section_count == 0) {
            file_header->SetRelocated(true);
            return;
        }

        const uintptr_t base_file = reinterpret_cast<uintptr_t>(file_header);

        const size_t entry_table_offset = this->GetEntryTableOffset();

        /* Iterate each section */
        for(u32 i = 0; i < section_count; ++i) {
            ResSection *section = this->GetSection(i);
            uintptr_t base_ptr = (section->section_ptr == nullptr) ? base_file : reinterpret_cast<uintptr_t>(section->section_ptr) - section->section_offset;

            /* Iterate each entry */
            u32 entry_count = section->entry_count;
            for(u32 entry_index = section->entry_index_offset; entry_index < entry_count + section->entry_index_offset; ++entry_index) {
                const size_t entry_offset = entry_table_offset + entry_index * sizeof(ResEntry);
                ResEntry *entry = reinterpret_cast<ResEntry*>(reinterpret_cast<uintptr_t>(this) + entry_offset);

                /* Relocate offsets specified from entries */
                uintptr_t relocation_offset = base_file + entry->first_relocation_offset;

                const u8 offset_count = entry->relocation_count;
                const u8 offset_mask = offset_count & 3;
                for(u32 array_index = 0; array_index < entry->array_count; ++array_index) {
                    size_t *current_relocation = reinterpret_cast<size_t*>(relocation_offset);

                    /* Quadruple method */
                    if (3 < offset_count) {
                        for(s32 offset_index = offset_mask - offset_count; offset_index < 0; offset_index = offset_index + 4) {
                            size_t relocation_pointer = (*current_relocation != 0) ? reinterpret_cast<size_t>(base_ptr + *current_relocation) : 0;
                            *current_relocation = relocation_pointer;
                            current_relocation = current_relocation + 1; /* Stride is sizeof(size_t) */
                            
                            relocation_pointer = (*current_relocation != 0) ? reinterpret_cast<size_t>(base_ptr + *current_relocation) : 0;
                            *current_relocation = relocation_pointer;
                            current_relocation = current_relocation + 1;
                            
                            relocation_pointer = (*current_relocation != 0) ? reinterpret_cast<size_t>(base_ptr + *current_relocation) : 0;
                            *current_relocation = relocation_pointer;
                            current_relocation = current_relocation + 1;
                            
                            relocation_pointer = (*current_relocation != 0) ? reinterpret_cast<size_t>(base_ptr + *current_relocation) : 0;
                            *current_relocation = relocation_pointer;
                            current_relocation = current_relocation + 1;
                        }
                    }
                    if (offset_mask != 0) {
                        for(s32 offset_index = -offset_mask; offset_index < 0; ++offset_index) {
                            size_t relocation_pointer = (*current_relocation != 0) ? reinterpret_cast<size_t>(base_ptr + *current_relocation) : 0;
                            *current_relocation = relocation_pointer;
                            current_relocation = current_relocation + 1;
                        }
                    }

                    relocation_offset = relocation_offset + (offset_count << 3) + (entry->array_stride << 3);
                }
            }
        }

        file_header->SetRelocated(true);
        return;
    }
}
