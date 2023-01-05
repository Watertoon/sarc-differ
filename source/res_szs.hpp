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
    
    constexpr ALWAYS_INLINE u32 SwapEndian32(u32 value) {
        const u32 swap = (value & 0xff00'ff00) >> 8 | (value & 0x00ff'00ff) << 8;
        return (swap >> 0x10) | (swap << 0x10);
    }
}

namespace dd::res {

	struct ResSzs {
		u32 magic;
		u32 decompressed_size;
		u32 reserve0;
		u32 reserve1;
		u8  data_array[];

		static constexpr u32 Magic = util::TCharCode32("Yaz0");
	};
	static_assert(sizeof(ResSzs) == 0x10);

	void DecodeSZS(u8 *output, ResSzs *szs) {

		u32 size_iter = util::SwapEndian32(szs->decompressed_size);
		u8 *data_iter = szs->data_array;
		u32 h_i = 0;
        u32 head = 0;
		do {
			for(;;) {
				/* Check header for compressed block */
				h_i = h_i >> 1;
				if (h_i == 0) {
					head = *data_iter;
					++data_iter;
					h_i = 0x80;
				}
				if ((head & h_i) == 0) { break; }

				/* Copy uncompressed bytes */
				--size_iter;
				*output = *data_iter;
				++output;
                ++data_iter;
				if (size_iter == 0) {
					return;
				}
			}

			/* Handle compressed data */
			u32 src_offset = (*data_iter << 0x8) + *(data_iter + 1);
			data_iter = data_iter + 2;

			const u32 length = src_offset >> 0xc;
			u32 sub_i = length + 2;
			if (length == 0) {
				sub_i = *data_iter + 0x12;
				++data_iter;
			}

            size_iter = size_iter - sub_i;
			do {
                --sub_i;
				*output = *(output - ((src_offset & 0xfff) + 1));
				++output;
			} while (sub_i != 0);
		} while (size_iter != 0);
	}
}
