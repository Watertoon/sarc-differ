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

void ProcessResSkeletalAnimSingle(dd::res::ResBfresSkeletalAnim *skeletal_anim, u32 indent_level, PrintSide print_side) {
    PrintIndent(indent_level);
    PrintOnlySide(print_side);
    std::cout << ": " << (skeletal_anim->animation_name + 2) << ".fska" << std::endl;
}

void ProcessResMaterialAnimSingle(dd::res::ResBfresMaterialAnim *material_anim, u32 indent_level, PrintSide print_side) {
    PrintIndent(indent_level);
    PrintOnlySide(print_side);
    std::cout << ": " << (material_anim->animation_name + 2) << ".fmaa" << std::endl;
}

void ProcessResBoneVisibilityAnimSingle(dd::res::ResBfresBoneVisibilityAnim *bone_visibility_anim, u32 indent_level, PrintSide print_side) {
    PrintIndent(indent_level);
    PrintOnlySide(print_side);
    std::cout << ": " << (bone_visibility_anim->animation_name + 2) << ".fvba" << std::endl;
}

void ProcessResShapeAnimSingle(dd::res::ResBfresShapeAnim *shape_anim, u32 indent_level, PrintSide print_side) {
    PrintIndent(indent_level);
    PrintOnlySide(print_side);
    std::cout << ": " << (shape_anim->animation_name + 2) << ".fsha" << std::endl;
}

void ProcessResSceneAnimSingle(dd::res::ResBfresSceneAnim *scene_anim, u32 indent_level, PrintSide print_side) {
    PrintIndent(indent_level);
    PrintOnlySide(print_side);
    std::cout << ": " << (scene_anim->animation_name + 2) << ".fscn" << std::endl;

    RomfsDirectoryParser camera_anim_iterator = {};
    camera_anim_iterator.InitializeByResDic(scene_anim->camera_anim_dictionary);
    RomfsDirectoryParser light_anim_iterator = {};
    light_anim_iterator.InitializeByResDic(scene_anim->light_anim_dictionary);
    RomfsDirectoryParser fog_anim_iterator = {};
    fog_anim_iterator.InitializeByResDic(scene_anim->fog_anim_dictionary);

    char **camera_anim_paths = camera_anim_iterator.GetFilePathArray();
    for (u32 y = 0; y < camera_anim_iterator.GetFileCount(); ++y) {
        PrintIndent(indent_level + 1);
        PrintOnlySide(print_side);
        std::cout << ": " << camera_anim_paths[y] << ".fcam" << std::endl;
    }
    char **light_anim_paths = light_anim_iterator.GetFilePathArray();
    for (u32 y = 0; y < light_anim_iterator.GetFileCount(); ++y) {
        PrintIndent(indent_level + 1);
        PrintOnlySide(print_side);
        std::cout << ": " << light_anim_paths[y] << ".flit" << std::endl;
    }
    char **fog_anim_paths = fog_anim_iterator.GetFilePathArray();
    for (u32 y = 0; y < fog_anim_iterator.GetFileCount(); ++y) {
        PrintIndent(indent_level + 1);
        PrintOnlySide(print_side);
        std::cout << ": " << fog_anim_paths[y] << ".ffog" << std::endl;
    }

    camera_anim_iterator.Finalize();
    light_anim_iterator.Finalize();
    fog_anim_iterator.Finalize();
}

bool DiffBfres(void *l_file, void *r_file, u32 indent_level, bool is_print) {

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


    /* Diff models */
    
    for (u32 i = 0; i < r_model_iterator.GetFileCount(); ++i) {
        u32 l_index = l_model_iterator.FindPathIndex(r_model_paths[i]);
        if (l_index != RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresModel *l_model = l_bfres->model_array + l_bfres->model_dictionary->FindEntryIndex(l_model_paths[l_index]);
            dd::res::ResBfresModel *r_model = r_bfres->model_array + r_bfres->model_dictionary->FindEntryIndex(r_model_paths[i]);

            const bool is_same = DiffBfresModel(l_model, r_model, indent_level + 1, false);
            if (is_same == true) { continue; }
            if (is_print == false) { return false; }
            
            PrintIndent(indent_level + 1);
            std::cout << "Different: " << (l_model->model_name + 2) << ".fmdl" << std::endl;
            DiffBfresModel(l_model, r_model, indent_level + 1, true);
        }
    }

    /* Diff embed files */
    for (u32 i = 0; i < r_embed_file_iterator.GetFileCount(); ++i) {
        u32 l_index = l_embed_file_iterator.FindPathIndex(r_embed_file_paths[i]);
        if (l_index != RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResGfxEmbedFile *l_embed_file = l_bfres->embed_file_array + l_bfres->embed_file_dictionary->FindEntryIndex(l_embed_file_paths[l_index]);
            dd::res::ResGfxEmbedFile *r_embed_file = r_bfres->embed_file_array + r_bfres->embed_file_dictionary->FindEntryIndex(r_embed_file_paths[i]);

            const bool is_same = DiffGfxEmbedFile(l_embed_file, r_embed_file, r_embed_file_paths[i], indent_level + 1, false);
            if (is_same == true) { continue; }
            if (is_print == false) { return false; }
            DiffGfxEmbedFile(l_embed_file, r_embed_file, r_embed_file_paths[i], indent_level + 1, true);
        }
    }

    /* Right only models */
    for (u32 i = 0; i < r_model_iterator.GetFileCount(); ++i) {
        u32 l_index = l_model_iterator.FindPathIndex(r_model_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresModel *model = r_bfres->model_array + r_bfres->model_dictionary->FindEntryIndex(r_model_paths[i]);
            ProcessResModelSingle(model, indent_level + 1, PrintSide::Right);
        }
    }

    /* Right only skeletal anims */
    for (u32 i = 0; i < r_skeletal_anim_iterator.GetFileCount(); ++i) {
        u32 l_index = l_skeletal_anim_iterator.FindPathIndex(r_skeletal_anim_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresSkeletalAnim *skeletal_anim = r_bfres->skeletal_anim_array + r_bfres->skeletal_anim_dictionary->FindEntryIndex(r_skeletal_anim_paths[i]);
            ProcessResSkeletalAnimSingle(skeletal_anim, indent_level + 1, PrintSide::Right);
        }
    }

    /* Right only material anims */
    for (u32 i = 0; i < r_material_anim_iterator.GetFileCount(); ++i) {
        u32 l_index = l_material_anim_iterator.FindPathIndex(r_material_anim_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresMaterialAnim *material_anim = r_bfres->material_anim_array + r_bfres->material_anim_dictionary->FindEntryIndex(r_material_anim_paths[i]);
            ProcessResMaterialAnimSingle(material_anim, indent_level + 1, PrintSide::Right);
        }
    }

    /* Right only bone visibility anims */
    for (u32 i = 0; i < r_bone_visibility_anim_iterator.GetFileCount(); ++i) {
        u32 l_index = l_bone_visibility_anim_iterator.FindPathIndex(r_bone_visibility_anim_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresBoneVisibilityAnim *bone_visibility_anim = r_bfres->bone_visibility_anim_array + r_bfres->bone_visibility_anim_dictionary->FindEntryIndex(r_bone_visibility_anim_paths[i]);
            ProcessResBoneVisibilityAnimSingle(bone_visibility_anim, indent_level + 1, PrintSide::Right);
        }
    }

    /* Right only shape anims */
    for (u32 i = 0; i < r_shape_anim_iterator.GetFileCount(); ++i) {
        u32 l_index = l_shape_anim_iterator.FindPathIndex(r_shape_anim_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresShapeAnim *shape_anim = r_bfres->shape_anim_array + r_bfres->shape_anim_dictionary->FindEntryIndex(r_shape_anim_paths[i]);
            ProcessResShapeAnimSingle(shape_anim, indent_level + 1, PrintSide::Right);
        }
    }

    /* Right only scene anims */
    for (u32 i = 0; i < r_scene_anim_iterator.GetFileCount(); ++i) {
        u32 l_index = l_scene_anim_iterator.FindPathIndex(r_scene_anim_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresSceneAnim *scene_anim = r_bfres->scene_anim_array + r_bfres->scene_anim_dictionary->FindEntryIndex(r_scene_anim_paths[i]);
            ProcessResSceneAnimSingle(scene_anim, indent_level + 1, PrintSide::Right);
        }
    }

    /* Right only embed files */
    for (u32 i = 0; i < r_embed_file_iterator.GetFileCount(); ++i) {
        u32 l_index = l_embed_file_iterator.FindPathIndex(r_embed_file_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResGfxEmbedFile *embed_file = r_bfres->embed_file_array + r_bfres->embed_file_dictionary->FindEntryIndex(r_embed_file_paths[i]);
            ProcessSingleImpl(embed_file->file_offset, embed_file->file_size, r_embed_file_paths[i], indent_level + 1, PrintSide::Right);
        }
    }

    /* Left only models */
    for (u32 i = 0; i < l_model_iterator.GetFileCount(); ++i) {
        u32 r_index = r_model_iterator.FindPathIndex(l_model_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresModel *model = l_bfres->model_array + l_bfres->model_dictionary->FindEntryIndex(l_model_paths[i]);
            ProcessResModelSingle(model, indent_level + 1, PrintSide::Left);
        }
    }

    /* Left only skeletal anims */
    for (u32 i = 0; i < l_skeletal_anim_iterator.GetFileCount(); ++i) {
        u32 r_index = r_skeletal_anim_iterator.FindPathIndex(l_skeletal_anim_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresSkeletalAnim *skeletal_anim = l_bfres->skeletal_anim_array + l_bfres->skeletal_anim_dictionary->FindEntryIndex(l_skeletal_anim_paths[i]);
            ProcessResSkeletalAnimSingle(skeletal_anim, indent_level + 1, PrintSide::Left);
        }
    }

    /* Left only material anims */
    for (u32 i = 0; i < l_material_anim_iterator.GetFileCount(); ++i) {
        u32 r_index = r_material_anim_iterator.FindPathIndex(l_material_anim_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresMaterialAnim *material_anim = l_bfres->material_anim_array + l_bfres->material_anim_dictionary->FindEntryIndex(l_material_anim_paths[i]);
            ProcessResMaterialAnimSingle(material_anim, indent_level + 1, PrintSide::Left);
        }
    }

    /* Left only bone visibility anims */
    for (u32 i = 0; i < l_bone_visibility_anim_iterator.GetFileCount(); ++i) {
        u32 r_index = r_bone_visibility_anim_iterator.FindPathIndex(l_bone_visibility_anim_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresBoneVisibilityAnim *bone_visibility_anim = l_bfres->bone_visibility_anim_array + l_bfres->bone_visibility_anim_dictionary->FindEntryIndex(l_bone_visibility_anim_paths[i]);
            ProcessResBoneVisibilityAnimSingle(bone_visibility_anim, indent_level + 1, PrintSide::Left);
        }
    }

    /* Left only shape anims */
    for (u32 i = 0; i < l_shape_anim_iterator.GetFileCount(); ++i) {
        u32 r_index = r_shape_anim_iterator.FindPathIndex(l_shape_anim_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresShapeAnim *shape_anim = l_bfres->shape_anim_array + l_bfres->shape_anim_dictionary->FindEntryIndex(l_shape_anim_paths[i]);
            ProcessResShapeAnimSingle(shape_anim, indent_level + 1, PrintSide::Left);
        }
    }

    /* Left only scene anims */
    for (u32 i = 0; i < l_scene_anim_iterator.GetFileCount(); ++i) {
        u32 r_index = r_scene_anim_iterator.FindPathIndex(l_scene_anim_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresSceneAnim *scene_anim = l_bfres->scene_anim_array + l_bfres->scene_anim_dictionary->FindEntryIndex(l_scene_anim_paths[i]);
            ProcessResSceneAnimSingle(scene_anim, indent_level + 1, PrintSide::Left);
        }
    }

    /* Left only embed files */
    for (u32 i = 0; i < l_embed_file_iterator.GetFileCount(); ++i) {
        u32 r_index = r_embed_file_iterator.FindPathIndex(l_embed_file_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResGfxEmbedFile *embed_file = l_bfres->embed_file_array + l_bfres->embed_file_dictionary->FindEntryIndex(l_embed_file_paths[i]);
            ProcessSingleImpl(embed_file->file_offset, embed_file->file_size, l_embed_file_paths[i], indent_level + 1, PrintSide::Left);
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

    return true;
}

void ProcessBfresSingle(void *file, u32 indent_level, PrintSide print_side) {

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
        ProcessResModelSingle(model, indent_level, print_side);
    }

    /* print skeletal anims */
    char **skeletal_anim_paths = skeletal_anim_iterator.GetFilePathArray();
    for (u32 i = 0; i < skeletal_anim_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresSkeletalAnim *skeletal_anim = bfres->skeletal_anim_array + bfres->skeletal_anim_dictionary->FindEntryIndex(skeletal_anim_paths[i]);
        ProcessResSkeletalAnimSingle(skeletal_anim, indent_level, print_side);
    }

    /* print material anims */
    char **material_anim_paths = material_anim_iterator.GetFilePathArray();
    for (u32 i = 0; i < material_anim_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresMaterialAnim *material_anim = bfres->material_anim_array + bfres->material_anim_dictionary->FindEntryIndex(material_anim_paths[i]);
        ProcessResMaterialAnimSingle(material_anim, indent_level, print_side);
    }

    /* print bone visibility anims */
    char **bone_visibility_anim_paths = bone_visibility_anim_iterator.GetFilePathArray();
    for (u32 i = 0; i < bone_visibility_anim_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresBoneVisibilityAnim *bone_visibility_anim = bfres->bone_visibility_anim_array + bfres->bone_visibility_anim_dictionary->FindEntryIndex(bone_visibility_anim_paths[i]);
        ProcessResBoneVisibilityAnimSingle(bone_visibility_anim, indent_level, print_side);
    }

    /* print shape anims */
    char **shape_anim_paths = shape_anim_iterator.GetFilePathArray();
    for (u32 i = 0; i < shape_anim_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresShapeAnim *shape_anim = bfres->shape_anim_array + bfres->shape_anim_dictionary->FindEntryIndex(shape_anim_paths[i]);
        ProcessResShapeAnimSingle(shape_anim, indent_level, print_side);
    }

    /* print scene anims*/
    char **scene_anim_paths = scene_anim_iterator.GetFilePathArray();
    for (u32 i = 0; i < scene_anim_iterator.GetFileCount(); ++i) {
        dd::res::ResBfresSceneAnim *scene_anim = bfres->scene_anim_array + bfres->scene_anim_dictionary->FindEntryIndex(scene_anim_paths[i]);
        ProcessResSceneAnimSingle(scene_anim, indent_level, print_side);
    }

    char **embed_file_paths = embed_file_iterator.GetFilePathArray();
    for (u32 i = 0; i < embed_file_iterator.GetFileCount(); ++i) {
        dd::res::ResGfxEmbedFile *embed_file = bfres->embed_file_array + bfres->embed_file_dictionary->FindEntryIndex(embed_file_paths[i]);
        ProcessSingleImpl(embed_file->file_offset, embed_file->file_size, embed_file_paths[i], indent_level, print_side);
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
