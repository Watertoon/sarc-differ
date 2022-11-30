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

    struct ResBwav {
        u32 magic;
        u16 endianess;
        u16 version;
        u32 samples_crc32;
        u16 is_prefetch;
        u16 channel_count;

        bool IsValid() const {
            if (magic == Magic && endianess == ByteOrder_Little && version == TargetVersion) { return true; }
            return false;
        }

        static constexpr u32 Magic              = util::TCharCode32("BWAV");
        static constexpr u32 TargetVersion      = 1;
        static constexpr u32 SampleArrayAlignment = 0x40;
    };
    static_assert(sizeof(ResBwav) == 0x10);

    enum BwavChannelPan {
        BwavChannelPanLeft   = 0,
        BwavChannelPanRight  = 1,
        BwavChannelPanMiddle = 2,
    };

    struct ResBwavChannelInfo {
        u16 reserve0;
        u16 channel_pan;
        u32 sample_rate;
        u32 non_prefetch_sample_count;
        u32 sample_count;
        u16 dsp_adpcm_coefficients[16];
        u32 non_prefetch_samples_offset;
        u32 samples_offset;
        u32 is_looping;
        u32 loop_end;
        u32 loop_start;
        u16 reserve1;
        u16 reserve2;
        u16 reserve3;
        u16 reserve4;

        static constexpr u32 InvalidLoopOffset = 0xFFFF'FFFF;
    };
    static_assert(sizeof(ResBwavChannelInfo) == 0x4C);
}
