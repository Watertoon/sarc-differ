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

    struct ResBfresPerMaterialAnim {
        const char        *model_name;
        void              *shader_param_anim_array;
        void              *texture_pattern_anim_array;
        ResBfresAnimCurve *anim_curve_array;
        void              *constant_array;
        u16                base_shader_param_curve_index;
        u16                base_texture_pattern_curve_index;
        u16                base_visibility_curve_index;
        u16                visibility_curve_index;
        u16                visibility_constant_index;
        u16                shader_param_anim_count;
        u16                texture_pattern_anim_count;
        u16                constant_count;
        u16                anim_curve_count;
        u16                reserve0;
        u32                reserve1;
    };
    static_assert(sizeof(ResBfresPerMaterialAnim) == 0x40);

    struct ResBfresMaterialAnim  {
        u32                         magic;
        u32                         is_baked        : 1;
        u32                         reserve0        : 1;
        u32                         is_looping      : 1;
        u32                         reserve1        : 29;
        const char                 *animation_name;
        const char                 *reserve2;
        ResBfresModel              *user_bound_model;
        u16                        *bind_table;
        ResBfresPerMaterialAnim    *per_material_anim_array;
        void                      **user_texture_view_array;
        const char                **texture_name_array;
        ResGfxUserData             *user_data_array;
        ResNintendoWareDictionary  *user_data_dictionary;
        s64                        *user_texture_descriptor_slot_array;
        u32                         frame_count;
        u32                         bake_size;
        u16                         user_data_count;
        u16                         per_material_anim_count;
        u16                         total_anim_curves;
        u16                         shader_param_anim_count;
        u16                         texutre_pattern_anim_count;
        u16                         material_visibility_anim_count;
        u16                         texture_view_count;
        u16                         reserve3;
        
        static constexpr u32 Magic = util::TCharCode32("FMAA");
    };
    static_assert(sizeof(ResBfresMaterialAnim) == 0x70);
}
