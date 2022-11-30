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

bool sPrintMaterial = false;

void ProcessResModelSingle(dd::res::ResBfresModel *model, u32 indent_level, bool is_right) {

    PrintIndent(indent_level);
    if (is_right) {
        std::cout << "Right only: " << (model->model_name + 2) << ".fmdl" << std::endl;
    } else {
        std::cout << "Left only : " << (model->model_name + 2) << ".fmdl" << std::endl;
    }

    RomfsDirectoryParser shape_iterator = {};
    shape_iterator.InitializeByResDic(model->shape_dictionary);
    RomfsDirectoryParser material_iterator = {};
    material_iterator.InitializeByResDic(model->material_dictionary);

    char **shape_paths = shape_iterator.GetFilePathArray();
    for (u32 i = 0; i < shape_iterator.GetFileCount(); ++i) {
        PrintIndent(indent_level + 1);
        if (is_right) {
            std::cout << "Right only: " << shape_paths[i] << ".fshp" << std::endl;
        } else {
            std::cout << "Left only : " << shape_paths[i] << ".fshp" << std::endl;
        }
    }

    char **material_paths = material_iterator.GetFilePathArray();
    for (u32 i = 0; i < material_iterator.GetFileCount(); ++i) {
        PrintIndent(indent_level + 1);
        if (is_right) {
            std::cout << "Right only: " << material_paths[i] << ".fmat" << std::endl;
        } else {
            std::cout << "Left only : " << material_paths[i] << ".fmat" << std::endl;
        }
        //dd::res::ResBfresMaterial *material = model->material_array + model->material_dictionary->FindEntryIndex(material_paths[i]);
        //ProcessResMaterialSingle(material, indent_level + 1, is_right);
    }

    /* cleanup */
    shape_iterator.Finalize();
    material_iterator.Finalize();
}

void ProcessResSkeletalAnimSingle(dd::res::ResBfresSkeletalAnim *skeletal_anim, u32 indent_level, bool is_right) {
    PrintIndent(indent_level);
    if (is_right) {
        std::cout << "Right only: " << (skeletal_anim->animation_name + 2) << ".fska" << std::endl;
    } else {
        std::cout << "Left only : " << (skeletal_anim->animation_name + 2) << ".fska" << std::endl;
    }
}

void ProcessResMaterialAnimSingle(dd::res::ResBfresMaterialAnim *material_anim, u32 indent_level, bool is_right) {
    PrintIndent(indent_level);
    if (is_right) {
        std::cout << "Right only: " << (material_anim->animation_name + 2) << ".fmaa" << std::endl;
    } else {
        std::cout << "Left only : " << (material_anim->animation_name + 2) << ".fmaa" << std::endl;
    }
}

void ProcessResBoneVisibilityAnimSingle(dd::res::ResBfresBoneVisibilityAnim *bone_visibility_anim, u32 indent_level, bool is_right) {
    PrintIndent(indent_level);
    if (is_right) {
        std::cout << "Right only: " << (bone_visibility_anim->animation_name + 2) << ".fbvs" << std::endl;
    } else {
        std::cout << "Left only : " << (bone_visibility_anim->animation_name + 2) << ".fbvs" << std::endl;
    }
}

void ProcessResShapeAnimSingle(dd::res::ResBfresShapeAnim *shape_anim, u32 indent_level, bool is_right) {
    PrintIndent(indent_level);
    if (is_right) {
        std::cout << "Right only: " << (shape_anim->animation_name + 2) << ".fsha" << std::endl;
    } else {
        std::cout << "Left only : " << (shape_anim->animation_name + 2) << ".fsha" << std::endl;
    }
}

void ProcessResSceneAnimSingle(dd::res::ResBfresSceneAnim *scene_anim, u32 indent_level, bool is_right) {
    PrintIndent(indent_level);
    if (is_right) {
        std::cout << "Right only: " << (scene_anim->animation_name + 2) << ".fscn" << std::endl;
    } else {
        std::cout << "Left only : " << (scene_anim->animation_name + 2) << ".fscn" << std::endl;
    }

    RomfsDirectoryParser camera_anim_iterator = {};
    camera_anim_iterator.InitializeByResDic(scene_anim->camera_anim_dictionary);
    RomfsDirectoryParser light_anim_iterator = {};
    light_anim_iterator.InitializeByResDic(scene_anim->light_anim_dictionary);
    RomfsDirectoryParser fog_anim_iterator = {};
    fog_anim_iterator.InitializeByResDic(scene_anim->fog_anim_dictionary);

    char **camera_anim_paths = camera_anim_iterator.GetFilePathArray();
    for (u32 y = 0; y < camera_anim_iterator.GetFileCount(); ++y) {
        PrintIndent(indent_level + 1);
        if (is_right) {
            std::cout << "Right only: " << camera_anim_paths[y] << ".fcam" << std::endl;
        } else {
            std::cout << "Left only : " << camera_anim_paths[y] << ".fcam" << std::endl;
        }
    }
    char **light_anim_paths = light_anim_iterator.GetFilePathArray();
    for (u32 y = 0; y < light_anim_iterator.GetFileCount(); ++y) {
        PrintIndent(indent_level + 1);
        if (is_right) {
            std::cout << "Right only: " << light_anim_paths[y] << ".flit" << std::endl;
        } else {
            std::cout << "Left only : " << light_anim_paths[y] << ".flit" << std::endl;
        }
    }
    char **fog_anim_paths = fog_anim_iterator.GetFilePathArray();
    for (u32 y = 0; y < fog_anim_iterator.GetFileCount(); ++y) {
        PrintIndent(indent_level + 1);
        if (is_right) {
            std::cout << "Right only: " << fog_anim_paths[y] << ".ffog" << std::endl;
        } else {
            std::cout << "Left only : " << fog_anim_paths[y] << ".ffog" << std::endl;
        }
    }

    camera_anim_iterator.Finalize();
    light_anim_iterator.Finalize();
    fog_anim_iterator.Finalize();
}

void DiffGfxUserData();

void DiffResMaterial(dd::res::ResBfresMaterial *l_material, dd::res::ResBfresMaterial *r_material, [[maybe_unused]] u32 indent_level) {

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
        if (r_index != RomfsDirectoryParser::InvalidIndex) {
            
        }
    }
    
    char **l_shader_param_paths = l_shader_param_iterator.GetFilePathArray();
    char **r_shader_param_paths = r_shader_param_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_shader_param_iterator.GetFileCount(); ++i) {
        u32 r_index = r_shader_param_iterator.FindPathIndex(l_shader_param_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {
            
        }
    }
    
    char **l_vertex_attribute_paths = l_vertex_attribute_iterator.GetFilePathArray();
    char **r_vertex_attribute_paths = r_vertex_attribute_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_vertex_attribute_iterator.GetFileCount(); ++i) {
        u32 r_index = r_vertex_attribute_iterator.FindPathIndex(l_vertex_attribute_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {
            
        }
    }
    
    char **l_sampler_paths = l_sampler_iterator.GetFilePathArray();
    char **r_sampler_paths = r_sampler_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_sampler_iterator.GetFileCount(); ++i) {
        u32 r_index = r_sampler_iterator.FindPathIndex(l_sampler_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {
            
        }
    }
    
    char **l_static_shader_option_paths = l_static_shader_option_iterator.GetFilePathArray();
    char **r_static_shader_option_paths = r_static_shader_option_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_static_shader_option_iterator.GetFileCount(); ++i) {
        u32 r_index = r_static_shader_option_iterator.FindPathIndex(l_static_shader_option_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {
            
        }
    }
    
    /* Right only */
    for (u32 i = 0; i < r_render_info_iterator.GetFileCount(); ++i) {
        u32 l_index = l_render_info_iterator.FindPathIndex(r_render_info_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << r_render_info_paths[i] << std::endl;
        }
    }

    for (u32 i = 0; i < r_shader_param_iterator.GetFileCount(); ++i) {
        u32 l_index = l_shader_param_iterator.FindPathIndex(r_shader_param_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << r_shader_param_paths[i] << std::endl;
        }
    }

    for (u32 i = 0; i < r_vertex_attribute_iterator.GetFileCount(); ++i) {
        u32 l_index = l_vertex_attribute_iterator.FindPathIndex(r_vertex_attribute_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << r_vertex_attribute_paths[i] << std::endl;
        }
    }
    

    for (u32 i = 0; i < r_sampler_iterator.GetFileCount(); ++i) {
        u32 l_index = l_sampler_iterator.FindPathIndex(r_sampler_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << r_sampler_paths[i] << std::endl;
        }
    }
    
    for (u32 i = 0; i < r_static_shader_option_iterator.GetFileCount(); ++i) {
        u32 l_index = l_static_shader_option_iterator.FindPathIndex(r_static_shader_option_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << r_static_shader_option_paths[i] << std::endl;
        }
    }    

    /* Left only */
    for (u32 i = 0; i < l_render_info_iterator.GetFileCount(); ++i) {
        u32 r_index = r_render_info_iterator.FindPathIndex(l_render_info_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Left only : " << l_render_info_paths[i] << std::endl;
        }
    }

    for (u32 i = 0; i < l_shader_param_iterator.GetFileCount(); ++i) {
        u32 r_index = r_shader_param_iterator.FindPathIndex(l_shader_param_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Left only : " << l_shader_param_paths[i] << std::endl;
        }
    }

    for (u32 i = 0; i < l_vertex_attribute_iterator.GetFileCount(); ++i) {
        u32 r_index = r_vertex_attribute_iterator.FindPathIndex(l_vertex_attribute_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Left only : " << l_vertex_attribute_paths[i] << std::endl;
        }
    }
    

    for (u32 i = 0; i < l_sampler_iterator.GetFileCount(); ++i) {
        u32 r_index = r_sampler_iterator.FindPathIndex(l_sampler_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Left only : " << l_sampler_paths[i] << std::endl;
        }
    }
    
    for (u32 i = 0; i < l_static_shader_option_iterator.GetFileCount(); ++i) {
        u32 r_index = r_static_shader_option_iterator.FindPathIndex(l_static_shader_option_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Left only : " << l_static_shader_option_paths[i] << std::endl;
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
}

void DiffResModel(dd::res::ResBfresModel *l_model, dd::res::ResBfresModel *r_model, u32 indent_level) {

    RomfsDirectoryParser l_shape_iterator = {};
    l_shape_iterator.InitializeByResDic(l_model->shape_dictionary);
    RomfsDirectoryParser l_material_iterator = {};
    l_material_iterator.InitializeByResDic(l_model->material_dictionary);

    RomfsDirectoryParser r_shape_iterator = {};
    r_shape_iterator.InitializeByResDic(r_model->shape_dictionary);
    RomfsDirectoryParser r_material_iterator = {};
    r_material_iterator.InitializeByResDic(r_model->material_dictionary);

    /* Compare shapes */
    char **l_shape_paths = l_shape_iterator.GetFilePathArray();
    char **r_shape_paths = r_shape_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_shape_iterator.GetFileCount(); ++i) {
        u32 r_index = r_shape_iterator.FindPathIndex(l_shape_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {

        }
    }

    /* Compare materials */
    char **l_material_paths = l_material_iterator.GetFilePathArray();
    char **r_material_paths = r_material_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_material_iterator.GetFileCount(); ++i) {
        u32 r_index = r_material_iterator.FindPathIndex(l_material_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {

        }
    }

    /* Right only shapes */
    for (u32 i = 0; i < r_shape_iterator.GetFileCount(); ++i) {
        u32 l_index = l_shape_iterator.FindPathIndex(r_shape_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresShape *shp = r_model->shape_array + r_model->shape_dictionary->FindEntryIndex(r_shape_paths[i]);
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << (shp->shape_name + 2) << ".fshp" <<  std::endl;
        }
    }

    /* Right only materials */
    for (u32 i = 0; i < r_material_iterator.GetFileCount(); ++i) {
        u32 l_index = l_material_iterator.FindPathIndex(r_material_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresMaterial *mat = r_model->material_array + r_model->material_dictionary->FindEntryIndex(r_material_paths[i]);
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << (mat->material_name + 2) << ".fmat" <<  std::endl;
        }
    }

    /* Left only shapes */
    for (u32 i = 0; i < l_shape_iterator.GetFileCount(); ++i) {
        u32 r_index = r_shape_iterator.FindPathIndex(l_shape_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresShape *shp = l_model->shape_array + l_model->shape_dictionary->FindEntryIndex(l_shape_paths[i]);
            PrintIndent(indent_level + 1);
            std::cout << "Left only : " << (shp->shape_name + 2) << ".fshp" <<  std::endl;
        }
    }

    /* Left only materials */
    for (u32 i = 0; i < l_material_iterator.GetFileCount(); ++i) {
        u32 r_index = r_material_iterator.FindPathIndex(l_material_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresMaterial *mat = l_model->material_array + l_model->material_dictionary->FindEntryIndex(l_material_paths[i]);
            PrintIndent(indent_level + 1);
            std::cout << "Left only : " << (mat->material_name + 2) << ".fmat" <<  std::endl;
        }
    }

    /* cleanup */
    l_shape_iterator.Finalize();
    l_material_iterator.Finalize();

    r_shape_iterator.Finalize();
    r_material_iterator.Finalize();
}

void DiffBfres(void *l_file, void *r_file, [[maybe_unused]] u32 indent_level) {

    /* rescast */
    dd::res::ResBfres *l_bfres = dd::res::ResBfres::ResCast(l_file);
    dd::res::ResBfres *r_bfres = dd::res::ResBfres::ResCast(r_file);

    /* dictionary iterators */
    RomfsDirectoryParser l_model_iterator = {};
    l_model_iterator.InitializeByResDic(l_bfres->model_dictionary);
    RomfsDirectoryParser l_skeletal_anim_iterator = {};
    l_skeletal_anim_iterator.InitializeByResDic(l_bfres->skeletal_anim_dictionary);
    RomfsDirectoryParser l_material_anim_iterator = {};
    l_material_anim_iterator.InitializeByResDic(l_bfres->material_anim_dictionary);
    RomfsDirectoryParser l_bone_visibility_anim_iterator = {};
    l_bone_visibility_anim_iterator.InitializeByResDic(l_bfres->bone_visibility_anim_dictionary);
    RomfsDirectoryParser l_shape_anim_iterator = {};
    l_shape_anim_iterator.InitializeByResDic(l_bfres->shape_anim_dictionary);
    RomfsDirectoryParser l_scene_anim_iterator = {};
    l_scene_anim_iterator.InitializeByResDic(l_bfres->scene_anim_dictionary);
    RomfsDirectoryParser l_embed_file_iterator = {};
    l_embed_file_iterator.InitializeByResDic(l_bfres->embed_file_dictionary);

    RomfsDirectoryParser r_model_iterator = {};
    r_model_iterator.InitializeByResDic(r_bfres->model_dictionary);
    RomfsDirectoryParser r_skeletal_anim_iterator = {};
    r_skeletal_anim_iterator.InitializeByResDic(r_bfres->skeletal_anim_dictionary);
    RomfsDirectoryParser r_material_anim_iterator = {};
    r_material_anim_iterator.InitializeByResDic(r_bfres->material_anim_dictionary);
    RomfsDirectoryParser r_bone_visibility_anim_iterator = {};
    r_bone_visibility_anim_iterator.InitializeByResDic(r_bfres->bone_visibility_anim_dictionary);
    RomfsDirectoryParser r_shape_anim_iterator = {};
    r_shape_anim_iterator.InitializeByResDic(r_bfres->shape_anim_dictionary);
    RomfsDirectoryParser r_scene_anim_iterator = {};
    r_scene_anim_iterator.InitializeByResDic(r_bfres->scene_anim_dictionary);
    RomfsDirectoryParser r_embed_file_iterator = {};
    r_embed_file_iterator.InitializeByResDic(r_bfres->embed_file_dictionary);

    char **l_model_paths = l_model_iterator.GetFilePathArray();
    char **r_model_paths = r_model_iterator.GetFilePathArray();

    char **l_skeletal_anim_paths = l_skeletal_anim_iterator.GetFilePathArray();
    char **r_skeletal_anim_paths = r_skeletal_anim_iterator.GetFilePathArray();

    char **l_material_anim_paths = l_material_anim_iterator.GetFilePathArray();
    char **r_material_anim_paths = r_material_anim_iterator.GetFilePathArray();

    char **l_bone_visibility_anim_paths = l_bone_visibility_anim_iterator.GetFilePathArray();
    char **r_bone_visibility_anim_paths = r_bone_visibility_anim_iterator.GetFilePathArray();

    char **l_shape_anim_paths = l_shape_anim_iterator.GetFilePathArray();
    char **r_shape_anim_paths = r_shape_anim_iterator.GetFilePathArray();

    char **l_scene_anim_paths = l_scene_anim_iterator.GetFilePathArray();
    char **r_scene_anim_paths = r_scene_anim_iterator.GetFilePathArray();

    char **l_embed_file_paths = l_embed_file_iterator.GetFilePathArray();
    char **r_embed_file_paths = r_embed_file_iterator.GetFilePathArray();

    /* Diff embed files */
    for (u32 i = 0; i < r_embed_file_iterator.GetFileCount(); ++i) {
        u32 l_index = l_embed_file_iterator.FindPathIndex(r_embed_file_paths[i]);
        if (l_index != RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResGfxEmbedFile *l_embed_file = l_bfres->embed_file_array + l_bfres->embed_file_dictionary->FindEntryIndex(l_embed_file_paths[l_index]);
            dd::res::ResGfxEmbedFile *r_embed_file = r_bfres->embed_file_array + r_bfres->embed_file_dictionary->FindEntryIndex(r_embed_file_paths[i]);
            ProcessFilesImpl(l_embed_file->file_offset, l_embed_file->file_size, r_embed_file->file_offset, r_embed_file->file_size, r_embed_file_paths[i], indent_level + 1);
        }
    }

    /* Right only models */
    for (u32 i = 0; i < r_model_iterator.GetFileCount(); ++i) {
        u32 l_index = l_model_iterator.FindPathIndex(r_model_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresModel *model = r_bfres->model_array + r_bfres->model_dictionary->FindEntryIndex(r_model_paths[i]);
            ProcessResModelSingle(model, indent_level + 1, true);
        }
    }

    /* Right only skeletal anims */
    for (u32 i = 0; i < r_skeletal_anim_iterator.GetFileCount(); ++i) {
        u32 l_index = l_skeletal_anim_iterator.FindPathIndex(r_skeletal_anim_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresSkeletalAnim *skeletal_anim = r_bfres->skeletal_anim_array + r_bfres->skeletal_anim_dictionary->FindEntryIndex(r_skeletal_anim_paths[i]);
            ProcessResSkeletalAnimSingle(skeletal_anim, indent_level + 1, true);
        }
    }

    /* Right only material anims */
    for (u32 i = 0; i < r_material_anim_iterator.GetFileCount(); ++i) {
        u32 l_index = l_material_anim_iterator.FindPathIndex(r_material_anim_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresMaterialAnim *material_anim = r_bfres->material_anim_array + r_bfres->material_anim_dictionary->FindEntryIndex(r_material_anim_paths[i]);
            ProcessResMaterialAnimSingle(material_anim, indent_level + 1, true);
        }
    }

    /* Right only bone visibility anims */
    for (u32 i = 0; i < r_bone_visibility_anim_iterator.GetFileCount(); ++i) {
        u32 l_index = l_bone_visibility_anim_iterator.FindPathIndex(r_bone_visibility_anim_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresBoneVisibilityAnim *bone_visibility_anim = r_bfres->bone_visibility_anim_array + r_bfres->bone_visibility_anim_dictionary->FindEntryIndex(r_bone_visibility_anim_paths[i]);
            ProcessResBoneVisibilityAnimSingle(bone_visibility_anim, indent_level + 1, true);
        }
    }

    /* Right only shape anims */
    for (u32 i = 0; i < r_shape_anim_iterator.GetFileCount(); ++i) {
        u32 l_index = l_shape_anim_iterator.FindPathIndex(r_shape_anim_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresShapeAnim *shape_anim = r_bfres->shape_anim_array + r_bfres->shape_anim_dictionary->FindEntryIndex(r_shape_anim_paths[i]);
            ProcessResShapeAnimSingle(shape_anim, indent_level + 1, true);
        }
    }

    /* Right only scene anims */
    for (u32 i = 0; i < r_scene_anim_iterator.GetFileCount(); ++i) {
        u32 l_index = l_scene_anim_iterator.FindPathIndex(r_scene_anim_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresSceneAnim *scene_anim = r_bfres->scene_anim_array + r_bfres->scene_anim_dictionary->FindEntryIndex(r_scene_anim_paths[i]);
            ProcessResSceneAnimSingle(scene_anim, indent_level + 1, true);
        }
    }

    /* Right only embed files */
    for (u32 i = 0; i < r_embed_file_iterator.GetFileCount(); ++i) {
        u32 l_index = l_embed_file_iterator.FindPathIndex(r_embed_file_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResGfxEmbedFile *embed_file = r_bfres->embed_file_array + r_bfres->embed_file_dictionary->FindEntryIndex(r_embed_file_paths[i]);
            ProcessSingleImpl(embed_file->file_offset, embed_file->file_size, r_embed_file_paths[i], indent_level + 1, true);
        }
    }

    /* Left only models */
    for (u32 i = 0; i < l_model_iterator.GetFileCount(); ++i) {
        u32 r_index = r_model_iterator.FindPathIndex(l_model_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresModel *model = l_bfres->model_array + l_bfres->model_dictionary->FindEntryIndex(l_model_paths[i]);
            ProcessResModelSingle(model, indent_level + 1, false);
        }
    }

    /* Left only skeletal anims */
    for (u32 i = 0; i < l_skeletal_anim_iterator.GetFileCount(); ++i) {
        u32 r_index = r_skeletal_anim_iterator.FindPathIndex(l_skeletal_anim_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresSkeletalAnim *skeletal_anim = l_bfres->skeletal_anim_array + l_bfres->skeletal_anim_dictionary->FindEntryIndex(l_skeletal_anim_paths[i]);
            ProcessResSkeletalAnimSingle(skeletal_anim, indent_level + 1, false);
        }
    }

    /* Left only material anims */
    for (u32 i = 0; i < l_material_anim_iterator.GetFileCount(); ++i) {
        u32 r_index = r_material_anim_iterator.FindPathIndex(l_material_anim_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresMaterialAnim *material_anim = l_bfres->material_anim_array + l_bfres->material_anim_dictionary->FindEntryIndex(l_material_anim_paths[i]);
            ProcessResMaterialAnimSingle(material_anim, indent_level + 1, false);
        }
    }

    /* Left only bone visibility anims */
    for (u32 i = 0; i < l_bone_visibility_anim_iterator.GetFileCount(); ++i) {
        u32 r_index = r_bone_visibility_anim_iterator.FindPathIndex(l_bone_visibility_anim_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresBoneVisibilityAnim *bone_visibility_anim = l_bfres->bone_visibility_anim_array + l_bfres->bone_visibility_anim_dictionary->FindEntryIndex(l_bone_visibility_anim_paths[i]);
            ProcessResBoneVisibilityAnimSingle(bone_visibility_anim, indent_level + 1, false);
        }
    }

    /* Left only shape anims */
    for (u32 i = 0; i < l_shape_anim_iterator.GetFileCount(); ++i) {
        u32 r_index = r_shape_anim_iterator.FindPathIndex(l_shape_anim_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresShapeAnim *shape_anim = l_bfres->shape_anim_array + l_bfres->shape_anim_dictionary->FindEntryIndex(l_shape_anim_paths[i]);
            ProcessResShapeAnimSingle(shape_anim, indent_level + 1, false);
        }
    }

    /* Left only scene anims */
    for (u32 i = 0; i < l_scene_anim_iterator.GetFileCount(); ++i) {
        u32 r_index = r_scene_anim_iterator.FindPathIndex(l_scene_anim_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresSceneAnim *scene_anim = l_bfres->scene_anim_array + l_bfres->scene_anim_dictionary->FindEntryIndex(l_scene_anim_paths[i]);
            ProcessResSceneAnimSingle(scene_anim, indent_level + 1, false);
        }
    }

    /* Left only embed files */
    for (u32 i = 0; i < l_embed_file_iterator.GetFileCount(); ++i) {
        u32 r_index = r_embed_file_iterator.FindPathIndex(l_embed_file_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResGfxEmbedFile *embed_file = l_bfres->embed_file_array + l_bfres->embed_file_dictionary->FindEntryIndex(l_embed_file_paths[i]);
            ProcessSingleImpl(embed_file->file_offset, embed_file->file_size, l_embed_file_paths[i], indent_level + 1, false);
        }
    }

    /* cleanup */
    l_model_iterator.Finalize();
    l_skeletal_anim_iterator.Finalize();
    l_material_anim_iterator.Finalize();
    l_bone_visibility_anim_iterator.Finalize();
    l_shape_anim_iterator.Finalize();
    l_scene_anim_iterator.Finalize();
    l_embed_file_iterator.Finalize();

    r_model_iterator.Finalize();
    r_skeletal_anim_iterator.Finalize();
    r_material_anim_iterator.Finalize();
    r_bone_visibility_anim_iterator.Finalize();
    r_shape_anim_iterator.Finalize();
    r_scene_anim_iterator.Finalize();
    r_embed_file_iterator.Finalize();
}

void ProcessResMaterialSingle(dd::res::ResBfresMaterial *material, u32 indent_level, bool is_right) {

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

    /* Render info */
    char **render_info_paths = render_info_iterator.GetFilePathArray();
    for (u32 i = 0; i < render_info_iterator.GetFileCount(); ++i) {
        PrintIndent(indent_level + 1);
        if (is_right) {
            std::cout << "Right only(render info): " << render_info_paths[i] << std::endl;
        } else {
            std::cout << "Left only (render info): " << render_info_paths[i] << std::endl;
        }
    }

    /* shader param */
    char **shader_param_paths = shader_param_iterator.GetFilePathArray();
    for (u32 i = 0; i < shader_param_iterator.GetFileCount(); ++i) {
        PrintIndent(indent_level + 1);
        if (is_right) {
            std::cout << "Right only(shader param): " << shader_param_paths[i] << std::endl;
        } else {
            std::cout << "Left only (shader param): " << shader_param_paths[i] << std::endl;
        }
    }

    /* vertex attribute */
    char **vertex_attribute_paths = vertex_attribute_iterator.GetFilePathArray();
    for (u32 i = 0; i < vertex_attribute_iterator.GetFileCount(); ++i) {
        u32 v_idx = (material->material_shader_data->vertex_attribute_index_array != nullptr) ? material->material_shader_data->vertex_attribute_index_array[i] : i;
        PrintIndent(indent_level + 1);
        if (is_right) {
            std::cout << "Right only(vertex attribute): " << vertex_attribute_paths[i] << "(index: "  << v_idx << ")" << std::endl;
        } else {
            std::cout << "Left only (vertex attribute): " << vertex_attribute_paths[i] << "(index: "  << v_idx << ")" << std::endl;
        }
    }

    /* sampler */
    char **sampler_paths = sampler_iterator.GetFilePathArray();
    for (u32 i = 0; i < sampler_iterator.GetFileCount(); ++i) {
        u32 m_idx = (material->material_shader_data->sampler_index_array != nullptr) ? material->material_shader_data->sampler_index_array[i] : i;
        PrintIndent(indent_level + 1);
        if (is_right) {
            std::cout << "Right only(sampler): " << sampler_paths[i] << "(index: " << m_idx << ")" << std::endl;
        } else {
            std::cout << "Left only (sampler): " << sampler_paths[i] << "(index: " << m_idx << ")" << std::endl;
        }
    }

    /* static shader option */
    char **static_shader_option_paths = static_shader_option_iterator.GetFilePathArray();
    for (u32 i = 0; i < static_shader_option_iterator.GetFileCount(); ++i) {
        u32 index = (material->material_shader_data->static_option_indice_array != nullptr) ? material->material_shader_data->static_option_indice_array[material->material_shader_data->total_static_option_count + material->material_shader_data->shader_reflection->static_shader_option_dictionary->FindEntryIndex(static_shader_option_paths[i])] : material->material_shader_data->shader_reflection->static_shader_option_dictionary->FindEntryIndex(static_shader_option_paths[i]);

        PrintIndent(indent_level + 1);
        if (index < material->material_shader_data->int_static_option_count) {
            s32 int_param = material->material_shader_data->static_shader_option_int_value_array[index];
            if (is_right) {
                std::cout << "Right only(static shader option): " << static_shader_option_paths[i] << "(value(int): " << int_param << ")" << std::endl;
            } else {
                std::cout << "Left only (static shader option): " << static_shader_option_paths[i] << "(value(int): " << int_param << ")" << std::endl;
            }
        } else {
            const char *string_param = material->material_shader_data->static_shader_option_string_value_array[(index - material->material_shader_data->int_static_option_count)] + 2;
            if (is_right) {
                std::cout << "Right only(static shader option): " << static_shader_option_paths[i] << "(value(char): " << string_param << ")" << std::endl;
            } else {
                std::cout << "Left only (static shader option): " << static_shader_option_paths[i] << "(value(char): " << string_param << ")" << std::endl;
            }
        }
    }

    /* cleanup */
    render_info_iterator.Finalize();
    shader_param_iterator.Finalize();
    vertex_attribute_iterator.Finalize();
    sampler_iterator.Finalize();
    static_shader_option_iterator.Finalize();
}

void ProcessBfresSingle(void *file, u32 indent_level, bool is_right) {

    /* rescast */
    dd::res::ResBfres *bfres = dd::res::ResBfres::ResCast(file);

    /* dictionary iterators */
    RomfsDirectoryParser model_iterator = {};
    model_iterator.InitializeByResDic(bfres->model_dictionary);
    RomfsDirectoryParser skeletal_anim_iterator = {};
    skeletal_anim_iterator.InitializeByResDic(bfres->skeletal_anim_dictionary);
    RomfsDirectoryParser material_anim_iterator = {};
    material_anim_iterator.InitializeByResDic(bfres->material_anim_dictionary);
    RomfsDirectoryParser bone_visibility_anim_iterator = {};
    bone_visibility_anim_iterator.InitializeByResDic(bfres->bone_visibility_anim_dictionary);
    RomfsDirectoryParser shape_anim_iterator = {};
    shape_anim_iterator.InitializeByResDic(bfres->shape_anim_dictionary);
    RomfsDirectoryParser scene_anim_iterator = {};
    scene_anim_iterator.InitializeByResDic(bfres->scene_anim_dictionary);
    RomfsDirectoryParser embed_file_iterator = {};
    embed_file_iterator.InitializeByResDic(bfres->embed_file_dictionary);

    /* print resmodels */
    char **model_paths = model_iterator.GetFilePathArray();
    for (u32 i = 0; i < model_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresModel *model = bfres->model_array + bfres->model_dictionary->FindEntryIndex(model_paths[i]);
        ProcessResModelSingle(model, indent_level, is_right);
    }

    /* print skeletal anims */
    char **skeletal_anim_paths = skeletal_anim_iterator.GetFilePathArray();
    for (u32 i = 0; i < skeletal_anim_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresSkeletalAnim *skeletal_anim = bfres->skeletal_anim_array + bfres->skeletal_anim_dictionary->FindEntryIndex(skeletal_anim_paths[i]);
        ProcessResSkeletalAnimSingle(skeletal_anim, indent_level, is_right);
    }

    /* print material anims */
    char **material_anim_paths = material_anim_iterator.GetFilePathArray();
    for (u32 i = 0; i < material_anim_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresMaterialAnim *material_anim = bfres->material_anim_array + bfres->material_anim_dictionary->FindEntryIndex(material_anim_paths[i]);
        ProcessResMaterialAnimSingle(material_anim, indent_level, is_right);
    }

    /* print bone visibility anims */
    char **bone_visibility_anim_paths = bone_visibility_anim_iterator.GetFilePathArray();
    for (u32 i = 0; i < bone_visibility_anim_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresBoneVisibilityAnim *bone_visibility_anim = bfres->bone_visibility_anim_array + bfres->bone_visibility_anim_dictionary->FindEntryIndex(bone_visibility_anim_paths[i]);
        ProcessResBoneVisibilityAnimSingle(bone_visibility_anim, indent_level, is_right);
    }

    /* print shape anims */
    char **shape_anim_paths = shape_anim_iterator.GetFilePathArray();
    for (u32 i = 0; i < shape_anim_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresShapeAnim *shape_anim = bfres->shape_anim_array + bfres->shape_anim_dictionary->FindEntryIndex(shape_anim_paths[i]);
        ProcessResShapeAnimSingle(shape_anim, indent_level, is_right);
    }

    /* print scene anims*/
    char **scene_anim_paths = scene_anim_iterator.GetFilePathArray();
    for (u32 i = 0; i < scene_anim_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresSceneAnim *scene_anim = bfres->scene_anim_array + bfres->scene_anim_dictionary->FindEntryIndex(scene_anim_paths[i]);
        ProcessResSceneAnimSingle(scene_anim, indent_level, is_right);
    }

    char **embed_file_paths = embed_file_iterator.GetFilePathArray();
    for (u32 i = 0; i < embed_file_iterator.GetFileCount(); ++i) {
        dd::res::ResGfxEmbedFile *embed_file = bfres->embed_file_array + bfres->embed_file_dictionary->FindEntryIndex(embed_file_paths[i]);
        ProcessSingleImpl(embed_file->file_offset, embed_file->file_size, embed_file_paths[i], indent_level, is_right);
    }

    /* cleanup */
    model_iterator.Finalize();
    skeletal_anim_iterator.Finalize();
    material_anim_iterator.Finalize();
    bone_visibility_anim_iterator.Finalize();
    shape_anim_iterator.Finalize();
    scene_anim_iterator.Finalize();
    embed_file_iterator.Finalize();
}
