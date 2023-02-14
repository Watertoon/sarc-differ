#pragma once

void PrintRenderInfoValue(dd::res::ResBfresMaterial *material, const char *render_info_name, PrintSide print_side) {

    const u32 entry_index = material->material_shader_data->shader_reflection->render_info_dictionary->FindEntryIndex(render_info_name);
    dd::res::ResBfresRenderInfo *render_info = std::addressof(material->material_shader_data->shader_reflection->render_info_array[entry_index]);

    switch (render_info->data_type) {
        case dd::res::BfresRenderInfoDataType::S32:
        {
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "S32: ";

            if (material->render_info_value_count_array[entry_index] == 0) {
                std::cout << "No Value)";
                break;
            }

            s32 *s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(material->render_info_value_array) + material->render_info_value_offset_array[entry_index]);
            for (s32 i = 0; i < material->render_info_value_count_array[entry_index] - 1; ++i) {
                std::cout << s32_values[i] << ", ";
            }
            std::cout << s32_values[material->render_info_value_count_array[entry_index] - 1] << ")";
            break;
        }
        case dd::res::BfresRenderInfoDataType::Float:
        {
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Float: ";

            if (material->render_info_value_count_array[entry_index] == 0) {
                std::cout << "No Value)";
                break;
            }

            float *float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->render_info_value_array) + material->render_info_value_offset_array[entry_index]);
            for (s32 i = 0; i < material->render_info_value_count_array[entry_index] - 1; ++i) {
                std::cout << float_values[i] << ", ";
            }
            std::cout << float_values[material->render_info_value_count_array[entry_index] - 1] << ")";
            break;
        }
        case dd::res::BfresRenderInfoDataType::String:
        {
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "String: ";

            if (material->render_info_value_count_array[entry_index] == 0) {
                std::cout << "No Value)";
                break;
            }

            const char **string_values = reinterpret_cast<const char**>(reinterpret_cast<uintptr_t>(material->render_info_value_array) + material->render_info_value_offset_array[entry_index]);
            for (s32 i = 0; i < material->render_info_value_count_array[entry_index] - 1; ++i) {
                std::cout << "\"" << (string_values[i] + 2) << "\", ";
            }
            std::cout << "\"" << (string_values[material->render_info_value_count_array[entry_index] - 1] + 2) << "\")";
            break;
        }
        default:
            std::cout << "Unknown RenderInfo data type found";
            break;
    }

    return;
}
bool CompareRenderInfoValue(dd::res::ResBfresMaterial *l_material, dd::res::ResBfresMaterial *r_material, const char *render_info_name) {

    const u32 l_entry_index = l_material->material_shader_data->shader_reflection->render_info_dictionary->FindEntryIndex(render_info_name);
    dd::res::ResBfresRenderInfo *l_render_info = std::addressof(l_material->material_shader_data->shader_reflection->render_info_array[l_entry_index]);

    const u32 r_entry_index = r_material->material_shader_data->shader_reflection->render_info_dictionary->FindEntryIndex(render_info_name);
    dd::res::ResBfresRenderInfo *r_render_info = std::addressof(r_material->material_shader_data->shader_reflection->render_info_array[r_entry_index]);

    if (l_render_info->data_type != r_render_info->data_type || l_material->render_info_value_count_array[l_entry_index] != r_material->render_info_value_count_array[r_entry_index]) {
        return false;
    }

    switch (l_render_info->data_type) {
        case dd::res::BfresRenderInfoDataType::S32:
        {
            s32 *l_s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(l_material->render_info_value_array) + l_material->render_info_value_offset_array[l_entry_index]);
            s32 *r_s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(r_material->render_info_value_array) + r_material->render_info_value_offset_array[r_entry_index]);
            bool result = 1;
            for (s32 i = 0; i < l_material->render_info_value_count_array[l_entry_index]; ++i) {
                result &= (l_s32_values[i] == r_s32_values[i]);
            }
            return result;
        }
        case dd::res::BfresRenderInfoDataType::Float:
        {
            float *l_float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->render_info_value_array) + l_material->render_info_value_offset_array[l_entry_index]);
            float *r_float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->render_info_value_array) + r_material->render_info_value_offset_array[r_entry_index]);
            bool result = 1;
            for (s32 i = 0; i < l_material->render_info_value_count_array[l_entry_index]; ++i) {
                result &= (l_float_values[i] == r_float_values[i]);
            }
            return result;
        }
        case dd::res::BfresRenderInfoDataType::String:
        {
            const char **l_string_values = reinterpret_cast<const char**>(reinterpret_cast<uintptr_t>(l_material->render_info_value_array) + l_material->render_info_value_offset_array[l_entry_index]);
            const char **r_string_values = reinterpret_cast<const char**>(reinterpret_cast<uintptr_t>(r_material->render_info_value_array) + r_material->render_info_value_offset_array[r_entry_index]);
            bool result = 1;
            for (s32 i = 0; i < l_material->render_info_value_count_array[l_entry_index]; ++i) {
                result &= (::strcmp(l_string_values[i] + 2, r_string_values[i] + 2) == 0);
            }
            return result;
        }
        default:
            break;
    }

    return false;
}

constexpr ALWAYS_INLINE const char *GetSrtModeName(dd::res::BfresSrtMode srt_mode) {
    switch(srt_mode) {
        case dd::res::BfresSrtMode::Maya:
            return "Maya";
        case dd::res::BfresSrtMode::Max:
            return "Max";
        case dd::res::BfresSrtMode::SoftImage:
            return "SoftImage";
        default:
            break;
    }
    return "Unknown SrtMode";
}

void PrintShaderParamValue(dd::res::ResBfresMaterial *material, const char *shader_param_name, PrintSide print_side) {

    const u32 entry_index = material->material_shader_data->shader_reflection->shader_param_dictionary->FindEntryIndex(shader_param_name);
    dd::res::ResBfresShaderParam *shader_param = std::addressof(material->material_shader_data->shader_reflection->shader_param_array[entry_index]);

    switch (shader_param->data_type) {
        case dd::res::BfresShaderParamDataType::Bool:
        {
            bool *bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Bool: " << bool_values[0] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec2Bool:
        {
            bool *bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec2Bool: " << bool_values[0] << ", " << bool_values[1] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec3Bool:
        {
            bool *bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec3Bool: " << bool_values[0] << ", " << bool_values[1] << ", " << bool_values[2] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec4Bool:
        {
            bool *bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec4Bool: " << bool_values[0] << ", " << bool_values[1] << ", " << bool_values[2] << ", " << bool_values[3] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::S32:
        {
            s32 *s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "S32: " << s32_values[0] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec2S32:
        {
            s32 *s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec2S32: " << s32_values[0] << ", " << s32_values[1] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec3S32:
        {
            s32 *s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec3S32: " << s32_values[0] << ", " << s32_values[1] << ", " << s32_values[2] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec4S32:
        {
            s32 *s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec4S32: " << s32_values[0] << ", " << s32_values[1] << ", " << s32_values[2] << ", " << s32_values[3] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::U32:
        {
            u32 *u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "U32: " << u32_values[0] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec2U32:
        {
            u32 *u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec2U32: " << u32_values[0] << ", " << u32_values[1] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec3U32:
        {
            u32 *u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec3U32: " << u32_values[0] << ", " << u32_values[1] << ", " << u32_values[2] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec4U32:
        {
            u32 *u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec4U32: " << u32_values[0] << ", " << u32_values[1] << ", " << u32_values[2] << ", " << u32_values[3] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Float:
        {
            float *float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Float: " << float_values[0] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec2Float:
        {
            float *float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec2Float: " << float_values[0] << ", " << float_values[1] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec3Float:
        {
            float *float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec3Float: " << float_values[0] << ", " << float_values[1] << ", " << float_values[2] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec4Float:
        {
            float *float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Vec4Float: " << float_values[0] << ", " << float_values[1] << ", " << float_values[2] << ", " << float_values[3] << ")";
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix22f:
        {
            float *mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Matrix22f: row1(" << mtx_values[0] << ", " << mtx_values[1] <<
                                  ") row2(" << mtx_values[2] << ", " << mtx_values[3] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix23f:
        {
            float *mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Matrix23f: row1(" << mtx_values[0] << ", " << mtx_values[1] << ", " << mtx_values[2] <<
                                  ") row2(" << mtx_values[3] << ", " << mtx_values[4] << ", " << mtx_values[5] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix24f:
        {
            float *mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Matrix24f: row1(" << mtx_values[0] << ", " << mtx_values[1] << ", " << mtx_values[2] << ", " << mtx_values[3] <<
                                  ") row2(" << mtx_values[4] << ", " << mtx_values[5] << ", " << mtx_values[6] << ", " << mtx_values[7] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix32f:
        {
            float *mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Matrix32f: row1(" << mtx_values[0] << ", " << mtx_values[1] <<
                                  ") row2(" << mtx_values[2] << ", " << mtx_values[3] <<
                                  ") row3(" << mtx_values[4] << ", " << mtx_values[5] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix33f:
        {
            float *mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Matrix33f: row1(" << mtx_values[0] << ", " << mtx_values[1] << ", " << mtx_values[2] <<
                                  ") row2(" << mtx_values[3] << ", " << mtx_values[4] << ", " << mtx_values[5] <<
                                  ") row3(" << mtx_values[6] << ", " << mtx_values[7] << ", " << mtx_values[8] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix34f:
        {
            float *mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Matrix34f: row1(" << mtx_values[0] << ", " << mtx_values[1] << ", " << mtx_values[2] << ", " << mtx_values[3] <<
                                  ") row2(" << mtx_values[4] << ", " << mtx_values[5] << ", " << mtx_values[6] << ", " << mtx_values[7] <<
                                  ") row3(" << mtx_values[8] << ", " << mtx_values[9] << ", " << mtx_values[10] << ", " << mtx_values[11] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix42f:
        {
            float *mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Matrix42f: row1(" << mtx_values[0] << ", " << mtx_values[1] <<
                                  ") row2(" << mtx_values[2] << ", " << mtx_values[3] <<
                                  ") row3(" << mtx_values[4] << ", " << mtx_values[5] <<
                                  ") row4(" << mtx_values[6] << ", " << mtx_values[7] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix43f:
        {
            float *mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Matrix43f: row1(" << mtx_values[0] << ", " << mtx_values[1] << ", " << mtx_values[2] <<
                                  ") row2(" << mtx_values[3] << ", " << mtx_values[4] << ", " << mtx_values[5] <<
                                  ") row3(" << mtx_values[6] << ", " << mtx_values[7] << ", " << mtx_values[8] <<
                                  ") row4(" << mtx_values[9] << ", " << mtx_values[10] << ", " << mtx_values[11] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix44f:
        {
            float *mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Matrix44f: row1(" << mtx_values[0] << ", " << mtx_values[1] << ", " << mtx_values[2] << ", " << mtx_values[3] <<
                                  ") row2(" << mtx_values[4] << ", " << mtx_values[5] << ", " << mtx_values[6] << ", " << mtx_values[7] <<
                                  ") row3(" << mtx_values[8] << ", " << mtx_values[9] << ", " << mtx_values[10] << ", " << mtx_values[11] <<
                                  ") row4(" << mtx_values[12] << ", " << mtx_values[13] << ", " << mtx_values[14] << ", " << mtx_values[15] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::Srt2d:
        {
            dd::res::ResBfresSrt2d *srt_values = reinterpret_cast<dd::res::ResBfresSrt2d*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Srt2d: trans(" << srt_values->translation[0] << ", " << srt_values->translation[1] <<
                              ") rot("   << srt_values->rotation <<
                              ") scale(" << srt_values->scale[0] << ", " << srt_values->scale[1] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::Srt3d:
        {
            dd::res::ResBfresSrt3d *srt_values = reinterpret_cast<dd::res::ResBfresSrt3d*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "Srt3d: trans(" << srt_values->translation[0] << ", " << srt_values->translation[1] << ", " << srt_values->translation[2] <<
                              ") rot("   << srt_values->rotation[0] << ", " << srt_values->rotation[1] << ", " << srt_values->rotation[2] <<
                              ") scale(" << srt_values->scale[0] << ", " << srt_values->scale[1] << ", " << srt_values->scale[2] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::SrtTexture:
        {
            dd::res::ResBfresTextureSrt *srt_values = reinterpret_cast<dd::res::ResBfresTextureSrt*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "SrtTexture: mode(" << GetSrtModeName(srt_values->srt_mode) <<
                                  ") trans(" << srt_values->translation[0] << ", " << srt_values->translation[1] <<
                                  ") rot("   << srt_values->rotation <<
                                  ") scale(" << srt_values->scale[0] << ", " << srt_values->scale[1] << "))";
            break;
        }
        case dd::res::BfresShaderParamDataType::SrtTextureWithUserPointer:
        {
            dd::res::ResBfresTextureSrtWithUserPointer *srt_values = reinterpret_cast<dd::res::ResBfresTextureSrtWithUserPointer*>(reinterpret_cast<uintptr_t>(material->shader_param_value_array) + shader_param->shader_param_offset);
            std::cout << "(";
            PrintSideSpace(print_side);
            std::cout << "SrtTextureWithUserPointer: mode(" << GetSrtModeName(srt_values->srt_mode) <<
                                                 ") trans(" << srt_values->translation[0] << ", " << srt_values->translation[1] <<
                                                 ") rot("   << srt_values->rotation <<
                                                 ") scale(" << srt_values->scale[0] << ", " << srt_values->scale[1] << "))";
            break;
        }
        default:
            std::cout << "Unknown ShaderParam data type found";
            break;
    }

    return;
}

bool CompareShaderParamValue(dd::res::ResBfresMaterial *l_material, dd::res::ResBfresMaterial *r_material, const char *shader_param_name) {
    
    const u32 l_entry_index = l_material->material_shader_data->shader_reflection->shader_param_dictionary->FindEntryIndex(shader_param_name);
    dd::res::ResBfresShaderParam *l_shader_param = std::addressof(l_material->material_shader_data->shader_reflection->shader_param_array[l_entry_index]);

    const u32 r_entry_index = r_material->material_shader_data->shader_reflection->shader_param_dictionary->FindEntryIndex(shader_param_name);
    dd::res::ResBfresShaderParam *r_shader_param = std::addressof(r_material->material_shader_data->shader_reflection->shader_param_array[r_entry_index]);

    if (l_shader_param->data_type != r_shader_param->data_type) {
        return false;
    }
    
    bool result = true;
    switch (l_shader_param->data_type) {
        case dd::res::BfresShaderParamDataType::Bool:
        {
            bool *l_bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            bool *r_bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_bool_values[0] == r_bool_values[0]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec2Bool:
        {
            bool *l_bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            bool *r_bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_bool_values[0] == r_bool_values[0]) & (l_bool_values[1] == r_bool_values[1]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec3Bool:
        {
            bool *l_bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            bool *r_bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_bool_values[0] == r_bool_values[0]) & (l_bool_values[1] == r_bool_values[1]) & (l_bool_values[2] == r_bool_values[2]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec4Bool:
        {
            bool *l_bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            bool *r_bool_values = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_bool_values[0] == r_bool_values[0]) & (l_bool_values[1] == r_bool_values[1]) & (l_bool_values[2] == r_bool_values[2]) & (l_bool_values[3] == r_bool_values[3]);
            break;
        }
        case dd::res::BfresShaderParamDataType::S32:
        {
            s32 *l_s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            s32 *r_s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_s32_values[0] == r_s32_values[0]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec2S32:
        {
            s32 *l_s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            s32 *r_s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_s32_values[0] == r_s32_values[0]) & (l_s32_values[1] == r_s32_values[1]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec3S32:
        {
            s32 *l_s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            s32 *r_s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_s32_values[0] == r_s32_values[0]) & (l_s32_values[1] == r_s32_values[1]) & (l_s32_values[2] == r_s32_values[2]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec4S32:
        {
            s32 *l_s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            s32 *r_s32_values = reinterpret_cast<s32*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_s32_values[0] == r_s32_values[0]) & (l_s32_values[1] == r_s32_values[1]) & (l_s32_values[2] == r_s32_values[2]) & (l_s32_values[3] == r_s32_values[3]);
            break;
        }
        case dd::res::BfresShaderParamDataType::U32:
        {
            u32 *l_u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            u32 *r_u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_u32_values[0] == r_u32_values[0]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec2U32:
        {
            u32 *l_u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            u32 *r_u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_u32_values[0] == r_u32_values[0]) & (l_u32_values[1] == r_u32_values[1]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec3U32:
        {
            u32 *l_u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            u32 *r_u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_u32_values[0] == r_u32_values[0]) & (l_u32_values[1] == r_u32_values[1]) & (l_u32_values[2] == r_u32_values[2]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec4U32:
        {
            u32 *l_u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            u32 *r_u32_values = reinterpret_cast<u32*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_u32_values[0] == r_u32_values[0]) & (l_u32_values[1] == r_u32_values[1]) & (l_u32_values[2] == r_u32_values[2]) & (l_u32_values[3] == r_u32_values[3]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Float:
        {
            float *l_float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_float_values[0] == r_float_values[0]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec2Float:
        {
            float *l_float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_float_values[0] == r_float_values[0]) & (l_float_values[1] == r_float_values[1]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec3Float:
        {
            float *l_float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_float_values[0] == r_float_values[0]) & (l_float_values[1] == r_float_values[1]) & (l_float_values[2] == r_float_values[2]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Vec4Float:
        {
            float *l_float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_float_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_float_values[0] == r_float_values[0]) & (l_float_values[1] == r_float_values[1]) & (l_float_values[2] == r_float_values[2]) & (l_float_values[3] == r_float_values[3]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix22f:
        {
            float *l_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_mtx_values[0] == r_mtx_values[0]) & (l_mtx_values[1] == r_mtx_values[1])
                    & (l_mtx_values[2] == r_mtx_values[2]) & (l_mtx_values[3] == r_mtx_values[3]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix23f:
        {
            float *l_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_mtx_values[0] == r_mtx_values[0]) & (l_mtx_values[1] == r_mtx_values[1]) & (l_mtx_values[2] == r_mtx_values[2])
                    & (l_mtx_values[3] == r_mtx_values[3]) & (l_mtx_values[4] == r_mtx_values[4]) & (l_mtx_values[5] == r_mtx_values[5]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix24f:
        {
            float *l_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_mtx_values[0] == r_mtx_values[0]) & (l_mtx_values[1] == r_mtx_values[1]) & (l_mtx_values[2] == r_mtx_values[2]) & (l_mtx_values[3] == r_mtx_values[3])
                    & (l_mtx_values[4] == r_mtx_values[4]) & (l_mtx_values[5] == r_mtx_values[5]) & (l_mtx_values[6] == r_mtx_values[6]) & (l_mtx_values[7] == r_mtx_values[7]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix32f:
        {
            float *l_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_mtx_values[0] == r_mtx_values[0]) & (l_mtx_values[1] == r_mtx_values[1]) 
                    & (l_mtx_values[2] == r_mtx_values[2]) & (l_mtx_values[3] == r_mtx_values[3]) 
                    & (l_mtx_values[4] == r_mtx_values[4]) & (l_mtx_values[5] == r_mtx_values[5]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix33f:
        {
            float *l_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_mtx_values[0] == r_mtx_values[0]) & (l_mtx_values[1] == r_mtx_values[1]) & (l_mtx_values[2] == r_mtx_values[2]) 
                    & (l_mtx_values[3] == r_mtx_values[3]) & (l_mtx_values[4] == r_mtx_values[4]) & (l_mtx_values[5] == r_mtx_values[5]) 
                    & (l_mtx_values[6] == r_mtx_values[6]) & (l_mtx_values[7] == r_mtx_values[7]) & (l_mtx_values[8] == r_mtx_values[8]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix34f:
        {
            float *l_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_mtx_values[0] == r_mtx_values[0]) & (l_mtx_values[1] == r_mtx_values[1]) & (l_mtx_values[2] == r_mtx_values[2]) & (l_mtx_values[3] == r_mtx_values[3])
                    & (l_mtx_values[4] == r_mtx_values[4]) & (l_mtx_values[5] == r_mtx_values[5]) & (l_mtx_values[6] == r_mtx_values[6]) & (l_mtx_values[7] == r_mtx_values[7])
                    & (l_mtx_values[8] == r_mtx_values[8]) & (l_mtx_values[9] == r_mtx_values[9]) & (l_mtx_values[10] == r_mtx_values[10]) & (l_mtx_values[11] == r_mtx_values[11]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix42f:
        {
            float *l_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_mtx_values[0] == r_mtx_values[0]) & (l_mtx_values[1] == r_mtx_values[1]) 
                    & (l_mtx_values[2] == r_mtx_values[2]) & (l_mtx_values[3] == r_mtx_values[3]) 
                    & (l_mtx_values[4] == r_mtx_values[4]) & (l_mtx_values[5] == r_mtx_values[5]) 
                    & (l_mtx_values[6] == r_mtx_values[6]) & (l_mtx_values[7] == r_mtx_values[7]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix43f:
        {
            float *l_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_mtx_values[0] == r_mtx_values[0]) & (l_mtx_values[1] == r_mtx_values[1]) & (l_mtx_values[2] == r_mtx_values[2])
                    & (l_mtx_values[3] == r_mtx_values[3]) & (l_mtx_values[4] == r_mtx_values[4]) & (l_mtx_values[5] == r_mtx_values[5])
                    & (l_mtx_values[6] == r_mtx_values[6]) & (l_mtx_values[7] == r_mtx_values[7]) & (l_mtx_values[8] == r_mtx_values[8])
                    & (l_mtx_values[9] == r_mtx_values[9]) & (l_mtx_values[10] == r_mtx_values[10]) & (l_mtx_values[11] == r_mtx_values[11]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Matrix44f:
        {
            float *l_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            float *r_mtx_values = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_mtx_values[0] == r_mtx_values[0]) & (l_mtx_values[1] == r_mtx_values[1]) & (l_mtx_values[2] == r_mtx_values[2]) & (l_mtx_values[3] == r_mtx_values[3])
                    & (l_mtx_values[4] == r_mtx_values[4]) & (l_mtx_values[5] == r_mtx_values[5]) & (l_mtx_values[6] == r_mtx_values[6]) & (l_mtx_values[7] == r_mtx_values[7])
                    & (l_mtx_values[8] == r_mtx_values[8]) & (l_mtx_values[9] == r_mtx_values[9]) & (l_mtx_values[10] == r_mtx_values[10]) & (l_mtx_values[11] == r_mtx_values[11])
                    & (l_mtx_values[12] == r_mtx_values[12]) & (l_mtx_values[13] == r_mtx_values[13]) & (l_mtx_values[14] == r_mtx_values[14]) & (l_mtx_values[15] == r_mtx_values[15]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Srt2d:
        {
            dd::res::ResBfresSrt2d *l_srt_values = reinterpret_cast<dd::res::ResBfresSrt2d*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            dd::res::ResBfresSrt2d *r_srt_values = reinterpret_cast<dd::res::ResBfresSrt2d*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_srt_values->translation[0] == r_srt_values->translation[0]) & (l_srt_values->translation[1] == r_srt_values->translation[1])
                    & (l_srt_values->rotation == r_srt_values->rotation)
                    & (l_srt_values->scale[0] == r_srt_values->scale[0]) & (l_srt_values->scale[1] == r_srt_values->scale[1]);
            break;
        }
        case dd::res::BfresShaderParamDataType::Srt3d:
        {
            dd::res::ResBfresSrt3d *l_srt_values = reinterpret_cast<dd::res::ResBfresSrt3d*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            dd::res::ResBfresSrt3d *r_srt_values = reinterpret_cast<dd::res::ResBfresSrt3d*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_srt_values->translation[0] == r_srt_values->translation[0]) & (l_srt_values->translation[1] == r_srt_values->translation[1]) & (l_srt_values->translation[2] == r_srt_values->translation[2])
                    & (l_srt_values->rotation[0] == r_srt_values->rotation[0]) & (l_srt_values->rotation[1] == r_srt_values->rotation[1]) & (l_srt_values->rotation[2] == r_srt_values->rotation[2])
                    & (l_srt_values->scale[0] == r_srt_values->scale[0]) & (l_srt_values->scale[1] == r_srt_values->scale[1]) & (l_srt_values->scale[2] == r_srt_values->scale[2]);
            break;
            break;
        }
        case dd::res::BfresShaderParamDataType::SrtTexture:
        {
            dd::res::ResBfresTextureSrt *l_srt_values = reinterpret_cast<dd::res::ResBfresTextureSrt*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            dd::res::ResBfresTextureSrt *r_srt_values = reinterpret_cast<dd::res::ResBfresTextureSrt*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_srt_values->srt_mode == r_srt_values->srt_mode);
            result &= (l_srt_values->translation[0] == r_srt_values->translation[0]) & (l_srt_values->translation[1] == r_srt_values->translation[1])
                    & (l_srt_values->rotation == r_srt_values->rotation)
                    & (l_srt_values->scale[0] == r_srt_values->scale[0]) & (l_srt_values->scale[1] == r_srt_values->scale[1]);
            break;
        }
        case dd::res::BfresShaderParamDataType::SrtTextureWithUserPointer:
        {
            dd::res::ResBfresTextureSrtWithUserPointer *l_srt_values = reinterpret_cast<dd::res::ResBfresTextureSrtWithUserPointer*>(reinterpret_cast<uintptr_t>(l_material->shader_param_value_array) + l_shader_param->shader_param_offset);
            dd::res::ResBfresTextureSrtWithUserPointer *r_srt_values = reinterpret_cast<dd::res::ResBfresTextureSrtWithUserPointer*>(reinterpret_cast<uintptr_t>(r_material->shader_param_value_array) + r_shader_param->shader_param_offset);

            result &= (l_srt_values->srt_mode == r_srt_values->srt_mode);
            result &= (l_srt_values->translation[0] == r_srt_values->translation[0]) & (l_srt_values->translation[1] == r_srt_values->translation[1])
                    & (l_srt_values->rotation == r_srt_values->rotation)
                    & (l_srt_values->scale[0] == r_srt_values->scale[0]) & (l_srt_values->scale[1] == r_srt_values->scale[1]);
            break;
        }
        default:
            result = false;
            break;
    }
    return result;
}

bool DiffBfresMaterial(dd::res::ResBfresMaterial *l_material, dd::res::ResBfresMaterial *r_material, u32 indent_level, bool is_print) {

    RomfsDirectoryParser l_render_info_iterator = {};
    l_render_info_iterator.InitializeByResDic(l_material->material_shader_data->shader_reflection->render_info_dictionary);
    RomfsDirectoryParser l_shader_param_iterator = {};
    l_shader_param_iterator.InitializeByResDic(l_material->material_shader_data->shader_reflection->shader_param_dictionary);
    RomfsDirectoryParser l_vertex_attribute_iterator = {};
    l_vertex_attribute_iterator.InitializeByArrayNW(l_material->material_shader_data->vertex_attribute_name_array, l_material->material_shader_data->vertex_attribute_count);
    RomfsDirectoryParser l_sampler_iterator = {};
    l_sampler_iterator.InitializeByArrayNW(l_material->material_shader_data->sampler_name_array, l_material->material_shader_data->sampler_count);
    RomfsDirectoryParser l_static_shader_option_iterator = {};
    l_static_shader_option_iterator.InitializeByResDic(l_material->material_shader_data->shader_reflection->static_shader_option_dictionary);

    RomfsDirectoryParser r_render_info_iterator = {};
    r_render_info_iterator.InitializeByResDic(r_material->material_shader_data->shader_reflection->render_info_dictionary);
    RomfsDirectoryParser r_shader_param_iterator = {};
    r_shader_param_iterator.InitializeByResDic(r_material->material_shader_data->shader_reflection->shader_param_dictionary);
    RomfsDirectoryParser r_vertex_attribute_iterator = {};
    r_vertex_attribute_iterator.InitializeByArrayNW(r_material->material_shader_data->vertex_attribute_name_array, r_material->material_shader_data->vertex_attribute_count);
    RomfsDirectoryParser r_sampler_iterator = {};
    r_sampler_iterator.InitializeByArrayNW(r_material->material_shader_data->sampler_name_array, r_material->material_shader_data->sampler_count);
    RomfsDirectoryParser r_static_shader_option_iterator = {};
    r_static_shader_option_iterator.InitializeByResDic(r_material->material_shader_data->shader_reflection->static_shader_option_dictionary);

    /* Different */
    char **l_render_info_paths = l_render_info_iterator.GetFilePathArray();
    char **r_render_info_paths = r_render_info_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_render_info_iterator.GetFileCount(); ++i) {
        u32 r_index = r_render_info_iterator.FindPathIndex(l_render_info_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex && CompareRenderInfoValue(l_material, r_material, l_render_info_paths[i]) == false) {
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            std::cout << "Different(render info): " << l_render_info_paths[i];
            PrintRenderInfoValue(l_material, l_render_info_paths[i], PrintSide::Left);
            PrintRenderInfoValue(r_material, l_render_info_paths[i], PrintSide::Right);
            std::cout << std::endl;
        }
    }
    
    char **l_shader_param_paths = l_shader_param_iterator.GetFilePathArray();
    char **r_shader_param_paths = r_shader_param_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_shader_param_iterator.GetFileCount(); ++i) {
        u32 r_index = r_shader_param_iterator.FindPathIndex(l_shader_param_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex && CompareShaderParamValue(l_material, r_material, l_shader_param_paths[i]) == false) {
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            std::cout << "Different(shader param): " << l_shader_param_paths[i];
            PrintShaderParamValue(l_material, l_shader_param_paths[i], PrintSide::Left);
            PrintShaderParamValue(r_material, l_shader_param_paths[i], PrintSide::Right);
            std::cout << std::endl;
        }
    }
    
    char **l_vertex_attribute_paths = l_vertex_attribute_iterator.GetFilePathArray();
    char **r_vertex_attribute_paths = r_vertex_attribute_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_vertex_attribute_iterator.GetFileCount(); ++i) {
        u32 r_index = r_vertex_attribute_iterator.FindPathIndex(l_vertex_attribute_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {
            const char *l_attr = l_material->GetShaderVertexAttributeName(l_vertex_attribute_paths[i]);
            const char *r_attr = r_material->GetShaderVertexAttributeName(r_vertex_attribute_paths[r_index]);
            if (::strcmp(l_attr, r_attr) == 0) { continue; }
            if (is_print == false) { return false; } 

            PrintIndent(indent_level + 1);
            std::cout << "Different(vertex attribute): " << l_vertex_attribute_paths[i] << "(left: " << l_attr << ")(right: " << r_attr << ")" << std::endl;
        }
    }

    char **l_sampler_paths = l_sampler_iterator.GetFilePathArray();
    char **r_sampler_paths = r_sampler_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_sampler_iterator.GetFileCount(); ++i) {
        u32 r_index = r_sampler_iterator.FindPathIndex(l_sampler_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {
            const char *l_attr = l_material->GetShaderSamplerName(l_sampler_paths[i]);
            const char *r_attr = r_material->GetShaderSamplerName(r_sampler_paths[r_index]);
            if (::strcmp(l_attr, r_attr) == 0) { continue; }
            if (is_print == false) { return false; } 

            PrintIndent(indent_level + 1);
            std::cout << "Different(sampler): " << l_sampler_paths[i] << "(left: " << l_attr << ")(right: " << r_attr << ")" << std::endl;
        }
    }
    
    char **l_static_shader_option_paths = l_static_shader_option_iterator.GetFilePathArray();
    char **r_static_shader_option_paths = r_static_shader_option_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_static_shader_option_iterator.GetFileCount(); ++i) {
        u32 r_index = r_static_shader_option_iterator.FindPathIndex(l_static_shader_option_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {
            const u32 l_i = l_material->GetStaticShaderOptionIndex(l_static_shader_option_paths[i]);
            const u32 r_i = r_material->GetStaticShaderOptionIndex(l_static_shader_option_paths[i]);
            if (l_i == 0xffff && r_i == 0xffff) { continue; }

            if (l_i == 0xffff || r_i == 0xffff || l_material->IsStaticShaderOptionBool(l_i) != r_material->IsStaticShaderOptionBool(r_i)) {
                if (is_print == false) { return false; }
            } else if (l_material->IsStaticShaderOptionBool(l_i) == true) {
                const u32 l_bool_param = l_material->GetStaticShaderOptionBool(l_i);
                const u32 r_bool_param = r_material->GetStaticShaderOptionBool(r_i);
                if (l_bool_param == r_bool_param) { continue; }
                if (is_print == false) { return false; }
            } else {
                const char *l_string_param = l_material->GetStaticShaderOptionString(l_i) + 2;
                const char *r_string_param = r_material->GetStaticShaderOptionString(r_i) + 2;
                if (::strcmp(l_string_param, r_string_param) == 0) { continue; }
                if (is_print == false) { return false; }
            }

            /* Common */
            PrintIndent(indent_level + 1);
            std::cout << "Different(static shader option): " << l_static_shader_option_paths[i];

            /* Left */
            if (l_i == 0xffff) {
                std::cout << "(left Invalid)";
            } else if (l_material->IsStaticShaderOptionBool(l_i)) {
                const u32 bool_param = l_material->GetStaticShaderOptionBool(l_i);
                std::cout << "(left Bool: " << bool_param << ")";
            } else {
                const char *string_param = l_material->GetStaticShaderOptionString(l_i) + 2;
                std::cout << "(left String: \"" << string_param << "\")";
            }
            
            /* Right */
            if (r_i == 0xffff) {
                std::cout << "(right Invalid)";
            } else if (r_material->IsStaticShaderOptionBool(r_i)) {
                const u32 bool_param = r_material->GetStaticShaderOptionBool(r_i);
                std::cout << "(right Bool: " << bool_param << ")";
            } else {
                const char *string_param = r_material->GetStaticShaderOptionString(r_i) + 2;
                std::cout << "(right String: \"" << string_param << "\")";
            }
            std::cout << std::endl;
        }
    }
    
    /* Right only */
    for (u32 i = 0; i < r_render_info_iterator.GetFileCount(); ++i) {
        u32 l_index = l_render_info_iterator.FindPathIndex(r_render_info_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            std::cout << "Right only(render info): " << r_render_info_paths[i];
            PrintRenderInfoValue(r_material, r_render_info_paths[i], PrintSide::None);
            std::cout << std::endl;
        }
    }

    for (u32 i = 0; i < r_shader_param_iterator.GetFileCount(); ++i) {
        u32 l_index = l_shader_param_iterator.FindPathIndex(r_shader_param_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            std::cout << "Right only(shader param): " << r_shader_param_paths[i];
            PrintShaderParamValue(r_material, r_shader_param_paths[i], PrintSide::None);
            std::cout << std::endl;
        }
    }

    for (u32 i = 0; i < r_vertex_attribute_iterator.GetFileCount(); ++i) {
        u32 l_index = l_vertex_attribute_iterator.FindPathIndex(r_vertex_attribute_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            const char *r_attr = r_material->GetShaderVertexAttributeName(r_vertex_attribute_paths[i]);
            std::cout << "Right only(vertex attribute): " << r_vertex_attribute_paths[i] << "(shader: " << r_attr << ")" << std::endl;
        }
    }
    

    for (u32 i = 0; i < r_sampler_iterator.GetFileCount(); ++i) {
        u32 l_index = l_sampler_iterator.FindPathIndex(r_sampler_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            const char *r_attr = r_material->GetShaderSamplerName(r_sampler_paths[i]);
            std::cout << "Right only(sampler): " << r_sampler_paths[i] << "(shader: " << r_attr << ")" << std::endl;
        }
    }

    for (u32 i = 0; i < r_static_shader_option_iterator.GetFileCount(); ++i) {
        u32 l_index = l_static_shader_option_iterator.FindPathIndex(r_static_shader_option_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            const u32 index = r_material->GetStaticShaderOptionIndex(r_static_shader_option_paths[i]);

            PrintIndent(indent_level + 1);
            if (index == 0xffff) {
                PrintOnlySide(PrintSide::Right);
                std::cout << "(static shader option): " << r_static_shader_option_paths[i] << "(Invalid)" << std::endl;
            } else if (r_material->IsStaticShaderOptionBool(index) == true) {
                s32 bool_param = r_material->GetStaticShaderOptionBool(index);
                PrintOnlySide(PrintSide::Right);
                std::cout << "(static shader option): " << r_static_shader_option_paths[i] << "(Bool: " << bool_param << ")" << std::endl;
            } else {
                const char *string_param = r_material->GetStaticShaderOptionString(index) + 2;
                PrintOnlySide(PrintSide::Right);
                std::cout << "(static shader option): " << r_static_shader_option_paths[i] << "(String: \"" << string_param << "\")" << std::endl;
            }
        }
    }    

    /* Left only */
    for (u32 i = 0; i < l_render_info_iterator.GetFileCount(); ++i) {
        u32 r_index = r_render_info_iterator.FindPathIndex(l_render_info_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            std::cout << "Left only (render info): " << l_render_info_paths[i];
            PrintRenderInfoValue(l_material, l_render_info_paths[i], PrintSide::None);
            std::cout << std::endl;
        }
    }

    for (u32 i = 0; i < l_shader_param_iterator.GetFileCount(); ++i) {
        u32 r_index = r_shader_param_iterator.FindPathIndex(l_shader_param_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            std::cout << "Left only (shader param): " << l_shader_param_paths[i];
            PrintShaderParamValue(l_material, l_shader_param_paths[i], PrintSide::None);
            std::cout << std::endl;
        }
    }

    for (u32 i = 0; i < l_vertex_attribute_iterator.GetFileCount(); ++i) {
        u32 r_index = r_vertex_attribute_iterator.FindPathIndex(l_vertex_attribute_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            const char *l_attr = l_material->GetShaderVertexAttributeName(l_vertex_attribute_paths[i]);
            std::cout << "Left only (vertex attribute): " << l_vertex_attribute_paths[i] << "(shader: " << l_attr << ")" << std::endl;
        }
    }

    for (u32 i = 0; i < l_sampler_iterator.GetFileCount(); ++i) {
        u32 r_index = r_sampler_iterator.FindPathIndex(l_sampler_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            const char *l_attr = l_material->GetShaderSamplerName(l_sampler_paths[i]);
            std::cout << "Left only (sampler): " << l_sampler_paths[i] << "(shader: " << l_attr << ")"  << std::endl;
        }
    }
    
    for (u32 i = 0; i < l_static_shader_option_iterator.GetFileCount(); ++i) {
        u32 r_index = r_static_shader_option_iterator.FindPathIndex(l_static_shader_option_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            const u32 index = l_material->GetStaticShaderOptionIndex(l_static_shader_option_paths[i]);
            
            PrintIndent(indent_level + 1);
            if (index == 0xffff) {
                PrintOnlySide(PrintSide::Left);
                std::cout << "(static shader option): " << l_static_shader_option_paths[i] << "(Invalid)" << std::endl;
            } else if (l_material->IsStaticShaderOptionBool(index) == true) {
                s32 bool_param = l_material->GetStaticShaderOptionBool(index);
                PrintOnlySide(PrintSide::Left);
                std::cout << "(static shader option): " << l_static_shader_option_paths[i] << "(Bool: " << bool_param << ")" << std::endl;
            } else {
                const char *string_param = l_material->GetStaticShaderOptionString(index) + 2;
                PrintOnlySide(PrintSide::Left);
                std::cout << "(static shader option): " << l_static_shader_option_paths[i] << "(String: \"" << string_param << "\")" << std::endl;
            }
        }
    }

    /* Cleanup */
    l_render_info_iterator.Finalize();
    l_shader_param_iterator.Finalize();
    l_vertex_attribute_iterator.Finalize();
    l_sampler_iterator.Finalize();
    l_static_shader_option_iterator.Finalize();

    r_render_info_iterator.Finalize();
    r_shader_param_iterator.Finalize();
    r_vertex_attribute_iterator.Finalize();
    r_sampler_iterator.Finalize();
    r_static_shader_option_iterator.Finalize();

    return true;
}

void ProcessResMaterialSingle(dd::res::ResBfresMaterial *material, u32 indent_level, PrintSide print_side) {

    /* parser */
    RomfsDirectoryParser render_info_iterator = {};
    render_info_iterator.InitializeByResDic(material->material_shader_data->shader_reflection->render_info_dictionary);
    RomfsDirectoryParser shader_param_iterator = {};
    shader_param_iterator.InitializeByResDic(material->material_shader_data->shader_reflection->shader_param_dictionary);
    RomfsDirectoryParser vertex_attribute_iterator = {};
    vertex_attribute_iterator.InitializeByArrayNW(material->material_shader_data->vertex_attribute_name_array, material->material_shader_data->vertex_attribute_count);
    RomfsDirectoryParser sampler_iterator = {};
    sampler_iterator.InitializeByArrayNW(material->material_shader_data->sampler_name_array, material->material_shader_data->sampler_count);
    RomfsDirectoryParser static_shader_option_iterator = {};
    static_shader_option_iterator.InitializeByResDic(material->material_shader_data->shader_reflection->static_shader_option_dictionary);


    PrintIndent(indent_level + 1);
    PrintSideSpace(print_side);
    std::cout << "(shader archive): " << (material->material_shader_data->shader_reflection->shader_archive_name + 2) << std::endl;

    PrintIndent(indent_level + 1);
    PrintSideSpace(print_side);
    std::cout << "(shader name): " << (material->material_shader_data->shader_reflection->shader_name + 2) << std::endl;

    /* Render info */
    char **render_info_paths = render_info_iterator.GetFilePathArray();
    for (u32 i = 0; i < render_info_iterator.GetFileCount(); ++i) {
        if (material->render_info_value_count_array[material->material_shader_data->shader_reflection->render_info_dictionary->FindEntryIndex(render_info_paths[i])] == 0) { continue; }

        PrintIndent(indent_level + 1);
        PrintSideSpace(print_side);
        std::cout << "(render info): " << render_info_paths[i];
        PrintRenderInfoValue(material, render_info_paths[i], PrintSide::None);
        std::cout << std::endl;
    }

    /* shader param */
    char **shader_param_paths = shader_param_iterator.GetFilePathArray();
    for (u32 i = 0; i < shader_param_iterator.GetFileCount(); ++i) {
        PrintIndent(indent_level + 1);
        PrintSideSpace(print_side);
        std::cout << "(shader param): " << shader_param_paths[i];
        PrintShaderParamValue(material, shader_param_paths[i], PrintSide::None);
        std::cout << std::endl;
    }

    /* vertex attribute */
    char **vertex_attribute_paths = vertex_attribute_iterator.GetFilePathArray();
    for (u32 i = 0; i < vertex_attribute_iterator.GetFileCount(); ++i) {
        const char *v_idx = material->GetShaderVertexAttributeName(i);
        PrintIndent(indent_level + 1);
        PrintSideSpace(print_side);
        std::cout << "(vertex attribute): " << vertex_attribute_paths[i] << "(shader: "  << v_idx << ")" << std::endl;
    }

    /* sampler */
    char **sampler_paths = sampler_iterator.GetFilePathArray();
    for (u32 i = 0; i < sampler_iterator.GetFileCount(); ++i) {
        const char *m_idx = material->GetShaderSamplerName(i);
        PrintIndent(indent_level + 1);
        PrintSideSpace(print_side);
        std::cout << "(sampler): " << sampler_paths[i] << "(shader: " << m_idx << ")" << std::endl;
    }

    /* static shader option */
    char **static_shader_option_paths = static_shader_option_iterator.GetFilePathArray();
    for (u32 i = 0; i < static_shader_option_iterator.GetFileCount(); ++i) {
        const u32 index = material->GetStaticShaderOptionIndex(static_shader_option_paths[i]);
        if (index == 0xffff) { continue; }
        
        PrintIndent(indent_level + 1);
        if (material->IsStaticShaderOptionBool(index) == true) {
            s32 bool_param = material->GetStaticShaderOptionBool(index);
            PrintSideSpace(print_side);
            std::cout << "(static shader option): " << static_shader_option_paths[i] << "(Bool: " << bool_param << ")" << std::endl;
        } else {
            const char *string_param = material->GetStaticShaderOptionString(index) + 2;
            PrintSideSpace(print_side);
            std::cout << "(static shader option): " << static_shader_option_paths[i] << "(String: \"" << string_param << "\")" << std::endl;
        }
    }

    /* cleanup */
    render_info_iterator.Finalize();
    shader_param_iterator.Finalize();
    vertex_attribute_iterator.Finalize();
    sampler_iterator.Finalize();
    static_shader_option_iterator.Finalize();
}
