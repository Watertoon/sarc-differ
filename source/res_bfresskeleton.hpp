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

    enum BfresBoneBillboardMode {
        BfresBoneBillboardMode_None,
        BfresBoneBillboardMode_Child,
        BfresBoneBillboardMode_WorldViewVector,
        BfresBoneBillboardMode_WorldViewPoint,
        BfresBoneBillboardMode_ScreenViewVector,
        BfresBoneBillboardMode_ScreenViewPoint,
        BfresBoneBillboardMode_YAxisViewVector,
        BfresBoneBillboardMode_YAxisViewPoint,
    };
    enum BfresBoneTransformMode : u8 {
        BfresBoneTransformMode_None                    = (1 << 0),
        BfresBoneTransformMode_SegmentScaleCompensate  = (1 << 1),
        BfresBoneTransformMode_ScaleUniform            = (1 << 2),
        BfresBoneTransformMode_ScaleVolumeOne          = (1 << 3),
        BfresBoneTransformMode_NoRotation              = (1 << 4),
        BfresBoneTransformMode_NoTranslation           = (1 << 5),
    };
    enum BfresBoneHierarchyTransformMode {
        BfresBoneHierarchyTransformMode_None                    = (1 << 0),
        BfresBoneHierarchyTransformMode_ScaleUniform            = (1 << 1),
        BfresBoneHierarchyTransformMode_ScaleVolumeOne          = (1 << 2),
        BfresBoneHierarchyTransformMode_NoRotation              = (1 << 3),
        BfresBoneHierarchyTransformMode_NoTranslation           = (1 << 4),
    };

    struct ResBfresBone {
        const char                *bone_name;
        ResGfxUserData            *user_data_array;
        ResNintendoWareDictionary *user_data_dictionary;
        void                      *reserve0;
        u16                        bone_index;
        u16                        bone_parent_index;
        u16                        smooth_bone_index;
        u16                        rigid_bone_index;
        u16                        billboard_index;
        u16                        user_data_count;
        u32                        reserve1                 : 12;
        u32                        is_bone_visible          : 1;
        u32                        reserve2                 : 3;
        u32                        billboard_mode           : 3;
        u32                        reserve3                 : 4;
        u32                        transform_mode           : 4;
        u32                        hierarchy_transform_mode : 5;
        util::math::Vector3f       translate;
        util::math::Vector4f       rotate;
        util::math::Vector3f       scale;
    };
    static_assert(sizeof(ResBfresBone) == 0x58);

    enum BfresSkeletonMirrorMode {
        BfresSkeletonMirrorMode_X,
        BfresSkeletonMirrorMode_XY,
        BfresSkeletonMirrorMode_XZ,
    };
    enum BfresSkeletonScaleMode {
        BfresSkeletonScaleMode_None,
        BfresSkeletonScaleMode_Standard,
        BfresSkeletonScaleMode_Maya,
        BfresSkeletonScaleMode_SoftImage,
    };
    enum BfresSkeletonRotationMode {
        BfresSkeletonRotationMode_Quarternion,
        BfresSkeletonRotationMode_EulerXYZ,
    };

    struct ResBfresSkeleton {
        u32                        magic;
        u32                        has_user_pointer : 2;
        u32                        reserve0         : 4;
        u32                        mirror_mode      : 2;
        u32                        scale_mode       : 2;
        u32                        reserve1         : 2;
        u32                        rotation_mode    : 2;
        ResNintendoWareDictionary *bone_dictionary;
        ResBfresBone              *bone_array;
        u16                       *bone_index_table;
        util::math::Matrix34f     *inverse_transformation_matrix_array;
        void                      *user_pointer;
        u16                       *mirrored_bone_index_table;
        u16                        bone_count;
        u16                        smooth_bone_count;
        u16                        rigid_bone_count;
        u16                        reserve2;
        
        static constexpr u32 Magic = util::TCharCode32("FSKL");
    };
    static_assert(sizeof(ResBfresSkeleton) == 0x40);
}
