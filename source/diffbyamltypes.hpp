#pragma once

bool DiffByamlIteratorByParser(dd::res::ByamlIterator *l_root_iter, dd::res::ByamlIterator *r_root_iter, const char *identifier_key, const char *array_name_key, u32 indent_level, bool is_print) {

    ByamlDictionaryParser l_parser = {};
    ByamlDictionaryParser r_parser = {};
    const bool l_result = l_parser.Initialize(l_root_iter, identifier_key);
    const bool r_result = r_parser.Initialize(r_root_iter, identifier_key);

    if (l_result != r_result && is_print == false) { return false; }

    if (l_result == false && r_result == false) { return true; }
    if (l_result == false && r_result == true) {
        r_parser.ProcessAllByamlDataSingle(array_name_key, indent_level, PrintSide::Right);
        return true;
    }
    if (l_result == true && r_result == false) {
        l_parser.ProcessAllByamlDataSingle(array_name_key, indent_level, PrintSide::Left);
        return true;
    }

    const bool is_same = l_parser.DiffAllByamlData(std::addressof(r_parser), array_name_key, indent_level, false);
    if (is_same == false && is_print == false) { return false; }
    l_parser.DiffAllByamlData(std::addressof(r_parser), array_name_key, indent_level, true);
    r_parser.ProcessAllByamlDataOnly(std::addressof(l_parser), array_name_key, indent_level, PrintSide::Right);
    l_parser.ProcessAllByamlDataOnly(std::addressof(r_parser), array_name_key, indent_level, PrintSide::Left);
    return true;
}

bool DiffBkres([[maybe_unused]] void *left_file, [[maybe_unused]] void *right_file, [[maybe_unused]] u32 indent_level, [[maybe_unused]] bool is_print) {

    /* Initialize iterator */
    dd::res::ByamlIterator l_root_iter(left_file);
    dd::res::ByamlIterator r_root_iter(right_file);
    
    std::cout << "Bkres Detection succeeded";

    return true;
}

bool DiffBgyml(void *left_file, void *right_file, u32 indent_level, bool is_print) {
    /* Initialize iterator */
    dd::res::ByamlIterator l_root_iter(left_file);
    dd::res::ByamlIterator r_root_iter(right_file);
    
    const s32 result = CompareByamlIterator(std::addressof(l_root_iter), std::addressof(r_root_iter));
    if (result == 0) { return true; }
    if (is_print == false) {return false; } 

    DiffByamlIterator(std::addressof(l_root_iter), std::addressof(r_root_iter), indent_level);
    
    return false;
}

bool DiffBcett(void *left_file, void *right_file, u32 indent_level, bool is_print) {

    /* Initialize iterator */
    dd::res::ByamlIterator l_root_iter(left_file);
    dd::res::ByamlIterator r_root_iter(right_file);

    /* Get Actor iterator */
    dd::res::ByamlIterator l_actor_iter;
    dd::res::ByamlIterator r_actor_iter;
    const bool l_actor_result = l_root_iter.TryGetIteratorByKey(std::addressof(l_actor_iter), "Actors");
    const bool r_actor_result = r_root_iter.TryGetIteratorByKey(std::addressof(r_actor_iter), "Actors");
    if (l_actor_result != r_actor_result && is_print == false) { return false; }

    /* Diff Actors */
    const bool is_actor_same = DiffByamlIteratorByParser(std::addressof(l_actor_iter), std::addressof(r_actor_iter), "InstanceID", "Name", indent_level + 1, false);
    if (is_actor_same == false) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level + 1);
        std::cout << "Different (Actors): " << std::endl;
        DiffByamlIteratorByParser(std::addressof(l_actor_iter), std::addressof(r_actor_iter), "InstanceID", "Name", indent_level + 2, true);
    }

    /* Get Rail iterator */
    dd::res::ByamlIterator l_rail_iter;
    dd::res::ByamlIterator r_rail_iter;
    const bool l_rail_result = l_root_iter.TryGetIteratorByKey(std::addressof(l_rail_iter), "Rails");
    const bool r_rail_result = r_root_iter.TryGetIteratorByKey(std::addressof(r_rail_iter), "Rails");
    if (l_rail_result != r_rail_result && is_print == false) { return false; }

    /* Diff Rails */
    const bool is_rail_same = DiffByamlIteratorByParser(std::addressof(l_rail_iter), std::addressof(r_rail_iter), "InstanceID", "Name", indent_level + 1, false);
    if (is_rail_same == false) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level + 1);
        std::cout << "Different (Rails): " << std::endl;
        DiffByamlIteratorByParser(std::addressof(l_rail_iter), std::addressof(r_rail_iter), "InstanceID", "Name", indent_level + 2, true);
    }

    /* Get AiGroup iterator */
    dd::res::ByamlIterator l_ai_group_iter;
    dd::res::ByamlIterator r_ai_group_iter;
    const bool l_ai_group_result = l_root_iter.TryGetIteratorByKey(std::addressof(l_ai_group_iter), "AiGroups");
    const bool r_ai_group_result = r_root_iter.TryGetIteratorByKey(std::addressof(r_ai_group_iter), "AiGroups");
    if (l_ai_group_result != r_ai_group_result && is_print == false) { return false; }

    /* Diff AiGroups */
    const bool is_ai_group_same = DiffByamlIteratorByParser(std::addressof(l_ai_group_iter), std::addressof(r_ai_group_iter), "Logic", "Logic", indent_level + 1, false);
    if (is_ai_group_same == false) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level + 1);
        std::cout << "Different (AiGroups): " << std::endl;
        DiffByamlIteratorByParser(std::addressof(l_ai_group_iter), std::addressof(r_ai_group_iter), "Logic", "Logic", indent_level + 2, true);
    }

    /* Get Graphs iterator */
    dd::res::ByamlIterator l_graph_iter;
    dd::res::ByamlIterator r_graph_iter;
    const bool l_graph_result = l_root_iter.TryGetIteratorByKey(std::addressof(l_graph_iter), "AiGroups");
    const bool r_graph_result = r_root_iter.TryGetIteratorByKey(std::addressof(r_graph_iter), "AiGroups");
    if (l_graph_result != r_graph_result && is_print == false) { return false; }

    /* Diff Graphs */
    const bool is_graph_same = DiffByamlIteratorByParser(std::addressof(l_graph_iter), std::addressof(r_graph_iter), "InstanceID", "Name", indent_level + 1, false);
    if (is_graph_same == false) {
        if (is_print == false) { return false; }

        PrintIndent(indent_level + 1);
        std::cout << "Different (Graphs): " << std::endl;
        DiffByamlIteratorByParser(std::addressof(l_graph_iter), std::addressof(r_graph_iter), "InstanceID", "Name", indent_level + 2, true);
    }

    return true;
}

bool DiffRstbl(dd::util::FixedString<dd::util::MaxPath> *file_name, void *left_file, void *right_file, u32 indent_level, bool is_print) {
    
    switch (GetByamlRstblFileVariationByName(file_name)) {
        case ByamlRstblFileVariation::RowIdDictionary:
        {
            dd::res::ByamlIterator l_iter(left_file);
            dd::res::ByamlIterator r_iter(right_file);
            const bool is_same = DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "__RowId", "__RowId", indent_level + 1, false);
            if (is_same == false) {
                if (is_print == false) { return false; }
                DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "__RowId", "__RowId", indent_level + 1, true);
            }
            break;
        }
        case ByamlRstblFileVariation::IdDictionary:
        {
            dd::res::ByamlIterator l_iter(left_file);
            dd::res::ByamlIterator r_iter(right_file);
            const bool is_same = DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Id", "Id", indent_level + 1, false);
            if (is_same == false) {
                if (is_print == false) { return false; }
                DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Id", "Id", indent_level + 1, true);
            }
            break;
        }
        case ByamlRstblFileVariation::TypeDictionary:
        {
            dd::res::ByamlIterator l_iter(left_file);
            dd::res::ByamlIterator r_iter(right_file);
            const bool is_same = DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Type", "Type", indent_level + 1, false);
            if (is_same == false) {
                if (is_print == false) { return false; }
                DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Type", "Type", indent_level + 1, true);
            }
            break;
        }
        case ByamlRstblFileVariation::NameDictionary:
        {
            dd::res::ByamlIterator l_iter(left_file);
            dd::res::ByamlIterator r_iter(right_file);
            const bool is_same = DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Name", "Name", indent_level + 1, false);
            if (is_same == false) {
                if (is_print == false) { return false; }
                DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "Name", "Name", indent_level + 1, true);
            }
            break;
        }
        default:
            /* Default to trying for RowId */
            dd::res::ByamlIterator l_iter(left_file);
            dd::res::ByamlIterator r_iter(right_file);
            const bool is_same = DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "__RowId", "__RowId", indent_level + 1, false);
            if (is_same == false) {
                if (is_print == false) { return false; }
                PrintIndent(indent_level + 1);
                std::cout << "Unknown __RowId based RSDB byaml detected!" << std::endl;
                DiffByamlIteratorByParser(std::addressof(l_iter), std::addressof(r_iter), "__RowId", "__RowId", indent_level + 1, true);
            }
            break;
    }
    return true;
}

bool DiffGenericByamlByFileName(dd::util::FixedString<dd::util::MaxPath> *file_name, [[maybe_unused]] void *left_file, [[maybe_unused]] void *right_file, [[maybe_unused]] u32 indent_level, [[maybe_unused]] bool is_print) {

   switch(GetByamlGenericFileVariationByName(file_name)) {
        case ByamlGenericFileVariation::PhiveConfig:
        case ByamlGenericFileVariation::EffectFileInfo:
        case ByamlGenericFileVariation::BarslistPack:
        case ByamlGenericFileVariation::Bkdat:
            DiffBkres(left_file, right_file, indent_level, is_print);
            break;
        default:
            std::cout << "Failed to detect byaml type by file name" << std::endl;
            break;
    }
    return false;
}
