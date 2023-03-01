#pragma once

void ProcessResModelSingle(dd::res::ResBfresModel *model, u32 indent_level, PrintSide print_side) {

    PrintIndent(indent_level);
    PrintOnlySide(print_side);
    std::cout << ": " << (model->model_name + 2) << ".fmdl" << std::endl;

    RomfsDirectoryParser shape_iterator = {};
    shape_iterator.InitializeByResDic(model->shape_dictionary);
    RomfsDirectoryParser material_iterator = {};
    material_iterator.InitializeByResDic(model->material_dictionary);

    char **shape_paths = shape_iterator.GetFilePathArray();
    for (u32 i = 0; i < shape_iterator.GetFileCount(); ++i) {
        PrintIndent(indent_level + 1);
        PrintOnlySide(print_side);
        std::cout << ": " << shape_paths[i] << ".fshp" << std::endl;
    }

    char **material_paths = material_iterator.GetFilePathArray();
    for (u32 i = 0; i < material_iterator.GetFileCount(); ++i) {
        PrintIndent(indent_level + 1);
        PrintOnlySide(print_side);
        std::cout << ": " << material_paths[i] << ".fmat" << std::endl;

        //dd::res::ResBfresMaterial *material = model->material_array + model->material_dictionary->FindEntryIndex(material_paths[i]);
        //ProcessResMaterialSingle(material, indent_level + 1, print_side);
    }

    /* cleanup */
    shape_iterator.Finalize();
    material_iterator.Finalize();
}

bool DiffBfresModel(dd::res::ResBfresModel *l_model, dd::res::ResBfresModel *r_model, void *l_gpu_memory, void *r_gpu_memory, u32 indent_level, bool is_print) {

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
            dd::res::ResBfresShape *l_shape = std::addressof(l_model->shape_array[l_model->shape_dictionary->FindEntryIndex(l_shape_paths[i])]);
            dd::res::ResBfresShape *r_shape = std::addressof(r_model->shape_array[r_model->shape_dictionary->FindEntryIndex(l_shape_paths[i])]);

            const bool is_same = DiffBfresShape(l_shape, r_shape, l_model, r_model, l_gpu_memory, r_gpu_memory, indent_level + 1, false);
            if (is_same == true) { continue; }
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            std::cout << "Different: " << l_shape_paths[i] << ".fshp" << std::endl;
            DiffBfresShape(l_shape, r_shape, l_model, r_model, l_gpu_memory, r_gpu_memory, indent_level + 2, true);
        }
    }

    /* Compare materials */
    char **l_material_paths = l_material_iterator.GetFilePathArray();
    char **r_material_paths = r_material_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_material_iterator.GetFileCount(); ++i) {
        u32 r_index = r_material_iterator.FindPathIndex(l_material_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {
            dd::res::ResBfresMaterial *l_material = std::addressof(l_model->material_array[l_model->material_dictionary->FindEntryIndex(l_material_paths[i])]);
            dd::res::ResBfresMaterial *r_material = std::addressof(r_model->material_array[r_model->material_dictionary->FindEntryIndex(l_material_paths[i])]);

            const bool is_same = DiffBfresMaterial(l_material, r_material, indent_level + 1, false);
            if (is_same == true) { continue; }
            if (is_print == false) { return false; }

            PrintIndent(indent_level + 1);
            std::cout << "Different: " << l_material_paths[i] << ".fmat" << std::endl;
            DiffBfresMaterial(l_material, r_material, indent_level + 1, true);
        }
    }

    /* Compare user data */
    const bool is_same = DiffGfxUserData(l_model->user_data_array, l_model->user_data_dictionary, r_model->user_data_array, r_model->user_data_dictionary, indent_level + 1, false);
    if (is_same == false && is_print == false) { return false; }
    DiffGfxUserData(l_model->user_data_array, l_model->user_data_dictionary, r_model->user_data_array, r_model->user_data_dictionary, indent_level + 1, true);

    /* Right only shapes */
    for (u32 i = 0; i < r_shape_iterator.GetFileCount(); ++i) {
        u32 l_index = l_shape_iterator.FindPathIndex(r_shape_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresShape *shp = r_model->shape_array + r_model->shape_dictionary->FindEntryIndex(r_shape_paths[i]);
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << (shp->shape_name + 2) << ".fshp" <<  std::endl;
        }
    }

    /* Right only materials */
    for (u32 i = 0; i < r_material_iterator.GetFileCount(); ++i) {
        u32 l_index = l_material_iterator.FindPathIndex(r_material_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresMaterial *mat = r_model->material_array + r_model->material_dictionary->FindEntryIndex(r_material_paths[i]);
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << (mat->material_name + 2) << ".fmat" <<  std::endl;
        }
    }

    /* Right only user_data */
    const bool right_is_same = RightOnlyGfxUserData(l_model->user_data_array, l_model->user_data_dictionary, r_model->user_data_array, r_model->user_data_dictionary, indent_level + 1, false);
    if (right_is_same == false && is_print == false) { return false; }
    RightOnlyGfxUserData(l_model->user_data_array, l_model->user_data_dictionary, r_model->user_data_array, r_model->user_data_dictionary, indent_level + 1, true);

    /* Left only shapes */
    for (u32 i = 0; i < l_shape_iterator.GetFileCount(); ++i) {
        u32 r_index = r_shape_iterator.FindPathIndex(l_shape_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresShape *shp = l_model->shape_array + l_model->shape_dictionary->FindEntryIndex(l_shape_paths[i]);
            PrintIndent(indent_level + 1);
            std::cout << "Left only : " << (shp->shape_name + 2) << ".fshp" <<  std::endl;
        }
    }

    /* Left only materials */
    for (u32 i = 0; i < l_material_iterator.GetFileCount(); ++i) {
        u32 r_index = r_material_iterator.FindPathIndex(l_material_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            if (is_print == false) { return false; }

            dd::res::ResBfresMaterial *mat = l_model->material_array + l_model->material_dictionary->FindEntryIndex(l_material_paths[i]);
            PrintIndent(indent_level + 1);
            std::cout << "Left only : " << (mat->material_name + 2) << ".fmat" <<  std::endl;
        }
    }

    /* Left only user_data */
    const bool left_is_same = LeftOnlyGfxUserData(l_model->user_data_array, l_model->user_data_dictionary, r_model->user_data_array, r_model->user_data_dictionary, indent_level + 1, false);
    if (left_is_same == false && is_print == false) { return false; }
    LeftOnlyGfxUserData(l_model->user_data_array, l_model->user_data_dictionary, r_model->user_data_array, r_model->user_data_dictionary, indent_level + 1, true);

    /* cleanup */
    l_shape_iterator.Finalize();
    l_material_iterator.Finalize();

    r_shape_iterator.Finalize();
    r_material_iterator.Finalize();

    return true;
}

