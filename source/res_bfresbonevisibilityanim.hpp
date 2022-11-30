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

    struct ResBfresBoneVisibilityAnim {
        u32                         magic;
        u32                         is_baked        : 1;
        u32                         reserve0        : 1;
        u32                         is_looping      : 1;
        u32                         reserve1        : 29;
        const char                 *animation_name;
        const char                 *reserve2;
        ResBfresModel              *user_bound_model;
        u16                        *bind_table;
        ResBfresAnimCurve          *anim_curve_array;
        bool                       *default_result_array;
        const char                **bound_bone_names;
        ResGfxUserData             *user_data_array;
        ResNintendoWareDictionary  *user_data_dictionary;
        u32                         frame_count;
        u32                         baked_size;
        u16                         bone_count;
        u16                         anim_curve_count;
        u16                         user_data_count;
        u16                         reserve3;

        static constexpr u32 Magic = util::TCharCode32("FBVS");
    };
    static_assert(sizeof(ResBfresBoneVisibilityAnim) == 0x60);
}
