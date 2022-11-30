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

    enum BfresBoneAnimCurveType : u16 {
        BfresBoneAnimCurveType_ScaleX      = (1 << 0),
        BfresBoneAnimCurveType_ScaleY      = (1 << 1),
        BfresBoneAnimCurveType_ScaleZ      = (1 << 2),
        BfresBoneAnimCurveType_RotateX     = (1 << 3),
        BfresBoneAnimCurveType_RotateY     = (1 << 4),
        BfresBoneAnimCurveType_RotateZ     = (1 << 5),
        BfresBoneAnimCurveType_RotateW     = (1 << 6),
        BfresBoneAnimCurveType_TranslateX  = (1 << 7),
        BfresBoneAnimCurveType_TranslateY  = (1 << 8),
        BfresBoneAnimCurveType_TranslateZ  = (1 << 9),
    };

    struct ResBfresBoneAnimResultDefault {
        util::math::Vector3f translate;
        util::math::Vector4f rotate;
        util::math::Vector3f scale;
    };
    static_assert(sizeof(ResBfresBoneAnimResultDefault) == sizeof(float) * 10);

    struct ResBfresBoneAnim {
        const char                    *bone_name;
        ResBfresAnimCurve             *anim_curve_array;
        ResBfresBoneAnimResultDefault *default_result;
        ResGfxUserData                *user_data_array;
        ResNintendoWareDictionary     *user_data_dictionary;
        u32                            reserve0            : 3;
        u32                            is_use_scale        : 1;
        u32                            is_use_rotation     : 1;
        u32                            is_use_translation  : 1;
        u32                            curve_type          : 10;
        u32                            reserve1            : 5;
        u32                            transform_mode      : 5;
        u32                            reserve2            : 6;
        u8                             reserve3;
        u8                             reserve4;
        u8                             anim_curve_count;
        u8                             reserve5;
        u32                            base_curve_index;
        u16                            user_data_count;
        u16                            reserve6;
    };
    static_assert(sizeof(ResBfresBoneAnim) == 0x38);

    struct ResBfresSkeletalAnim {
        u32                        magic;
        u32                        is_baked       : 1;
        u32                        reserve0       : 1;
        u32                        is_looping     : 1;
        u32                        reserve1       : 5;
        u32                        scale_mode     : 2;
        u32                        reserve2       : 2;
        u32                        rotation_mode  : 1;
        u32                        reserve3       : 19;
        const char                *animation_name;
        const char                *reserve4;
        ResBfresSkeleton          *user_bound_skeleton;
        u16                       *bind_table;
        ResBfresBoneAnim          *bone_anim_array;
        ResGfxUserData            *user_data_array;
        ResNintendoWareDictionary *user_data_dictionary;
        u32                        frame_count;
        u32                        total_anim_curves;
        u32                        baked_size;
        u16                        bone_anim_count;
        u16                        user_data_count;

        static constexpr u32 Magic = util::TCharCode32("FSKA");
    };
    static_assert(sizeof(ResBfresSkeletalAnim) == 0x50);
}
