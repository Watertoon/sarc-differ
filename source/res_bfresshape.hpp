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

    struct ResBfresBufferStride {
        u32 stride;
        u32 divisor;
        u32 reserve0;
        u32 reserve1;
    };
    static_assert(sizeof(ResBfresBufferStride) == 0x10);

    struct ResBfresVertexAttribute {
        const char *attribute_name;
        u32         attribute_format;
        u16         buffer_offset;
        u8          buffer_index;
        u8          is_dynamic_vbo;
    };
    static_assert(sizeof(ResBfresVertexAttribute) == 0x10);

    struct ResBfresVertex {
        u32                         magic;
        u32                         reserve0;
        ResBfresVertexAttribute    *vertex_attribute_array;
        ResNintendoWareDictionary  *vertex_attribute_dictionary;
        void                       *memory_pool;
        void                       *buffer_array;
        void                      **buffer_ptr_array;
        ResGfxBufferInfo           *buffer_info_array;
        ResBfresBufferStride       *buffer_stride_info_array;
        void                       *user_pointer;
        u32                         base_memory_offset;
        u8                          vertex_attribute_count;
        u8                          vertex_buffer_count;
        u16                         section_index;
        u32                         vertex_count;
        u16                         reserve1;
        u16                         vertex_buffer_alignment;

        static constexpr u32 Magic = util::TCharCode32("FVTX");
    };
    static_assert(sizeof(ResBfresVertex) == 0x58);

    struct ResBfresSubMeshRange {
        u32 base_index;
        u32 index_count;
    };
    static_assert(sizeof(ResBfresSubMeshRange) == 0x8);

    struct ResBfresMesh {
        ResBfresSubMeshRange *sub_mesh_range;
        void                 *memory_pool;
        void                 *buffer_array;
        ResGfxBufferInfo     *buffer_info_array;
        u32                   index_buffer_memory_offset;
        u32                   primitive_topology;
        u32                   index_format;
        u32                   indice_count;
        u32                   base_buffer_index;
        u16                   sub_mesh_count;
        u16                   reserve;
    };
    static_assert(sizeof(ResBfresMesh) == 0x38);

    struct ResBfresKeyShape {
        u8 key_shape_attribute_location_array[18];
        u8 reserve[2];
    };
    static_assert(sizeof(ResBfresKeyShape) == 0x14);

    struct ResBfresBounding {
        util::math::Vector3f center_position;
        util::math::Vector3f extents;
    };
    static_assert(sizeof(ResBfresBounding) == 0x18);

    struct ResBfresShape {
        u32                        magic;
        u32                        reserve0               : 1;
        u32                        is_bounding_consistent : 1;
        u32                        has_vertex_buffer      : 1;
        u32                        reserve1               : 29;
        const char                *shape_name;
        ResBfresVertex            *shape_vertex;
        ResBfresMesh              *mesh_array;
        u16                       *skin_bone_indice_array;
        ResBfresKeyShape          *key_shape_array;
        ResNintendoWareDictionary *key_shape_dictionary;
        ResBfresBounding          *bounding_box_array;
        float                     *bounding_sphere_radius_array;
        void                      *user_pointer;
        u16                        section_index;
        u16                        material_index;
        u16                        bone_index;
        u16                        vertex_index;
        u16                        skin_bone_indice_count;
        u8                         max_bone_influences_per_vertex;
        u8                         mesh_count;
        u8                         key_shape_count;
        u8                         target_attribute_count;
        u16                        reserve2;

        static constexpr u32 Magic = util::TCharCode32("FSHP");
    };
    static_assert(sizeof(ResBfresShape) == 0x60);
}
