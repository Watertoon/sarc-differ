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

    struct ResAmta {
        u32 magic;
        u16 endianess;
        u8  minor_version;
        u8  major_version;
        u32 file_size;
        u32 reserve0;
        u32 first_offset;
        u32 reserve1;
        u32 reserve2;
        u32 reserve3;
        u32 reserve4;
        u32 path_offset;

        bool IsValid() const {
            if (magic != Magic || major_version != TargetMajorVersion || endianess != ByteOrder_Little) {
                return false;
            }
            return true;
        }

        static constexpr u32 Magic = util::TCharCode32("AMTA");
        static constexpr u32 TargetMajorVersion = 5;
    };
    //static_assert(sizeof(ResAmta) == 0x30);

    struct ResAmtaData {
        u32 name_offset;
        
    };
    //static_assert(sizeof(ResAmta) == 0x20);
}
