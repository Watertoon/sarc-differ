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

bool DiffGfxEmbedFile(dd::res::ResGfxEmbedFile *l_embed_file, dd::res::ResGfxEmbedFile *r_embed_file, const char *path, u32 indent_level, bool is_print) {

	if (is_print == false) {
        return (l_embed_file->file_size == r_embed_file->file_size) && ::memcmp(l_embed_file->file_offset, r_embed_file->file_offset, r_embed_file->file_size);
    }
    ProcessFilesImpl(l_embed_file->file_offset, l_embed_file->file_size, r_embed_file->file_offset, r_embed_file->file_size, path, indent_level);
    return false;
}

void ProcessGfxUserDataSingle(dd::res::ResGfxUserData *data_array, dd::res::ResNintendoWareDictionary *data_dictionary, u32 indent_level, PrintSide print_side) {

    RomfsDirectoryParser data_iterator = {};
    const bool result = data_iterator.InitializeByResDic(data_dictionary);

    if (result == false) { return; }

    char **data_paths = data_iterator.GetFilePathArray();
    for (u32 i = 0; i < data_iterator.GetFileCount(); ++i) {
        const u32 entry_index = data_dictionary->FindEntryIndex(data_paths[i]);
        
        PrintIndent(indent_level);
        PrintOnlySide(print_side);
        std::cout << "(user data): " << (data_array[entry_index].user_data_name + 2);

        switch (static_cast<dd::res::GfxUserDataType>(data_array[entry_index].data_type)) {
            case dd::res::GfxUserDataType::S32:
            {
                const s32 *user_data = reinterpret_cast<s32*>(data_array[entry_index].user_data);
                std::cout << "(S32: ";

                if (data_array[entry_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (data_array[entry_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(data_array[entry_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::Float:
            {
                const float *user_data = reinterpret_cast<float*>(data_array[entry_index].user_data);
                std::cout << "(Float: ";

                if (data_array[entry_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (data_array[entry_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(data_array[entry_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::String:
            {
                const char **user_data = reinterpret_cast<const char**>(data_array[entry_index].user_data);
                std::cout << "(String: ";

                if (data_array[entry_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (data_array[entry_index].data_entries - 1); ++i) {
                    std::cout << "\"" << (user_data[i] + 2) << "\", ";
                }
                std::cout << "\"" << (user_data[(data_array[entry_index].data_entries - 1)] + 2) << "\")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::Byte:
            {
                const u8 *user_data = reinterpret_cast<u8*>(data_array[entry_index].user_data);
                std::cout << "(U8: ";

                if (data_array[entry_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (data_array[entry_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(data_array[entry_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            default:
                std::cout << "(Invalid)" << std::endl;
                break;
        }
    }
}

bool LeftOnlyGfxUserData(dd::res::ResGfxUserData *l_data_array, dd::res::ResNintendoWareDictionary *l_data_dictionary, [[maybe_unused]] dd::res::ResGfxUserData *r_data_array, dd::res::ResNintendoWareDictionary *r_data_dictionary, u32 indent_level, bool is_print) {

    RomfsDirectoryParser l_data_iterator = {};
    const bool l_result = l_data_iterator.InitializeByResDic(l_data_dictionary);

    RomfsDirectoryParser r_data_iterator = {};
    const bool r_result = r_data_iterator.InitializeByResDic(r_data_dictionary);

    /* Handle single or no user data */
    if (l_result == false && r_result == false) {
        return true;
    } else if (l_result == true && r_result == false) {
        if (is_print == false) { return false; }
        ProcessGfxUserDataSingle(l_data_array, l_data_dictionary, indent_level, PrintSide::Left);
        return false;
    } else if (l_result == false && r_result == true) {
        if (is_print == false) { return false; }
        return false;
    }

    char **l_data_paths = l_data_iterator.GetFilePathArray();

    for (u32 i = 0; i < l_data_iterator.GetFileCount(); ++i) {
        const u32 r_path_index = r_data_iterator.FindPathIndex(l_data_paths[i]);
        if (r_path_index != RomfsDirectoryParser::InvalidIndex) { continue; }

        if (is_print == false) { return false; }

        const u32 l_index = l_data_dictionary->FindEntryIndex(l_data_paths[i]);

        PrintIndent(indent_level);
        PrintOnlySide(PrintSide::Left);
        std::cout << "(user data): " << (l_data_array[l_index].user_data_name + 2);

        switch (static_cast<dd::res::GfxUserDataType>(l_data_array[l_index].data_type)) {
            case dd::res::GfxUserDataType::S32:
            {
                const s32 *user_data = reinterpret_cast<s32*>(l_data_array[l_index].user_data);
                std::cout << "(S32: ";

                if (l_data_array[l_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (l_data_array[l_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(l_data_array[l_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::Float:
            {
                const float *user_data = reinterpret_cast<float*>(l_data_array[l_index].user_data);
                std::cout << "(Float: ";

                if (l_data_array[l_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (l_data_array[l_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(l_data_array[l_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::String:
            {
                const char **user_data = reinterpret_cast<const char**>(l_data_array[l_index].user_data);
                std::cout << "(String: ";

                if (l_data_array[l_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (l_data_array[l_index].data_entries - 1); ++i) {
                    std::cout << "\"" << (user_data[i] + 2) << "\", ";
                }
                std::cout << "\"" << (user_data[(l_data_array[l_index].data_entries - 1)] + 2) << "\")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::Byte:
            {
                const u8 *user_data = reinterpret_cast<u8*>(l_data_array[l_index].user_data);
                std::cout << "(U8: ";

                if (l_data_array[l_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (l_data_array[l_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(l_data_array[l_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            default:
                std::cout << "(Invalid)" << std::endl;
                break;
        }
    }

    return true;
}

bool RightOnlyGfxUserData([[maybe_unused]] dd::res::ResGfxUserData *l_data_array, dd::res::ResNintendoWareDictionary *l_data_dictionary, dd::res::ResGfxUserData *r_data_array, dd::res::ResNintendoWareDictionary *r_data_dictionary, u32 indent_level, bool is_print) {

    RomfsDirectoryParser l_data_iterator = {};
    const bool l_result = l_data_iterator.InitializeByResDic(l_data_dictionary);

    RomfsDirectoryParser r_data_iterator = {};
    const bool r_result = r_data_iterator.InitializeByResDic(r_data_dictionary);

    /* Handle single or no user data */
    if (l_result == false && r_result == false) {
        return true;
    } else if (l_result == true && r_result == false) {
        if (is_print == false) { return false; }
        return false;
    } else if (l_result == false && r_result == true) {
        if (is_print == false) { return false; }
        ProcessGfxUserDataSingle(l_data_array, l_data_dictionary, indent_level, PrintSide::Right);
        return false;
    }

    char **r_data_paths = r_data_iterator.GetFilePathArray();

    for (u32 i = 0; i < r_data_iterator.GetFileCount(); ++i) {
        const u32 l_path_index = l_data_iterator.FindPathIndex(r_data_paths[i]);
        if (l_path_index != RomfsDirectoryParser::InvalidIndex) { continue; }

        if (is_print == false) { return false; }

        const u32 r_index = r_data_dictionary->FindEntryIndex(r_data_paths[i]);

        PrintIndent(indent_level);
        PrintOnlySide(PrintSide::Right);
        std::cout << "(user data): " << (r_data_array[r_index].user_data_name + 2);

        switch (static_cast<dd::res::GfxUserDataType>(r_data_array[r_index].data_type)) {
            case dd::res::GfxUserDataType::S32:
            {
                const s32 *user_data = reinterpret_cast<s32*>(r_data_array[r_index].user_data);
                std::cout << "(S32: ";

                if (r_data_array[r_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (r_data_array[r_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(r_data_array[r_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::Float:
            {
                const float *user_data = reinterpret_cast<float*>(r_data_array[r_index].user_data);
                std::cout << "(Float: ";

                if (r_data_array[r_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (r_data_array[r_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(r_data_array[r_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::String:
            {
                const char **user_data = reinterpret_cast<const char**>(r_data_array[r_index].user_data);
                std::cout << "(String: ";

                if (r_data_array[r_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (r_data_array[r_index].data_entries - 1); ++i) {
                    std::cout << "\"" << (user_data[i] + 2) << "\", ";
                }
                std::cout << "\"" << (user_data[(r_data_array[r_index].data_entries - 1)] + 2) << "\")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::Byte:
            {
                const u8 *user_data = reinterpret_cast<u8*>(r_data_array[r_index].user_data);
                std::cout << "(U8: ";

                if (r_data_array[r_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (r_data_array[r_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(r_data_array[r_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            default:
                std::cout << "(Invalid)" << std::endl;
                break;
        }
    }

    return true;
}

bool DiffGfxUserData(dd::res::ResGfxUserData *l_data_array, dd::res::ResNintendoWareDictionary *l_data_dictionary, dd::res::ResGfxUserData *r_data_array, dd::res::ResNintendoWareDictionary *r_data_dictionary, u32 indent_level, bool is_print) {

    RomfsDirectoryParser l_data_iterator = {};
    const bool l_result = l_data_iterator.InitializeByResDic(l_data_dictionary);

    RomfsDirectoryParser r_data_iterator = {};
    const bool r_result = r_data_iterator.InitializeByResDic(r_data_dictionary);

    /* Handle single or no user data */
    if (l_result == false && r_result == false) {
        return true;
    } else if (l_result == true && r_result == false) {
        if (is_print == false) { return false; }
        return false;
    } else if (l_result == false && r_result == true) {
        if (is_print == false) { return false; }
        return false;
    }

    char **l_data_paths = l_data_iterator.GetFilePathArray();
    
    for (u32 i = 0; i < l_data_iterator.GetFileCount(); ++i) {
        const u32 r_path_index = r_data_iterator.FindPathIndex(l_data_paths[i]);
        if (r_path_index == RomfsDirectoryParser::InvalidIndex) { if (is_print == false) { return false; } continue; }

        const u32 l_index = l_data_dictionary->FindEntryIndex(l_data_paths[i]);
        const u32 r_index = r_data_dictionary->FindEntryIndex(l_data_paths[i]);

        /* Comparison switch */
        if (l_data_array[l_index].data_type == r_data_array[r_index].data_type && l_data_array[l_index].data_entries == r_data_array[r_index].data_entries) {
            switch (static_cast<dd::res::GfxUserDataType>(l_data_array[l_index].data_type)) {
                case dd::res::GfxUserDataType::S32:
                {
                    const s32 *l_user_data = reinterpret_cast<s32*>(l_data_array[l_index].user_data);
                    const s32 *r_user_data = reinterpret_cast<s32*>(r_data_array[r_index].user_data);
                    
                    for (s32 i = 0; i < l_data_array[l_index].data_entries; ++i) {
                        if (l_user_data[i] != r_user_data[i]) { break; }
                    }
                    continue;
                }
                case dd::res::GfxUserDataType::Float:
                {
                    const float *l_user_data = reinterpret_cast<float*>(l_data_array[l_index].user_data);
                    const float *r_user_data = reinterpret_cast<float*>(r_data_array[r_index].user_data);
                    
                    for (s32 i = 0; i < l_data_array[l_index].data_entries; ++i) {
                        if (l_user_data[i] != r_user_data[i]) { break; }
                    }
                    continue;
                }
                case dd::res::GfxUserDataType::String:
                {
                    const char **l_user_data = reinterpret_cast<const char**>(l_data_array[l_index].user_data);
                    const char **r_user_data = reinterpret_cast<const char**>(r_data_array[r_index].user_data);
                    
                    for (s32 i = 0; i < l_data_array[l_index].data_entries; ++i) {
                        if (::strcmp(l_user_data[i] + 2, r_user_data[i] + 2) != 0) { break; }
                    }
                    continue;
                }
                case dd::res::GfxUserDataType::Byte:
                {
                    const u8 *l_user_data = reinterpret_cast<u8*>(l_data_array[l_index].user_data);
                    const u8 *r_user_data = reinterpret_cast<u8*>(r_data_array[r_index].user_data);
                    
                    for (s32 i = 0; i < l_data_array[l_index].data_entries; ++i) {
                        if (l_user_data[i] != r_user_data[i]) { break; }
                    }
                    continue;
                }
            }
        }

        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "Different(user data): " << (l_data_array[l_index].user_data_name + 2);

        /* Print left */
        switch (static_cast<dd::res::GfxUserDataType>(l_data_array[l_index].data_type)) {
            case dd::res::GfxUserDataType::S32:
            {
                const s32 *user_data = reinterpret_cast<s32*>(l_data_array[l_index].user_data);
                std::cout << "(left S32: ";

                if (l_data_array[l_index].data_entries < 1) { std::cout << "Invalid)"; continue; }

                for (s32 i = 0; i < (l_data_array[l_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(l_data_array[l_index].data_entries - 1)] << ")";
                break;
            }
            case dd::res::GfxUserDataType::Float:
            {
                const float *user_data = reinterpret_cast<float*>(l_data_array[l_index].user_data);
                std::cout << "(left Float: ";

                if (l_data_array[l_index].data_entries < 1) { std::cout << "Invalid)"; continue; }

                for (s32 i = 0; i < (l_data_array[l_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(l_data_array[l_index].data_entries - 1)] << ")";
                break;
            }
            case dd::res::GfxUserDataType::String:
            {
                const char **user_data = reinterpret_cast<const char**>(l_data_array[l_index].user_data);
                std::cout << "(left String: ";

                if (l_data_array[l_index].data_entries < 1) { std::cout << "Invalid)"; continue; }

                for (s32 i = 0; i < (l_data_array[l_index].data_entries - 1); ++i) {
                    std::cout << "\"" << (user_data[i] + 2) << "\", ";
                }
                std::cout << "\"" << (user_data[(l_data_array[l_index].data_entries - 1)] + 2) << "\")";
                break;
            }
            case dd::res::GfxUserDataType::Byte:
            {
                const u8 *user_data = reinterpret_cast<u8*>(l_data_array[l_index].user_data);
                std::cout << "(left U8: ";

                if (l_data_array[l_index].data_entries < 1) { std::cout << "Invalid)"; continue; }

                for (s32 i = 0; i < (l_data_array[l_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(l_data_array[l_index].data_entries - 1)] << ")";
                break;
            }
            default:
                std::cout << "(left Invalid)";
                break;
        }

        /* Print right */
        switch (static_cast<dd::res::GfxUserDataType>(r_data_array[r_index].data_type)) {
            case dd::res::GfxUserDataType::S32:
            {
                const s32 *user_data = reinterpret_cast<s32*>(r_data_array[r_index].user_data);
                std::cout << "(right S32: ";

                if (r_data_array[r_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (r_data_array[r_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(r_data_array[r_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::Float:
            {
                const float *user_data = reinterpret_cast<float*>(r_data_array[r_index].user_data);
                std::cout << "(right Float: ";

                if (r_data_array[r_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (r_data_array[r_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(r_data_array[r_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::String:
            {
                const char **user_data = reinterpret_cast<const char**>(r_data_array[r_index].user_data);
                std::cout << "(right String: ";

                if (r_data_array[r_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (r_data_array[r_index].data_entries - 1); ++i) {
                    std::cout << "\"" << (user_data[i] + 2) << "\", ";
                }
                std::cout << "\"" << (user_data[(r_data_array[r_index].data_entries - 1)] + 2) << "\")" << std::endl;
                break;
            }
            case dd::res::GfxUserDataType::Byte:
            {
                const u8 *user_data = reinterpret_cast<u8*>(r_data_array[r_index].user_data);
                std::cout << "(right U8: ";

                if (r_data_array[r_index].data_entries < 1) { std::cout << "Invalid)" << std::endl; continue; }

                for (s32 i = 0; i < (r_data_array[r_index].data_entries - 1); ++i) {
                    std::cout << user_data[i] << ", ";
                }
                std::cout << user_data[(r_data_array[r_index].data_entries - 1)] << ")" << std::endl;
                break;
            }
            default:
                std::cout << "(right Invalid)" << std::endl;
                break;
        }
    }

    return true;
}

bool DiffGfxSamplerInfo(dd::res::ResGfxSamplerInfo *l_sampler_info, dd::res::ResGfxSamplerInfo *r_sampler_info, u32 indent_level, bool is_print) {

    if (l_sampler_info->wrap_mode_u != r_sampler_info->wrap_mode_u) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left WrapModeU: " << dd::res::GfxWrapModeToString(static_cast<dd::res::GfxWrapMode>(l_sampler_info->wrap_mode_u)) << ")";
        std::cout << "(right WrapModeU: " << dd::res::GfxWrapModeToString(static_cast<dd::res::GfxWrapMode>(r_sampler_info->wrap_mode_u)) << ")" << std::endl;
    }
    if (l_sampler_info->wrap_mode_v != r_sampler_info->wrap_mode_v) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left WrapModeV: " << dd::res::GfxWrapModeToString(static_cast<dd::res::GfxWrapMode>(l_sampler_info->wrap_mode_v)) << ")";
        std::cout << "(right WrapModeV: " << dd::res::GfxWrapModeToString(static_cast<dd::res::GfxWrapMode>(r_sampler_info->wrap_mode_v)) << ")" << std::endl;
    }
    if (l_sampler_info->wrap_mode_w != r_sampler_info->wrap_mode_w) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left WrapModeW: " << dd::res::GfxWrapModeToString(static_cast<dd::res::GfxWrapMode>(l_sampler_info->wrap_mode_w)) << ")";
        std::cout << "(right WrapModeW: " << dd::res::GfxWrapModeToString(static_cast<dd::res::GfxWrapMode>(r_sampler_info->wrap_mode_w)) << ")" << std::endl;
    }
    if (l_sampler_info->enable_compare_op != r_sampler_info->enable_compare_op) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left CompareMode: " << static_cast<bool>(l_sampler_info->enable_compare_op) << ")";
        std::cout << "(right CompareMode: " << static_cast<bool>(r_sampler_info->enable_compare_op) << ")" << std::endl;
    }
    if (l_sampler_info->compare_op != r_sampler_info->compare_op) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left CompareOp: " << dd::res::GfxCompareOpToString(static_cast<dd::res::GfxCompareOp>(l_sampler_info->compare_op)) << ")";
        std::cout << "(right CompareOp: " << dd::res::GfxCompareOpToString(static_cast<dd::res::GfxCompareOp>(r_sampler_info->compare_op)) << ")" << std::endl;
    }
    if (l_sampler_info->border_color != r_sampler_info->border_color) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left BorderColor: " << dd::res::GfxBorderColorToString(static_cast<dd::res::GfxBorderColor>(l_sampler_info->border_color)) << ")";
        std::cout << "(right BordorColor: " << dd::res::GfxBorderColorToString(static_cast<dd::res::GfxBorderColor>(r_sampler_info->border_color)) << ")" << std::endl;
    }
    if (l_sampler_info->enable_anisotropy != r_sampler_info->enable_anisotropy) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left AnisotropyEnable: " << static_cast<bool>(l_sampler_info->enable_anisotropy) << ")";
        std::cout << "(right AnisotropyEnable: " << static_cast<bool>(r_sampler_info->enable_anisotropy) << ")" << std::endl;
    }
    if (l_sampler_info->max_anisotropy != r_sampler_info->max_anisotropy) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left MaxAnisotropy: " << l_sampler_info->max_anisotropy << ")";
        std::cout << "(right MaxAnisotropy: " << r_sampler_info->max_anisotropy << ")" << std::endl;
    }
    if (l_sampler_info->mip_map_filter != r_sampler_info->mip_map_filter) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left MipMapFilter: " << dd::res::GfxMipMapFilterToString(static_cast<dd::res::GfxMipMapFilter>(l_sampler_info->mip_map_filter)) << ")";
        std::cout << "(right MipMapFilter: " << dd::res::GfxMipMapFilterToString(static_cast<dd::res::GfxMipMapFilter>(r_sampler_info->mip_map_filter)) << ")" << std::endl;
    }
    if (l_sampler_info->mag_filter != r_sampler_info->mag_filter) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left MagFilter: " << dd::res::GfxMagFilterToString(static_cast<dd::res::GfxMagFilter>(l_sampler_info->mag_filter)) << ")";
        std::cout << "(right MagFilter: " << dd::res::GfxMagFilterToString(static_cast<dd::res::GfxMagFilter>(r_sampler_info->mag_filter)) << ")" << std::endl;
    }
    if (l_sampler_info->min_filter != r_sampler_info->min_filter) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left MinFilter: " << dd::res::GfxMinFilterToString(static_cast<dd::res::GfxMinFilter>(l_sampler_info->min_filter)) << ")";
        std::cout << "(right MinFilter: " << dd::res::GfxMinFilterToString(static_cast<dd::res::GfxMinFilter>(r_sampler_info->min_filter)) << ")" << std::endl;
    }
    if (l_sampler_info->reduction_filter != r_sampler_info->reduction_filter) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left ReductionFilter: " << dd::res::GfxReductionFilterToString(static_cast<dd::res::GfxReductionFilter>(l_sampler_info->reduction_filter)) << ")";
        std::cout << "(right ReductionFilter: " << dd::res::GfxReductionFilterToString(static_cast<dd::res::GfxReductionFilter>(r_sampler_info->reduction_filter)) << ")" << std::endl;
    }
    if (l_sampler_info->lod_clamp_min != r_sampler_info->lod_clamp_min || l_sampler_info->lod_clamp_max != r_sampler_info->lod_clamp_max) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left LodClamp: " << l_sampler_info->lod_clamp_min << ", " << l_sampler_info->lod_clamp_max << ")";
        std::cout << "(right LodClamp: " << r_sampler_info->lod_clamp_min << ", " << r_sampler_info->lod_clamp_max << ")" << std::endl;
    }
    if (l_sampler_info->lod_bias != r_sampler_info->lod_bias) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "(left LodBias: " << l_sampler_info->lod_bias << ")";
        std::cout << "(right LodBias: " << r_sampler_info->lod_bias << ")" << std::endl;
    }

    return true;
}

void ProcessGfxSamplerInfoSingle(dd::res::ResGfxSamplerInfo *sampler_info, u32 indent_level, PrintSide print_side) {

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "WrapModeU: " << dd::res::GfxWrapModeToString(static_cast<dd::res::GfxWrapMode>(sampler_info->wrap_mode_u)) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "WrapModeV: " << dd::res::GfxWrapModeToString(static_cast<dd::res::GfxWrapMode>(sampler_info->wrap_mode_v)) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "WrapModeW: " << dd::res::GfxWrapModeToString(static_cast<dd::res::GfxWrapMode>(sampler_info->wrap_mode_w)) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "CompareMode: " << static_cast<bool>(sampler_info->enable_compare_op) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "CompareOp: " << dd::res::GfxCompareOpToString(static_cast<dd::res::GfxCompareOp>(sampler_info->compare_op)) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "BordorColor: " << dd::res::GfxBorderColorToString(static_cast<dd::res::GfxBorderColor>(sampler_info->border_color)) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "AnisotropyEnable: " << static_cast<bool>(sampler_info->enable_anisotropy) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "MaxAnisotropy: " << static_cast<u32>(sampler_info->max_anisotropy) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "MipMapFilter: " << dd::res::GfxMipMapFilterToString(static_cast<dd::res::GfxMipMapFilter>(sampler_info->mip_map_filter)) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "MagFilter: " << dd::res::GfxMagFilterToString(static_cast<dd::res::GfxMagFilter>(sampler_info->mag_filter)) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "MinFilter: " << dd::res::GfxMinFilterToString(static_cast<dd::res::GfxMinFilter>(sampler_info->min_filter)) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "ReductionFilter: " << dd::res::GfxReductionFilterToString(static_cast<dd::res::GfxReductionFilter>(sampler_info->reduction_filter)) << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "LodClamp: " << sampler_info->lod_clamp_min << ", " << sampler_info->lod_clamp_max << ")" << std::endl;

        PrintIndent(indent_level);
        std::cout << "(";
        PrintSideSpace(print_side);
        std::cout << "LodBias: " << sampler_info->lod_bias << ")" << std::endl;
}
