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

namespace dd::util {

	//constexpr ALWAYS_INLINE u32 Crc32b_32(u32 value, u32 accumulate) {
    //
    //    const v2ull constant = {0xB4E5B025F7011641, 0x00000001DB710641};
    //    v2ull state = {static_cast<size_t>(value ^ accumulate) << 32, 0ull};
    //
    //    state = pclmulqdq(state, constant, PclmulSelectionMask(false, false));
    //    state = pclmulqdq(state, constant, PclmulSelectionMask(false, true));
    //
    //    return static_cast<u32>(state[1]);
    //}
    //
    //constexpr ALWAYS_INLINE u32 Crc32b_64(u64 value, u32 accumulate) {
    //
    //    const v2ull constant = {0xB4E5B025F7011641, 0x00000001DB710641};
    //    v2ull state = {value ^ accumulate, 0ull};
    //
    //    state = pclmulqdq(state, constant, PclmulSelectionMask(false, false));
    //    state = pclmulqdq(state, constant, PclmulSelectionMask(false, true));
    //
    //    return static_cast<u32>(state[1]);
    //}

    constexpr inline u32 Crc32bSeed = 0xedb88320;

    consteval ALWAYS_INLINE std::array<u32, 0x100> GenerateCrc32bTable() {
        std::array<u32, 0x100> context = {};
        
        for (u32 i = 0; i < 0x100; ++i) {
            u32 seed = (i >> 1) & 0x7fff'ffff;
            if ((i & 1) != 0) {
                seed = seed ^ Crc32bSeed;
            }
            for (u32 u = 0; u < 7; ++u) {
                if ((seed & 1) != 0) {
                    seed = (seed >> 1) ^ Crc32bSeed;
                } else {
                    seed = seed >> 1;
                }
            }
            context[i] = seed;
        }
        return context;
    }

    constexpr inline std::array<u32, 0x100> Crc32bTable = GenerateCrc32bTable();

    constexpr ALWAYS_INLINE u32 Crc32bHash(const char *string) {
        //if (std::is_constant_evaluated() == true) {

            u32 seed = 0xffff'ffff;
            while (*string != '\0') {
                seed = Crc32bTable[(*string ^ seed) & 0xff] ^ seed >> 8;
                string = string + 1;
            }
            
            return ~seed;
        //} else {
        //
        //    u32 seed = 0xffff'ffff;
        //    
        //    /* Perform 8 byte aligned version of crc32 */
        //    const size_t size = ::strlen(string);
        //    const size_t aligned_size = (size & ~7);
        //    const char *string_leftover = string + aligned_size;
        //    
        //    if (aligned_size != 0) {
        //        while (string != string_leftover) {
        //            const size_t val = *reinterpret_cast<const size_t*>(string);
        //            seed = Crc32b_64(val, seed);
        //            string = string + 8;
        //        }
        //    }
        //    
        //    /* 4 byte fallback */
        //    if ((size & 4) != 0) {
        //        const u32 val = *reinterpret_cast<const u32*>(string);
        //        seed = Crc32b_32(val, seed);
        //        string_leftover = string_leftover + 4;
        //    }
        //    
        //    /* Fallback for rest of string */
        //    while (*string_leftover != '\0') {
        //        seed = Crc32bTable[(*string_leftover ^ seed) & 0xff] ^ seed >> 8;
        //        
        //        string_leftover = string_leftover + 1;
        //    }
        //    
        //    return ~seed;
        //}
    }
}