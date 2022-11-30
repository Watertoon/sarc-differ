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

    struct ResBfresRenderInfo {
        const char *render_info_name;
        u8          data_type;
        u8          reserve0;
        u16         reserve1;
        u32         reserve2;
    };
    static_assert(sizeof(ResBfresRenderInfo) == 0x10);

    enum class BfresShaderParamDataType : u8 {
        Bool                   = 0x0,
        Vec2Bool               = 0x1,
        Vec3Bool               = 0x2,
        Vec4Bool               = 0x3,
        S32                    = 0x4,
        Vec2S32                = 0x5,
        Vec3S32                = 0x6,
        Vec4S32                = 0x7,
        U32                    = 0x8,
        Vec2U32                = 0x9,
        Vec3U32                = 0xa,
        Vec4U32                = 0xb,
        Float                  = 0xc,
        Vec2Float              = 0xd,
        Vec3Float              = 0xe,
        Vec4Float              = 0xf,
        Matrix22f              = 0x11,
        Matrix23f              = 0x12,
        Matrix24f              = 0x13,
        Matrix32f              = 0x15,
        Matrix33f              = 0x16,
        Matrix34f              = 0x17,
        Matrix42f              = 0x19,
        Matrix43f              = 0x1a,
        Matrix44f              = 0x1b,
        Srt2D                  = 0x1c,
        Srt3D                  = 0x1d,
        SrtTexture             = 0x1e,
        SrtTextureWithUserData = 0x1f,
    };

    struct ResBfresShaderParam;

    using ConvertTextureSrtFunction = u32 (*)(void*, const void*, ResBfresShaderParam*, void*);

    struct ResBfresShaderParam {
        ConvertTextureSrtFunction  *convert_texture_srt_callback;
        const char                 *shader_param_name;
        u16                         shader_param_offset;
        BfresShaderParamDataType    data_type;
        u8                          reserve0;
        u32                         reserve1;
    };
    static_assert(sizeof(ResBfresShaderParam) == 0x18);

    struct ResBfresShaderReflection {
        const char                *shader_archive_name;
        const char                *shader_name;
        ResBfresRenderInfo        *render_info_array;
        ResNintendoWareDictionary *render_info_dictionary;
        ResBfresShaderParam       *shader_param_array;
        ResNintendoWareDictionary *shader_param_dictionary;
        ResNintendoWareDictionary *vertex_attribute_dictionary;
        ResNintendoWareDictionary *sampler_dictionary;
        ResNintendoWareDictionary *static_shader_option_dictionary;
        u16                        render_info_count;
        u16                        shader_param_count;
        u16                        shader_param_data_size;
        u16                        reserve0;
        u64                        reserve1;
    };
    static_assert(sizeof(ResBfresShaderReflection) == 0x58);

    struct ResBfresMaterialShaderData {
        ResBfresShaderReflection  *shader_reflection;
        const char               **vertex_attribute_name_array;
        u8                        *vertex_attribute_index_array;
        const char               **sampler_name_array;
        u8                        *sampler_index_array;
        u32                       *static_shader_option_int_value_array;
        const char               **static_shader_option_string_value_array;
        u16                       *static_option_indice_array;
        u32                        reserve0;
        u8                         vertex_attribute_count;
        u8                         sampler_count;
        u16                        int_static_option_count;
        u16                        total_static_option_count;
        u16                        reserve1;
        u32                        reserve2;
    };
    static_assert(sizeof(ResBfresMaterialShaderData) == 0x50);

    struct ResBfresMaterial {
        u32                          magic;
        u32                          is_material_visible;
        const char                  *material_name;
        ResBfresMaterialShaderData  *material_shader_data;
        void                       **user_texture_view_array;
        const char                 **texture_name_array;
        void                        *sampler_array;
        ResGfxSamplerInfo           *sampler_info_array;
        ResNintendoWareDictionary   *sampler_dictionary;
        void                        *render_info_value_array;
        u16                         *render_info_value_count_array;
        u16                         *render_info_value_offset_array;
        void                        *shader_param_value_array;
        s32                         *shader_param_ubo_offset_array;
        void                        *reserve0;
        ResGfxUserData              *user_data_array;
        ResNintendoWareDictionary   *user_data_dictionary;
        u32                         *shader_param_convert_flags_array;
        void                        *user_pointer;
        u64                         *sampler_descriptor_slot_array;
        u64                         *texture_descriptor_slot_array;
        u16                          section_index;
        u8                           sampler_count;
        u8                           texture_count;
        u16                          reserve1;
        u16                          user_data_count;
        u16                          unknown_size;
        u16                          user_shading_model_option_ubo_size;
        u32                          reserve2;

        static constexpr u32 Magic = util::TCharCode32("FMAT");
    };
    static_assert(sizeof(ResBfresMaterial) == 0xb0);
}
