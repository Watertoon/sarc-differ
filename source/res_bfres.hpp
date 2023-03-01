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

    struct ResBfres : public ResNintendoWareFileHeader {
        const char                 *fres_name;
        ResBfresModel              *model_array;
        ResNintendoWareDictionary  *model_dictionary;
        u64                         reserve0;
        u64                         reserve1;
        u64                         reserve2;
        u64                         reserve3;
        ResBfresSkeletalAnim       *skeletal_anim_array;
        ResNintendoWareDictionary  *skeletal_anim_dictionary;
        ResBfresMaterialAnim       *material_anim_array;
        ResNintendoWareDictionary  *material_anim_dictionary;
        ResBfresBoneVisibilityAnim *bone_visibility_anim_array;
        ResNintendoWareDictionary  *bone_visibility_anim_dictionary;
        ResBfresShapeAnim          *shape_anim_array;
        ResNintendoWareDictionary  *shape_anim_dictionary;
        ResBfresSceneAnim          *scene_anim_array;
        ResNintendoWareDictionary  *scene_anim_dictionary;
        void                       *user_memory_pool;
        ResGfxMemoryPoolInfo       *memory_pool_info;
        ResGfxEmbedFile            *embed_file_array;
        ResNintendoWareDictionary  *embed_file_dictionary;
        void                       *user_pointer;
        const char                 *reserve4;
        u32                         reserve5;
        u16                         model_count;
        u16                         reserve6;
        u16                         reserve7;
        u16                         skeletal_anim_count;
        u16                         material_anim_count;
        u16                         bone_visibility_anim_count;
        u16                         shape_anim_count;
        u16                         scene_anim_count;
        u16                         embed_file_count;
        u16                         reserve8;

        static constexpr u64 Magic = util::TCharCode64("FRES    ");

        static ResBfres *ResCast(void *file) {
            ResBfres *fres = reinterpret_cast<ResBfres*>(file);
            if (fres == nullptr || fres->ResNintendoWareFileHeader::IsValid(Magic, 10, 0) == false) { return nullptr; }
            fres->Relocate();
            return fres;
        }

        static bool IsValid(void *file) {
            ResBfres *fres = reinterpret_cast<ResBfres*>(file);
            return fres->ResNintendoWareFileHeader::IsValid(Magic, 10, 0, 0);
        }
        
        //void BindTexture(BindTextureCallback bind_callback, ResBntx *res_bntx) {
        //    for (u32 i = 0; i < model_count; ++i) {
        //        model_array[i].BindTexture(bind_callback, res_bntx);
        //    }
        //    for (u32 i = 0; i < material_anim_count; ++i) {
        //        material_anim_array[i].BindTexture(bind_callback, res_bntx);
        //    }
        //}

        constexpr ALWAYS_INLINE u64 GetGpuMemorySize() {
            return (memory_pool_info == nullptr) ? 0xffff'ffff'ffff'ffff : memory_pool_info->size;
        }

        constexpr ALWAYS_INLINE void *GetGpuMemoryRegion() {
            return (memory_pool_info == nullptr) ? nullptr : memory_pool_info->storage;
        }
    };
    static_assert(sizeof(ResBfres) == 0xf0);

    enum BfresAnimCurveFrameDataType {
        BfresAnimCurveFrameDataType_Float,
        BfresAnimCurveFrameDataType_F16,
        BfresAnimCurveFrameDataType_U8,
    };
    enum BfresAnimCurveValueDataType {
        BfresAnimCurveValueDataType_Float = 0,
        BfresAnimCurveValueDataType_S32   = 0,
        BfresAnimCurveValueDataType_S16   = 1,
        BfresAnimCurveValueDataType_S8    = 2,
    };
    enum BfresAnimCurveCurveType {
        BfresAnimCurveCurveType_CubicFloat,
        BfresAnimCurveCurveType_LinearFloat,
        BfresAnimCurveCurveType_BakedFloat,
        BfresAnimCurveCurveType_StepInteger,
        BfresAnimCurveCurveType_BakedInteger,
        BfresAnimCurveCurveType_StepBoolean,
        BfresAnimCurveCurveType_BakedBoolean,
    };
    enum BfresAnimCurveWrapMode {
        BfresAnimCurveWrapMode_Clamp,
        BfresAnimCurveWrapMode_Repeat,
        BfresAnimCurveWrapMode_Mirror,
        BfresAnimCurveWrapMode_RelativeRepeat,
    };

    struct ResBfresAnimCurve {
        union {
            float *frame_array_f32;
            s16   *frame_array_f16;
            u8    *frame_array_u8;
        };
        union {
            float *value_array_f32;
            s32   *value_array_s32;
            s16   *value_array_s16;
            s8    *value_array_s8;
        };
        u16   frame_data_type : 2;
        u16   value_data_type : 2;
        u16   curve_type      : 3;
        u16   reserve0        : 1;
        u16   pre_wrap_mode   : 2;
        u16   reserve1        : 2;
        u16   post_wrap_mode  : 2;
        u16   reserve2        : 2;
        u16   frame_count;
        u32   base_value;
        float start_key;
        float end_key;
        float frame_data_scale;
        float frame_data_add;
        float frame_delta;
        u32   reserve3;
    };
    static_assert(sizeof(ResBfresAnimCurve) == 0x30);
}
