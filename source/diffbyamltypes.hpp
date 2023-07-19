#pragma once

bool DiffByamlIteratorByParser(vp::res::ByamlIterator *l_root_iter, vp::res::ByamlIterator *r_root_iter, const char *identifier_key, const char *array_name_key, ByamlDataOverrideCallback override_callback, u32 indent_level, bool is_print) {

    ByamlDictionaryParser l_parser = {};
    ByamlDictionaryParser r_parser = {};
    const bool l_result = l_parser.Initialize(l_root_iter, identifier_key);
    const bool r_result = r_parser.Initialize(r_root_iter, identifier_key);

    if (l_result != r_result && is_print == false) { return false; }

    if (l_result == false && r_result == false) { return true; }
    if (l_result == false && r_result == true) {
        if (is_print == false) { return false; }
        r_parser.ProcessAllByamlDataSingle(array_name_key, indent_level, PrintSide::Right);
        return true;
    }
    if (l_result == true && r_result == false) {
        if (is_print == false) { return false; }
        l_parser.ProcessAllByamlDataSingle(array_name_key, indent_level, PrintSide::Left);
        return true;
    }

    const bool is_same = l_parser.DiffAllByamlData(std::addressof(r_parser), array_name_key, override_callback, indent_level, false);
    if (is_same == false && is_print == false) { return false; }
    l_parser.DiffAllByamlData(std::addressof(r_parser), array_name_key, override_callback, indent_level, true);
    r_parser.ProcessAllByamlDataOnly(std::addressof(l_parser), array_name_key, indent_level, PrintSide::Right);
    l_parser.ProcessAllByamlDataOnly(std::addressof(r_parser), array_name_key, indent_level, PrintSide::Left);
    return true;
}

bool BkresModelElementCallback(vp::res::ByamlIterator *l_iterator, vp::res::ByamlIterator *r_iterator, vp::res::ByamlData lhs, vp::res::ByamlData rhs, u32 indent_level) {

    /* Get keys */
    const char *l_key = nullptr;
    const char *r_key = nullptr;
    const bool result0 = l_iterator->TryGetKeyByData(std::addressof(l_key), lhs);
    const bool result1 = r_iterator->TryGetKeyByData(std::addressof(r_key), rhs);
    if (result0 == false || result1 == false || ::strcmp(l_key, r_key) != 0) { return false; }

    if (::strcmp(l_key, "ModelElements") == 0) {
        vp::res::ByamlIterator l_model_iter;
        vp::res::ByamlIterator r_model_iter;
        const bool result2 = l_iterator->TryGetIteratorByData(std::addressof(l_model_iter), lhs);
        const bool result3 = r_iterator->TryGetIteratorByData(std::addressof(r_model_iter), rhs);
        if (result2 == false || result3 == false) { return false; }

        DiffByamlIteratorByParser(std::addressof(l_model_iter), std::addressof(r_model_iter), "Guid", "ModelName", BkresModelElementCallback, indent_level, true);
        return true;
    }
    if (::strcmp(l_key, "MaterialElements") == 0) {
        vp::res::ByamlIterator l_mat_iter;
        vp::res::ByamlIterator r_mat_iter;
        const bool result2 = l_iterator->TryGetIteratorByData(std::addressof(l_mat_iter), lhs);
        const bool result3 = r_iterator->TryGetIteratorByData(std::addressof(r_mat_iter), rhs);
        if (result2 == false || result3 == false) { return false; }

        DiffByamlIteratorByParser(std::addressof(l_mat_iter), std::addressof(r_mat_iter), "MaterialName", "MaterialName", BkresModelElementCallback, indent_level, true);
        return true;
    }

    return false;
}

bool DiffBkresDataElements(vp::res::ByamlIterator *l_data_elements_iter, vp::res::ByamlIterator *r_data_elements_iter, u32 indent_level, bool is_print) {

    const bool is_same = DiffByamlIteratorByParser(l_data_elements_iter, r_data_elements_iter, "DataType", "DataType", BkresModelElementCallback, indent_level, false);
    if (is_same == false) {
        if (is_print == false) { return false; }
        DiffByamlIteratorByParser(l_data_elements_iter, r_data_elements_iter, "DataType", "DataType", BkresModelElementCallback, indent_level, true);
    }

    return true;
}

bool DiffBkres(void *left_file, void *right_file, u32 indent_level, bool is_print) {

    /* Initialize iterator */
    vp::res::ByamlIterator l_root_iter(left_file);
    vp::res::ByamlIterator r_root_iter(right_file);

    /* Compare byml */
    const s32 result = CompareByamlIterator(std::addressof(l_root_iter), std::addressof(r_root_iter));
    if (result == 0)       { return true; }
    if (is_print == false) { return false; } 

    /* Get data elements */
    vp::res::ByamlIterator l_data_elements_iter;
    vp::res::ByamlIterator r_data_elements_iter;
    const bool result0 = l_root_iter.TryGetIteratorByKey(std::addressof(l_data_elements_iter), "DataElements");
    const bool result1 = r_root_iter.TryGetIteratorByKey(std::addressof(r_data_elements_iter), "DataElements");
    if (result0 == false || result1 == false) {
        if (is_print == false) { return false; }
        std::cout << "Currently Unsupported" << std::endl;
        return false;
    }

    const bool is_data_elements_same = DiffBkresDataElements(std::addressof(l_data_elements_iter), std::addressof(r_data_elements_iter), indent_level + 1, false);
        if (is_data_elements_same == false) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level);
        std::cout << "Different (DataElements): " << std::endl;
        DiffBkresDataElements(std::addressof(l_data_elements_iter), std::addressof(r_data_elements_iter), indent_level + 1, true);
    }

    return true;
}

bool DiffByamlFull(void *left_file, void *right_file, u32 indent_level, bool is_print) {

    /* Initialize iterator */
    vp::res::ByamlIterator l_root_iter(left_file);
    vp::res::ByamlIterator r_root_iter(right_file);

    const s32 result = CompareByamlIterator(std::addressof(l_root_iter), std::addressof(r_root_iter));
    if (result == 0) { return true; }
    if (is_print == false) {return false; } 

    DiffByamlIterator(std::addressof(l_root_iter), std::addressof(r_root_iter), indent_level);

    return false;
}

bool DiffBcett(void *left_file, void *right_file, u32 indent_level, bool is_print) {

    /* Initialize iterator */
    vp::res::ByamlIterator l_root_iter(left_file);
    vp::res::ByamlIterator r_root_iter(right_file);

    /* Get Actor iterator */
    vp::res::ByamlIterator l_actor_iter;
    vp::res::ByamlIterator r_actor_iter;
    const bool l_actor_result = l_root_iter.TryGetIteratorByKey(std::addressof(l_actor_iter), "Actors");
    const bool r_actor_result = r_root_iter.TryGetIteratorByKey(std::addressof(r_actor_iter), "Actors");
    if (l_actor_result != r_actor_result && is_print == false) { return false; }

    /* Diff Actors */
    const bool is_actor_same = DiffByamlIteratorByParser(std::addressof(l_actor_iter), std::addressof(r_actor_iter), "Hash", "Name", nullptr, indent_level + 1, false);
    if (is_actor_same == false) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level + 1);
        std::cout << "Different (Actors): " << std::endl;
        DiffByamlIteratorByParser(std::addressof(l_actor_iter), std::addressof(r_actor_iter), "Hash", "Name", nullptr, indent_level + 2, true);
    }

    /* Get Rail iterator */
    vp::res::ByamlIterator l_rail_iter;
    vp::res::ByamlIterator r_rail_iter;
    const bool l_rail_result = l_root_iter.TryGetIteratorByKey(std::addressof(l_rail_iter), "Rails");
    const bool r_rail_result = r_root_iter.TryGetIteratorByKey(std::addressof(r_rail_iter), "Rails");
    if (l_rail_result != r_rail_result && is_print == false) { return false; }

    /* Diff Rails */
    const bool is_rail_same = DiffByamlIteratorByParser(std::addressof(l_rail_iter), std::addressof(r_rail_iter), "Hash", "Name", nullptr, indent_level + 1, false);
    if (is_rail_same == false) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level + 1);
        std::cout << "Different (Rails): " << std::endl;
        DiffByamlIteratorByParser(std::addressof(l_rail_iter), std::addressof(r_rail_iter), "Hash", "Name", nullptr, indent_level + 2, true);
    }

    /* Get AiGroup iterator */
    vp::res::ByamlIterator l_ai_group_iter;
    vp::res::ByamlIterator r_ai_group_iter;
    const bool l_ai_group_result = l_root_iter.TryGetIteratorByKey(std::addressof(l_ai_group_iter), "AiGroups");
    const bool r_ai_group_result = r_root_iter.TryGetIteratorByKey(std::addressof(r_ai_group_iter), "AiGroups");
    if (l_ai_group_result != r_ai_group_result && is_print == false) { return false; }

    /* Diff AiGroups */
    const bool is_ai_group_same = DiffByamlIteratorByParser(std::addressof(l_ai_group_iter), std::addressof(r_ai_group_iter), "Hash", "Logic", nullptr, indent_level + 1, false);
    if (is_ai_group_same == false) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level + 1);
        std::cout << "Different (AiGroups): " << std::endl;
        DiffByamlIteratorByParser(std::addressof(l_ai_group_iter), std::addressof(r_ai_group_iter), "Hash", "Logic", nullptr, indent_level + 2, true);
    }

    /* Get Graphs iterator */
    //vp::res::ByamlIterator l_graph_iter;
    //vp::res::ByamlIterator r_graph_iter;
    //const bool l_graph_result = l_root_iter.TryGetIteratorByKey(std::addressof(l_graph_iter), "AiGroups");
    //const bool r_graph_result = r_root_iter.TryGetIteratorByKey(std::addressof(r_graph_iter), "AiGroups");
    //if (l_graph_result != r_graph_result && is_print == false) { return false; }

    /* Diff Graphs */
    //const bool is_graph_same = DiffByamlIteratorByParser(std::addressof(l_graph_iter), std::addressof(r_graph_iter), "InstanceID", "Name", indent_level + 1, false);
    //if (is_graph_same == false) {
    //    if (is_print == false) { return false; }
    //
    //    PrintIndent(indent_level + 1);
    //    std::cout << "Different (Graphs): " << std::endl;
    //    DiffByamlIteratorByParser(std::addressof(l_graph_iter), std::addressof(r_graph_iter), "InstanceID", "Name", indent_level + 2, true);
    //}

    return true;
}

bool DiffRstbl(dd::util::FixedString<dd::util::MaxPath> *file_name, void *left_file, void *right_file, u32 indent_level, bool is_print) {
    
    switch (GetByamlRstblFileVariationByName(file_name)) {
        case ByamlRstblFileVariation::RowIdDictionary:
        {
            vp::res::ByamlIterator l_iter(left_file);
            vp::res::ByamlIterator r_iter(right_file);
            const bool is_same = DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "__RowId", "__RowId", nullptr, indent_level + 1, false);
            if (is_same == false) {
                if (is_print == false) { return false; }
                DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "__RowId", "__RowId", nullptr, indent_level + 1, true);
            }
            break;
        }
        case ByamlRstblFileVariation::IdDictionary:
        {
            vp::res::ByamlIterator l_iter(left_file);
            vp::res::ByamlIterator r_iter(right_file);
            const bool is_same = DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Id", "Id", nullptr, indent_level + 1, false);
            if (is_same == false) {
                if (is_print == false) { return false; }
                DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Id", "Id", nullptr, indent_level + 1, true);
            }
            break;
        }
        case ByamlRstblFileVariation::TypeDictionary:
        {
            vp::res::ByamlIterator l_iter(left_file);
            vp::res::ByamlIterator r_iter(right_file);
            const bool is_same = DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Type", "Type", nullptr, indent_level + 1, false);
            if (is_same == false) {
                if (is_print == false) { return false; }
                DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Type", "Type", nullptr, indent_level + 1, true);
            }
            break;
        }
        case ByamlRstblFileVariation::NameDictionary:
        {
            vp::res::ByamlIterator l_iter(left_file);
            vp::res::ByamlIterator r_iter(right_file);
            const bool is_same = DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Name", "Name", nullptr, indent_level + 1, false);
            if (is_same == false) {
                if (is_print == false) { return false; }
                DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Name", "Name", nullptr, indent_level + 1, true);
            }
            break;
        }
        default:
            /* Default to trying for RowId */
            vp::res::ByamlIterator l_iter(left_file);
            vp::res::ByamlIterator r_iter(right_file);
            const bool is_same = DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "__RowId", "__RowId", nullptr, indent_level + 1, false);
            if (is_same == false) {
                if (is_print == false) { return false; }
                PrintIndent(indent_level + 1);
                //std::cout << "Unknown __RowId based RSDB byaml detected!" << std::endl;
                DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "__RowId", "__RowId", nullptr, indent_level + 1, true);
            } else if (DiffByamlFull(left_file, right_file, indent_level, false) == false) {
                if (is_print == false) { return false; }
                PrintIndent(indent_level + 1);
                DiffByamlFull(std::addressof(l_iter), std::addressof(r_iter), indent_level + 1, true);
            }
            break;
    }
    return true;
}

bool DiffGenericByamlByFileName(dd::util::FixedString<dd::util::MaxPath> *file_name, void *left_file, void *right_file,  u32 indent_level, bool is_print) {

   switch(GetByamlGenericFileVariationByName(file_name)) {
        case ByamlGenericFileVariation::PhiveConfig:
        case ByamlGenericFileVariation::EffectFileInfo:
        case ByamlGenericFileVariation::BarslistPack:
        {
            DiffByamlFull(left_file, right_file, indent_level + 1, is_print);
            break;
        }
        case ByamlGenericFileVariation::Bkdat:
            DiffBkres(left_file, right_file, indent_level + 1, is_print);
            break;
        default:
            DiffByamlFull(left_file, right_file, indent_level + 1, is_print);
            break;
    }
    return false;
}
