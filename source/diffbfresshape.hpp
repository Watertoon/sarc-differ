#pragma once

bool DiffBfresVertex(dd::res::ResBfresVertex *l_vertex, dd::res::ResBfresVertex *r_vertex, void *l_gpu_memory, void *r_gpu_memory, u32 indent_level, bool is_print) {

    /* Compare vertex buffer count */
    if (l_vertex->vertex_buffer_count != r_vertex->vertex_buffer_count) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << std::dec << "Different(vertex buffer count): (left: " << static_cast<u32>(l_vertex->vertex_buffer_count) << " buffers)(right: " << static_cast<u32>(r_vertex->vertex_buffer_count) << " buffers)" << std::endl;
    }

    /* Compare vertex counts */
    if (l_vertex->vertex_count != r_vertex->vertex_count) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << std::dec << "Different(vertex count): (left: " << l_vertex->vertex_count << " vertices)(right: " << r_vertex->vertex_count << " vertices)" << std::endl;
    }

    /* Compare vertex buffer alignments */
    if (l_vertex->vertex_buffer_alignment != r_vertex->vertex_buffer_alignment) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << std::dec << "Different(vertex buffer alignment): (left: " << l_vertex->vertex_buffer_alignment << " bytes)(right: " << r_vertex->vertex_buffer_alignment << " bytes)" << std::endl;
    }

    /* Compare vertex attributes */
    RomfsDirectoryParser l_attribute_iterator = {};
    l_attribute_iterator.InitializeByResDic(l_vertex->vertex_attribute_dictionary);
    RomfsDirectoryParser r_attribute_iterator = {};
    r_attribute_iterator.InitializeByResDic(r_vertex->vertex_attribute_dictionary);

    /* Different attributes */
    char **l_attribute_paths = l_attribute_iterator.GetFilePathArray();
    char **r_attribute_paths = r_attribute_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_attribute_iterator.GetFileCount(); ++i) {
        const u32 r_path_index = r_attribute_iterator.FindPathIndex(l_attribute_paths[i]);
        if (r_path_index == RomfsDirectoryParser::InvalidIndex) { if (is_print == false) { return false; } continue; }

        const u32 l_index = l_vertex->vertex_attribute_dictionary->FindEntryIndex(l_attribute_paths[i]);
        const u32 r_index = r_vertex->vertex_attribute_dictionary->FindEntryIndex(r_attribute_paths[r_path_index]);

        if (l_vertex->vertex_attribute_array[l_index].attribute_format == r_vertex->vertex_attribute_array[r_index].attribute_format
         && l_vertex->vertex_attribute_array[l_index].buffer_offset == r_vertex->vertex_attribute_array[r_index].buffer_offset
         && l_vertex->vertex_attribute_array[l_index].buffer_mask_offset == r_vertex->vertex_attribute_array[r_index].buffer_mask_offset
         && l_vertex->vertex_attribute_array[l_index].buffer_flag_offset == r_vertex->vertex_attribute_array[r_index].buffer_flag_offset
         && l_vertex->vertex_attribute_array[l_index].is_dynamic_vbo == r_vertex->vertex_attribute_array[r_index].is_dynamic_vbo) {
            continue;
        }
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "Different(vertex attribute): " << (l_vertex->vertex_attribute_array[l_index].attribute_name + 2) << std::endl;

        if (l_vertex->vertex_attribute_array[l_index].attribute_format != r_vertex->vertex_attribute_array[r_index].attribute_format) {
            PrintIndent(indent_level + 1);
            std::cout << "(left AttributeFormat: " << dd::res::GfxAttributeFormatToString(static_cast<dd::res::GfxAttributeFormat>(l_vertex->vertex_attribute_array[l_index].attribute_format)) << ")(right AttributeFormat: " << dd::res::GfxAttributeFormatToString(static_cast<dd::res::GfxAttributeFormat>(r_vertex->vertex_attribute_array[r_index].attribute_format)) << ")" << std::endl;
        }
    }
    
    /* Right only attributes */
    for (u32 i = 0; i < r_attribute_iterator.GetFileCount(); ++i) {
        const u32 l_path_index = l_attribute_iterator.FindPathIndex(r_attribute_paths[i]);
        if (l_path_index != RomfsDirectoryParser::InvalidIndex) { continue; }
        if (is_print == false) { return false; }

        const u32 r_index = r_vertex->vertex_attribute_dictionary->FindEntryIndex(r_attribute_paths[i]);

        PrintIndent(indent_level);
        PrintOnlySide(PrintSide::Right);
        std::cout << "(vertex attribute): " << (r_vertex->vertex_attribute_array[r_index].attribute_name + 2) << std::endl;
        
        PrintIndent(indent_level + 1);
        std::cout << "(";
        PrintSideSpace(PrintSide::Right);
        std::cout << "AttributeFormat: " << dd::res::GfxAttributeFormatToString(static_cast<dd::res::GfxAttributeFormat>(r_vertex->vertex_attribute_array[r_index].attribute_format)) << ")" << std::endl;
    }
    
    /* Left only attributes */
    for (u32 i = 0; i < l_attribute_iterator.GetFileCount(); ++i) {
        const u32 r_path_index = r_attribute_iterator.FindPathIndex(l_attribute_paths[i]);
        if (r_path_index != RomfsDirectoryParser::InvalidIndex) { continue; }
        if (is_print == false) { return false; }

        const u32 l_index = l_vertex->vertex_attribute_dictionary->FindEntryIndex(l_attribute_paths[i]);

        PrintIndent(indent_level);
        PrintOnlySide(PrintSide::Left);
        std::cout << "(vertex attribute): " << (l_vertex->vertex_attribute_array[l_index].attribute_name + 2) << std::endl;
        
        PrintIndent(indent_level + 1);
        std::cout << "(";
        PrintSideSpace(PrintSide::Left);
        std::cout << "AttributeFormat: " << dd::res::GfxAttributeFormatToString(static_cast<dd::res::GfxAttributeFormat>(l_vertex->vertex_attribute_array[l_index].attribute_format)) << ")" << std::endl;
    }

    if (is_print == true) { return true; }

    size_t l_offset = l_vertex->base_memory_offset;
    size_t r_offset = r_vertex->base_memory_offset;
    for (u32 i = 0; i < l_vertex->vertex_buffer_count; ++i) {

        /* Compare strides */
        if (l_vertex->vertex_buffer_stride_info_array[i].stride != r_vertex->vertex_buffer_stride_info_array[i].stride
         || l_vertex->vertex_buffer_stride_info_array[i].divisor != r_vertex->vertex_buffer_stride_info_array[i].divisor) { return false; }

        /* Compare buffer info */
        if (l_vertex->vertex_buffer_info_array[i].gpu_access_flags != r_vertex->vertex_buffer_info_array[i].gpu_access_flags
         || l_vertex->vertex_buffer_info_array[i].size != r_vertex->vertex_buffer_info_array[i].size) { return false; }

        /* Find vertices in gpu memory */
        void *l_vertices = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(l_gpu_memory) + l_offset);
        void *r_vertices = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(r_gpu_memory) + r_offset);

        /* Advance memory offsets */
        l_offset = dd::util::AlignUp(l_offset + l_vertex->vertex_buffer_info_array[i].size, (l_vertex->vertex_buffer_alignment != 0) ? l_vertex->vertex_buffer_alignment : 8);
        r_offset = dd::util::AlignUp(r_offset + r_vertex->vertex_buffer_info_array[i].size, (r_vertex->vertex_buffer_alignment != 0) ? r_vertex->vertex_buffer_alignment : 8);

        /* Compare vertex buffers */
        if (::memcmp(l_vertices, r_vertices, l_vertex->vertex_buffer_info_array[i].size) != 0) { return false; }
    }

    return true;
}

bool DiffBfresMesh(dd::res::ResBfresMesh *l_mesh, dd::res::ResBfresMesh *r_mesh, void *l_gpu_memory, void *r_gpu_memory, u32 indent_level, bool is_print) {

    /* Compare sub mesh count */
    if (l_mesh->sub_mesh_count != r_mesh->sub_mesh_count) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << std::dec << "(left SubMeshCount: " << static_cast<u32>(l_mesh->sub_mesh_count) << ")(right SubMeshCount: " << static_cast<u32>(r_mesh->sub_mesh_count) << ")" << std::endl;
    }
    
    /* Compare indice count */
    if (l_mesh->index_count != r_mesh->index_count) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << std::dec << "(left IndexCount: " << l_mesh->index_count << ")(right IndexCount: " << r_mesh->index_count << ")" << std::endl;
    }

    /* Compare index format */
    if (l_mesh->index_format != r_mesh->index_format) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left IndexFormat: " << dd::res::GfxIndexFormatToString(static_cast<dd::res::GfxIndexFormat>(l_mesh->index_format)) << ")(right IndexFormat: " << dd::res::GfxIndexFormatToString(static_cast<dd::res::GfxIndexFormat>(r_mesh->index_format)) << ")" << std::endl;
    }

    /* Compare primitive topology */
    if (l_mesh->primitive_topology != r_mesh->primitive_topology) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left PrimitiveTopology: " << dd::res::GfxPrimitiveTopologyToString(static_cast<dd::res::GfxPrimitiveTopology>(l_mesh->primitive_topology)) << ")(right PrimitiveTopology: " << dd::res::GfxPrimitiveTopologyToString(static_cast<dd::res::GfxPrimitiveTopology>(r_mesh->primitive_topology)) << ")" << std::endl;
    }
    
    /* Compare sub meshes */
    if (is_print == true) { return true; }

    /* Compare base index */
    if (l_mesh->base_index != r_mesh->base_index) { return false; }
    
    /* Compare index buffer info */
    if (l_mesh->index_buffer_info->size != r_mesh->index_buffer_info->size
     || l_mesh->index_buffer_info->gpu_access_flags != r_mesh->index_buffer_info->gpu_access_flags) { return false; }

    u32 l_offset = l_mesh->index_buffer_memory_offset;
    u32 r_offset = r_mesh->index_buffer_memory_offset;
    for (u32 i = 0; i < l_mesh->sub_mesh_count; ++i) {

        /* Compare sub mesh ranges */
        if (l_mesh->sub_mesh_range_array[i].base_memory_offset != r_mesh->sub_mesh_range_array[i].base_memory_offset
         || l_mesh->sub_mesh_range_array[i].index_count != r_mesh->sub_mesh_range_array[i].index_count) { return false; }

        /* Compare index buffer */
        s32 base_index = static_cast<s32>(l_mesh->base_index);
        for (u32 y = 0; y < i; ++y) {
            base_index = base_index - l_mesh->sub_mesh_range_array[y].index_count;
            if (base_index < 0) { base_index = 0; break; }
        }
        if (l_mesh->sub_mesh_range_array[i].index_count < static_cast<u32>(base_index)) {
            continue;
        }

        void *l_indices = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(l_gpu_memory) + l_offset + l_mesh->sub_mesh_range_array[i].base_memory_offset + (static_cast<u32>(base_index) << l_mesh->index_format));
        void *r_indices = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(r_gpu_memory) + r_offset + r_mesh->sub_mesh_range_array[i].base_memory_offset + (static_cast<u32>(base_index) << r_mesh->index_format));

        if (::memcmp(l_indices, r_indices, (l_mesh->sub_mesh_range_array[i].index_count - base_index) << l_mesh->index_format) != 0) { return false; }
    }

    return true;
}

bool DiffBfresShape(dd::res::ResBfresShape *l_shape, dd::res::ResBfresShape *r_shape, dd::res::ResBfresModel *l_model, dd::res::ResBfresModel *r_model, void *l_gpu_memory, void *r_gpu_memory, u32 indent_level, bool is_print) {

    /* Compare ResVertex */
    const bool vertex_result = DiffBfresVertex(l_shape->vertex, r_shape->vertex, l_gpu_memory, r_gpu_memory, indent_level + 1, false);
    if (vertex_result == false) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(vertex buffers):" << std::endl;
        DiffBfresVertex(l_shape->vertex, r_shape->vertex, l_gpu_memory, r_gpu_memory, indent_level + 1, true);
    }

    /* Compare ResMaterial */
    const bool material_result = DiffBfresMaterial(std::addressof(l_model->material_array[l_shape->material_index]), std::addressof(r_model->material_array[r_shape->material_index]), indent_level + 1, false);
    if (material_result == false) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        if (::strcmp((l_model->material_array[l_shape->material_index].material_name + 2), (r_model->material_array[r_shape->material_index].material_name + 2)) == 0) {
            std::cout << "(material: " << (l_model->material_array[l_shape->material_index].material_name + 2) << ")" << std::endl;
        } else {
            std::cout << "(left material: " << (l_model->material_array[l_shape->material_index].material_name + 2) << ")(right material: " << (r_model->material_array[r_shape->material_index].material_name + 2) << ")" << std::endl;
        }
    }

    /* Compare ResMesh */
    if (l_shape->mesh_count != r_shape->mesh_count) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << std::dec << "(left MeshCount: " << static_cast<u32>(l_shape->mesh_count) << ")(right MeshCount: " << static_cast<u32>(r_shape->mesh_count) << ")" << std::endl;
    }

    if (l_shape->mesh_count < r_shape->mesh_count) {
        if (is_print == false) { return false; }
        
        for (u32 i = 0; i < l_shape->mesh_count; ++i) {
            const bool diff_result = DiffBfresMesh(std::addressof(l_shape->mesh_array[i]), std::addressof(r_shape->mesh_array[i]), l_gpu_memory, r_gpu_memory, indent_level + 1, false);
            if (diff_result == false) {

                PrintIndent(indent_level);
                std::cout << "(mesh " << i << "): " << std::endl;
                DiffBfresMesh(std::addressof(l_shape->mesh_array[i]), std::addressof(r_shape->mesh_array[i]), l_gpu_memory, r_gpu_memory, indent_level + 1, true);
            }
        }
        for (u32 i = l_shape->mesh_count; i < r_shape->mesh_count; ++i) {
            //ProcessBfresMeshSingle(std::addressof(r_shape->mesh_array[i]), r_gpu_memory, indent_level + 1, PrintSide::Right);
        }
    } else if (l_shape->mesh_count > r_shape->mesh_count) {
        if (is_print == false) { return false; }
        
        for (u32 i = 0; i < r_shape->mesh_count; ++i) {
            const bool diff_result = DiffBfresMesh(std::addressof(l_shape->mesh_array[i]), std::addressof(r_shape->mesh_array[i]), l_gpu_memory, r_gpu_memory, indent_level + 1, false);
            if (diff_result == false) {

                PrintIndent(indent_level);
                std::cout << "(mesh " << i << "): " << std::endl;
                DiffBfresMesh(std::addressof(l_shape->mesh_array[i]), std::addressof(r_shape->mesh_array[i]), l_gpu_memory, r_gpu_memory, indent_level + 1, true);
            }
        }
        for (u32 i = r_shape->mesh_count; i < l_shape->mesh_count; ++i) {
            //ProcessBfresMeshSingle(std::addressof(l_shape->mesh_array[i]), l_gpu_memory, indent_level + 1, PrintSide::Left);
        }
    } else {
        for (u32 i = 0; i < r_shape->mesh_count; ++i) {
            const bool diff_result = DiffBfresMesh(std::addressof(l_shape->mesh_array[i]), std::addressof(r_shape->mesh_array[i]), l_gpu_memory, r_gpu_memory, indent_level + 1, false);
            if (diff_result == false) {
                if (is_print == false) { return false; }

                PrintIndent(indent_level);
                std::cout << "(mesh " << i << "): " << std::endl;
                DiffBfresMesh(std::addressof(l_shape->mesh_array[i]), std::addressof(r_shape->mesh_array[i]), l_gpu_memory, r_gpu_memory, indent_level + 1, true);
            }
        }
    }

    /* Compare full bounding box */
    dd::res::ResBfresBounding *l_bounding = l_shape->bounding_box_array + l_shape->mesh_array[0].sub_mesh_count;
    dd::res::ResBfresBounding *r_bounding = r_shape->bounding_box_array + r_shape->mesh_array[0].sub_mesh_count;
    if (l_bounding->center_position[0] != r_bounding->center_position[0] || l_bounding->center_position[1] != r_bounding->center_position[1] || l_bounding->center_position[2] != r_bounding->center_position[2] 
     || l_bounding->extents[0] != r_bounding->extents[0] || l_bounding->extents[1] != r_bounding->extents[1] || l_bounding->extents[2] != r_bounding->extents[2]) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left BoundingBox: pos(" << l_bounding->center_position[0] << ", " << l_bounding->center_position[1] << ", " << l_bounding->center_position[1] << "), extents(";
        std::cout << l_bounding->extents[0] << ", " << l_bounding->extents[1] << ", " << l_bounding->extents[1] << "))";
        std::cout << "(right BoundingBox: pos(" << r_bounding->center_position[0] << ", " << r_bounding->center_position[1] << ", " << r_bounding->center_position[1] << "), extents(";
        std::cout << r_bounding->extents[0] << ", " << r_bounding->extents[1] << ", " << r_bounding->extents[1] << "))" << std::endl;
    }

    /* Compare bounding sphere */
    float *l_bounding_sphere = l_shape->bounding_sphere_radius_array + l_shape->mesh_array[0].sub_mesh_count;
    float *r_bounding_sphere = r_shape->bounding_sphere_radius_array + r_shape->mesh_array[0].sub_mesh_count;
    if (*l_bounding_sphere != *r_bounding_sphere) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left BoundingSphereRadius: " << *l_bounding_sphere << ")(right BoundingSphereRadius: " << *r_bounding_sphere << ")" << std::endl;
    }

    /* Compare KeyShapes */
    RomfsDirectoryParser l_key_shape_iterator = {};
    l_key_shape_iterator.InitializeByResDic(l_shape->key_shape_dictionary);
    RomfsDirectoryParser r_key_shape_iterator = {};
    r_key_shape_iterator.InitializeByResDic(r_shape->key_shape_dictionary);

    char **l_key_shape_paths = l_key_shape_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_key_shape_iterator.GetFileCount(); ++i) {
        const u32 r_path_index = r_key_shape_iterator.FindPathIndex(l_key_shape_paths[i]);
        if (r_path_index == RomfsDirectoryParser::InvalidIndex) { if (is_print == false) { return false; } continue; }

        const u32 l_index = l_shape->key_shape_dictionary->FindEntryIndex(l_key_shape_paths[i]);
        const u32 r_index = r_shape->key_shape_dictionary->FindEntryIndex(l_key_shape_paths[i]);

        /* Compare key shape attributes */
        bool first_time_print = true;
        for (u32 y = 0; y < 18; ++y) {
            const u32 l_location = l_shape->key_shape_array[l_index].key_shape_attribute_location_array[y];
            const u32 r_location = r_shape->key_shape_array[r_index].key_shape_attribute_location_array[y];
            if (l_location == 0 && r_location == 0) { continue; }

            const char *l_attribute_name = (l_location != 0) ? l_shape->vertex->vertex_attribute_array[l_location].attribute_name + 2 : "";
            const char *r_attribute_name = (r_location != 0) ? r_shape->vertex->vertex_attribute_array[r_location].attribute_name + 2 : "";
            if (::strcmp(l_attribute_name, r_attribute_name) != 0) {
                if (is_print == false) { return false; }

                if (first_time_print == true) {
                    first_time_print = false;

                    PrintIndent(indent_level);
                    std::cout << "(shape key): " << l_key_shape_paths[i] << std::endl;
                }

                PrintIndent(indent_level + 1);
                std::cout << "(left VertexAttribute: " << l_attribute_name << ")(right VertexAttribute: " << r_attribute_name << ")" << std::endl;
            }
        }
    }

    /* Compare skin bone indice array */

    return true;
}
