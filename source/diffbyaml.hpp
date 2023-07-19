#pragma once

int CompareByamlData(vp::res::ByamlIterator *l_iterator, vp::res::ByamlIterator *r_iterator, vp::res::ByamlData lhs, vp::res::ByamlData rhs) {

    const u8 left_type  = lhs.data_type;
    const u8 right_type = rhs.data_type;
    if (right_type < left_type) { return 1; }
    if (left_type < right_type) { return -1; }

    switch (static_cast<vp::res::ByamlDataType>(left_type)) {
        case vp::res::ByamlDataType::StringIndex:
        {
            const char *left  = nullptr;
            const char *right = nullptr;
            l_iterator->TryGetStringByData(std::addressof(left), lhs);
            r_iterator->TryGetStringByData(std::addressof(right), rhs);
            return ::strcmp(left, right);
        }
        case vp::res::ByamlDataType::BinaryData:
        case vp::res::ByamlDataType::BinaryDataWithAlignment:
        {
            void *left  = nullptr;
            void *right = nullptr;
            u32 left_size  = 0;
            u32 right_size = 0;
            u32 left_alignment = 0;
            u32 right_alignment = 0;
            l_iterator->TryGetBinaryDataByData(std::addressof(left), std::addressof(left_size), std::addressof(left_alignment), lhs);
            r_iterator->TryGetBinaryDataByData(std::addressof(right), std::addressof(right_size), std::addressof(right_alignment), rhs);
            if (right_size < left_size) { return 1; }
            if (left_size < right_size) { return -1; }
            return ::memcmp(left, right, left_size);
        }
        case vp::res::ByamlDataType::MonoTypedArray:
        case vp::res::ByamlDataType::Array:
        {
            vp::res::ByamlIterator left;
            vp::res::ByamlIterator right;
            l_iterator->TryGetIteratorByData(std::addressof(left), lhs);
            r_iterator->TryGetIteratorByData(std::addressof(right), rhs);

            if (right.GetDataCount() < left.GetDataCount()) { return 1; }
            if (left.GetDataCount() < right.GetDataCount()) { return -1; }

            for (u32 i = 0; i < left.GetDataCount(); ++i) {
                vp::res::ByamlData left_data;
                vp::res::ByamlData right_data;
                left.TryGetByamlDataByIndex(std::addressof(left_data), i);
                right.TryGetByamlDataByIndex(std::addressof(right_data), i);
                const s32 compare_result = CompareByamlData(std::addressof(left), std::addressof(right), left_data, right_data);
                if (compare_result != 0) { return compare_result; }
            }

            return 0;
        }
        case vp::res::ByamlDataType::Dictionary:
        case vp::res::ByamlDataType::DictionaryWithRemap:
        {
            vp::res::ByamlIterator left;
            vp::res::ByamlIterator right;
            l_iterator->TryGetIteratorByData(std::addressof(left), lhs);
            r_iterator->TryGetIteratorByData(std::addressof(right), rhs);

            if (right.GetDataCount() < left.GetDataCount()) { return 1; }
            if (left.GetDataCount() < right.GetDataCount()) { return -1; }

            for (u32 i = 0; i < left.GetDataCount(); ++i) {
                vp::res::ByamlData left_data;
                vp::res::ByamlData right_data;
                left.TryGetByamlDataByIndex(std::addressof(left_data), i);
                const char *left_key = nullptr;
                left.TryGetKeyByData(std::addressof(left_key), left_data);
                const bool get_result = right.TryGetByamlDataByKey(std::addressof(right_data), left_key);
                if (get_result == false) { return 1; }
                const s32 compare_result = CompareByamlData(std::addressof(left), std::addressof(right), left_data, right_data);
                if (compare_result != 0) { return compare_result; }
            }

            return 0;
        }
        case vp::res::ByamlDataType::KeyTable:
            return 0;
        case vp::res::ByamlDataType::Bool:
        {
            bool left  = 0;
            bool right = 0;
            l_iterator->TryGetBoolByData(std::addressof(left), lhs);
            r_iterator->TryGetBoolByData(std::addressof(right), rhs);
            if (right < left)  { return 1; }
            if (left  < right) { return -1; }
            return 0;
        }
        case vp::res::ByamlDataType::S32:
        {
            s32 left  = 0;
            s32 right = 0;
            l_iterator->TryGetS32ByData(std::addressof(left), lhs);
            r_iterator->TryGetS32ByData(std::addressof(right), rhs);
            if (right < left)  { return 1; }
            if (left  < right) { return -1; }
            return 0;
        }
        case vp::res::ByamlDataType::F32:
        {
            float left  = 0.0;
            float right = 0.0;
            l_iterator->TryGetF32ByData(std::addressof(left), lhs);
            r_iterator->TryGetF32ByData(std::addressof(right), rhs);
            if (right < left)  { return 1; }
            if (left  < right) { return -1; }
            return 0;
        }
        case vp::res::ByamlDataType::U32:
        {
            u32 left  = 0;
            u32 right = 0;
            l_iterator->TryGetU32ByData(std::addressof(left), lhs);
            r_iterator->TryGetU32ByData(std::addressof(right), rhs);
            if (right < left)  { return 1; }
            if (left  < right) { return -1; }
            return 0;
        }
        case vp::res::ByamlDataType::S64:
        {
            s64 left  = 0;
            s64 right = 0;
            l_iterator->TryGetS64ByData(std::addressof(left), lhs);
            r_iterator->TryGetS64ByData(std::addressof(right), rhs);
            if (right < left)  { return 1; }
            if (left  < right) { return -1; }
            return 0;
        }
        case vp::res::ByamlDataType::U64:
        {
            u64 left  = 0;
            u64 right = 0;
            l_iterator->TryGetU64ByData(std::addressof(left), lhs);
            r_iterator->TryGetU64ByData(std::addressof(right), rhs);
            if (right < left)  { return 1; }
            if (left  < right) { return -1; }
            return 0;
        }
        case vp::res::ByamlDataType::F64:
        {
            double left  = 0.0;
            double right = 0.0;
            l_iterator->TryGetF64ByData(std::addressof(left), lhs);
            r_iterator->TryGetF64ByData(std::addressof(right), rhs);
            if (right < left)  { return 1; }
            if (left  < right) { return -1; }
            return 0;
        }
        default:
        case vp::res::ByamlDataType::Null:
        break;
    }
    return 0;
}

int CompareByamlIterator(vp::res::ByamlIterator *l_iterator, vp::res::ByamlIterator *r_iterator) {

    const bool l_is_valid = l_iterator->IsValid();
    const bool r_is_valid = r_iterator->IsValid();
    if (r_is_valid < l_is_valid) { return 1; }
    if (l_is_valid < r_is_valid) { return -1; }
    if (l_is_valid == false && r_is_valid == false) { return 0; }

    const u32 left_count  = l_iterator->GetDataCount();
    const u32 right_count = r_iterator->GetDataCount();
    if (right_count < left_count) { return 1; }
    if (left_count < right_count) { return -1; }

    const u8 left_type  = l_iterator->GetDataType();
    const u8 right_type = r_iterator->GetDataType();
    if (right_type < left_type) { return 1; }
    if (left_type < right_type) { return -1; }

    if (static_cast<vp::res::ByamlDataType>(left_type) == vp::res::ByamlDataType::Array || static_cast<vp::res::ByamlDataType>(left_type) == vp::res::ByamlDataType::MonoTypedArray) {

        for (u32 i = 0; i < left_count; ++i) {
            vp::res::ByamlData left_data;
            vp::res::ByamlData right_data;
            l_iterator->TryGetByamlDataByIndex(std::addressof(left_data), i);
            r_iterator->TryGetByamlDataByIndex(std::addressof(right_data), i);
            const bool compare_result = CompareByamlData(l_iterator, r_iterator, left_data, right_data);
            if (compare_result != 0) { return compare_result; }
        }
        return 0;
    }
    if (static_cast<vp::res::ByamlDataType>(left_type) != vp::res::ByamlDataType::Dictionary && static_cast<vp::res::ByamlDataType>(left_type) != vp::res::ByamlDataType::DictionaryWithRemap) { return 0; }

    for (u32 i = 0; i < left_count; ++i) {
        vp::res::ByamlData left_data;
        vp::res::ByamlData right_data;
        l_iterator->TryGetByamlDataByIndex(std::addressof(left_data), i);
        const char *left_key = nullptr;
        l_iterator->TryGetKeyByData(std::addressof(left_key), left_data);
        const bool get_result = r_iterator->TryGetByamlDataByKey(std::addressof(right_data), left_key);
        if (get_result == false) { return 1; }
        const bool compare_result = CompareByamlData(l_iterator, r_iterator, left_data, right_data);
        if (compare_result != 0) { return compare_result; }
    }
    return 0;
}

void PrintByamlDataKey(vp::res::ByamlIterator *iterator, vp::res::ByamlData data, u32 array_index) {

    const char *key = nullptr;
    const bool key_result = iterator->TryGetKeyByData(std::addressof(key), data);
    if (key_result == false) {
        std::cout << std::dec << "<Array> " << array_index << " ";
    } else {
        std::cout << "\"" << key << "\"";
    }
}

void PrintByamlDataSingle(vp::res::ByamlIterator *iterator, vp::res::ByamlData data, u32 array_index, const char *array_name_key, u32 indent_level, PrintSide print_side) {

    PrintIndent(indent_level);
    PrintOnlySide(print_side);
    std::cout << "(key: ";
    PrintByamlDataKey(iterator, data, array_index);
    std::cout << ")(DataType: " << vp::res::ByamlDataTypeToString(static_cast<vp::res::ByamlDataType>(data.data_type)) << "): ";

    switch (static_cast<vp::res::ByamlDataType>(data.data_type)) {
        case vp::res::ByamlDataType::StringIndex:
        {
            const char *value  = nullptr;
            iterator->TryGetStringByData(std::addressof(value), data);
            std::cout << "\"" << value << "\"" << std::endl;
        }
        break;
        case vp::res::ByamlDataType::BinaryData:
        case vp::res::ByamlDataType::BinaryDataWithAlignment:
        {
            std::cout << "(BinaryData):" << std::endl;
            void *binary          = nullptr;
            u32 binary_size       = 0;
            u32 binary_alignment  = 0;
            iterator->TryGetBinaryDataByData(std::addressof(binary), std::addressof(binary_size), std::addressof(binary_alignment), data);
            const char *path = "(Unnamed Binary Data)";
            iterator->TryGetKeyByData(std::addressof(path), data);
            ProcessSingleImpl(binary, binary_size, path, indent_level + 1, print_side);
        }
        break;
        case vp::res::ByamlDataType::Array:
        {
            vp::res::ByamlIterator iter;
            iterator->TryGetIteratorByIndex(std::addressof(iter), array_index);

            const char *name = "";
            const bool name_result = (array_name_key == nullptr) ? false : iter.TryGetStringByKey(std::addressof(name), array_name_key);

            if (name_result == true) {
                std::cout << "(Array)(" << array_name_key << ": \"" << name << "\")" << std::endl;
            } else {
                std::cout << "(Array)" << std::endl;
            }

            for (u32 i = 0; i < iter.GetDataCount(); ++i) {
                vp::res::ByamlData value;
                iter.TryGetByamlDataByIndex(std::addressof(value), i);
                PrintByamlDataSingle(std::addressof(iter), value, i, nullptr, indent_level + 1, print_side);
            }
        }
        break;
        case vp::res::ByamlDataType::Dictionary:
        {
            vp::res::ByamlIterator iter;
            iterator->TryGetIteratorByIndex(std::addressof(iter), array_index);

            const char *name = "";
            const bool name_result = (array_name_key == nullptr) ? false : iter.TryGetStringByKey(std::addressof(name), array_name_key);

            if (name_result == true) {
                std::cout << "(Dictionary)(" << array_name_key << ": \"" << name << "\")" << std::endl;
            } else {
                std::cout << "(Dictionary)" << std::endl;
            }

            for (u32 i = 0; i < iter.GetDataCount(); ++i) {
                vp::res::ByamlData value;
                iter.TryGetByamlDataByIndex(std::addressof(value), i);
                PrintByamlDataSingle(std::addressof(iter), value, i, nullptr, indent_level + 1, print_side);
            }
        }
        break;
        case vp::res::ByamlDataType::KeyTable:
            return;
        case vp::res::ByamlDataType::Bool:
        {
            bool value  = 0;
            iterator->TryGetBoolByData(std::addressof(value), data);
            std::cout << value << std::endl;
        }
        break;
        case vp::res::ByamlDataType::S32:
        {
            s32 value  = 0;
            iterator->TryGetS32ByData(std::addressof(value), data);
            std::cout << std::dec << value << std::endl;
        }
        break;
        case vp::res::ByamlDataType::F32:
        {
            float value = 0.0;
            iterator->TryGetF32ByData(std::addressof(value), data);
            std::cout << std::fixed << value << std::endl;
        }
        break;
        case vp::res::ByamlDataType::U32:
        {
            u32 value = 0;
            iterator->TryGetU32ByData(std::addressof(value), data);
            std::cout << std::dec << value << std::endl;
        }
        break;
        case vp::res::ByamlDataType::S64:
        {
            s64 value = 0;
            iterator->TryGetS64ByData(std::addressof(value), data);
            std::cout << std::dec << value << std::endl;
        }
        break;
        case vp::res::ByamlDataType::U64:
        {
            u64 value = 0;
            iterator->TryGetU64ByData(std::addressof(value), data);
            std::cout << std::dec << value << std::endl;
        }
        break;
        case vp::res::ByamlDataType::F64:
        {
            double value = 0.0;
            iterator->TryGetF64ByData(std::addressof(value), data);
            std::cout << std::fixed << value << std::endl;
        }
        break;
        default:
        case vp::res::ByamlDataType::Null:
            std::cout  << "Null" << std::endl;
        break;
    }
}

void PrintByamlIteratorSingle(vp::res::ByamlIterator *iterator, [[maybe_unused]] u32 array_index, u32 indent_level, PrintSide print_side) {

    for (u32 i = 0; i < iterator->GetDataCount(); ++i) {
        vp::res::ByamlData data;
        iterator->TryGetByamlDataByIndex(std::addressof(data), i);
        PrintByamlDataSingle(iterator, data, i, "", indent_level, print_side);
    }
}

void PrintOnlyByamlData(vp::res::ByamlIterator *iterator, vp::res::ByamlData data) {

    switch (static_cast<vp::res::ByamlDataType>(data.data_type)) {
        case vp::res::ByamlDataType::StringIndex:
        {
            const char *value  = nullptr;
            iterator->TryGetStringByData(std::addressof(value), data);
            std::cout << "\"" << value << "\"";
        }
        break;
        case vp::res::ByamlDataType::BinaryData:
        case vp::res::ByamlDataType::BinaryDataWithAlignment:
        case vp::res::ByamlDataType::Array:
        case vp::res::ByamlDataType::Dictionary:
        case vp::res::ByamlDataType::KeyTable:
        default:
            std::cout << "NotSupported";
        break;
        case vp::res::ByamlDataType::Bool:
        {
            bool value  = 0;
            iterator->TryGetBoolByData(std::addressof(value), data);
            std::cout << value;
        }
        break;
        case vp::res::ByamlDataType::S32:
        {
            s32 value  = 0;
            iterator->TryGetS32ByData(std::addressof(value), data);
            std::cout << std::dec << value;
        }
        break;
        case vp::res::ByamlDataType::F32:
        {
            float value = 0.0;
            iterator->TryGetF32ByData(std::addressof(value), data);
            std::cout << std::fixed << value;
        }
        break;
        case vp::res::ByamlDataType::U32:
        {
            u32 value = 0;
            iterator->TryGetU32ByData(std::addressof(value), data);
            std::cout << std::dec << value;
        }
        break;
        case vp::res::ByamlDataType::S64:
        {
            s64 value = 0;
            iterator->TryGetS64ByData(std::addressof(value), data);
            std::cout << std::dec << value;
        }
        break;
        case vp::res::ByamlDataType::U64:
        {
            u64 value = 0;
            iterator->TryGetU64ByData(std::addressof(value), data);
            std::cout << std::dec << value;
        }
        break;
        case vp::res::ByamlDataType::F64:
        {
            double value = 0.0;
            iterator->TryGetF64ByData(std::addressof(value), data);
            std::cout << std::fixed << value;
        }
        break;
        case vp::res::ByamlDataType::Null:
            std::cout << "Null";
        break;
    }
    return;
}

void PrintByamlDataNames(vp::res::ByamlIterator *l_iterator, vp::res::ByamlIterator *r_iterator, vp::res::ByamlData lhs, vp::res::ByamlData rhs, u32 l_array_index, u32 r_array_index) {

    /* Handle <array> */
    const char *l_name = nullptr;
    const char *r_name = nullptr;
    const bool l_result = l_iterator->TryGetKeyByData(std::addressof(l_name), lhs);
    const bool r_result = r_iterator->TryGetKeyByData(std::addressof(r_name), rhs);

    if (l_result != r_result) {
        if (l_result == true) {
            std::cout  << std::dec << "(left key: \"" << l_name << "\")(right key: <Array> " << r_array_index << ")";
        } else {
            std::cout << std::dec << "(left key: <Array> " << l_array_index << ")(right key: \"" << r_name << "\")";
        }
    } else {
        if (l_result == true) {
            if (::strcmp(l_name, r_name) == 0) {
                std::cout << "(key: \"" << l_name << "\")";
            } else {
                std::cout << "(left key: \"" << l_name << "\")(right key: \"" << r_name << "\")";
            }
        } else {
            if (l_array_index == r_array_index) {
                std::cout  << std::dec << "(key: <Array> " << l_array_index << ")";
            } else {
                std::cout << std::dec << "(left key: <Array> " << l_array_index << ")(right key: <Array> " << r_array_index << ")";
            }
        }
    }

    return;
}


using ByamlDataOverrideCallback = bool (*) (vp::res::ByamlIterator *l_iterator, vp::res::ByamlIterator *r_iterator, vp::res::ByamlData lhs, vp::res::ByamlData rhs, u32 index);
void DiffByamlData(vp::res::ByamlIterator *l_iterator, vp::res::ByamlIterator *r_iterator, vp::res::ByamlData lhs, vp::res::ByamlData rhs, u32 l_array_index, u32 r_array_index, const char *array_name_key, ByamlDataOverrideCallback override_callback, u32 indent_level) {

    const u8 left_type  = lhs.data_type;
    const u8 right_type = rhs.data_type;

    if (right_type != left_type) {
        PrintIndent(indent_level);
        std::cout << "DataType Change";
        PrintByamlDataNames(l_iterator, r_iterator, lhs, rhs, l_array_index, r_array_index);
        std::cout << ": (left: " << vp::res::ByamlDataTypeToString(static_cast<vp::res::ByamlDataType>(left_type)) << ")(right: " << vp::res::ByamlDataTypeToString(static_cast<vp::res::ByamlDataType>(right_type)) << ")" << std::endl;
        return;
    }

    if (override_callback != nullptr) {
        const bool result = (override_callback)(l_iterator, r_iterator, lhs, rhs, indent_level);
        if (result == true) { return; }
    }

    const s32 compare_result0 = CompareByamlData(l_iterator, r_iterator, lhs, rhs);
    if (compare_result0 == 0) { return; }

    switch (static_cast<vp::res::ByamlDataType>(left_type)) {
        case vp::res::ByamlDataType::BinaryData:
        case vp::res::ByamlDataType::BinaryDataWithAlignment:
        {
            void *left  = nullptr;
            void *right = nullptr;
            u32 left_size  = 0;
            u32 right_size = 0;
            u32 left_alignment  = 0;
            u32 right_alignment = 0;
            l_iterator->TryGetBinaryDataByData(std::addressof(left), std::addressof(left_size), std::addressof(left_alignment), lhs);
            r_iterator->TryGetBinaryDataByData(std::addressof(right), std::addressof(right_size), std::addressof(right_alignment), rhs);
            
            if (left_size != right_size || ::memcmp(left, right, left_size) != 0) {
                PrintIndent(indent_level);
                std::cout << "Different";
                PrintByamlDataNames(l_iterator, r_iterator, lhs, rhs, l_array_index, r_array_index);
                std::cout << ": (BinaryData)" << std::endl;
                const char *path = "(Unnamed Binary Data)";
                r_iterator->TryGetKeyByData(std::addressof(path), rhs);
                ProcessFilesImpl(left, left_size, right, right_size, path, indent_level + 1);
            }
        }
        break;
        case vp::res::ByamlDataType::MonoTypedArray:
        case vp::res::ByamlDataType::Array:
        {
            vp::res::ByamlIterator left;
            vp::res::ByamlIterator right;
            l_iterator->TryGetIteratorByData(std::addressof(left), lhs);
            r_iterator->TryGetIteratorByData(std::addressof(right), rhs);

            PrintIndent(indent_level);
            std::cout << "Different";
            PrintByamlDataNames(l_iterator, r_iterator, lhs, rhs, l_array_index, r_array_index);

            const char *l_name = "";
            const char *r_name = "";
            const bool l_name_result = (array_name_key == nullptr) ? false : left.TryGetStringByKey(std::addressof(l_name), array_name_key);
            const bool r_name_result = (array_name_key == nullptr) ? false : right.TryGetStringByKey(std::addressof(r_name), array_name_key);

            if (l_name_result == true || r_name_result == true) {
                if (::strcmp(l_name, r_name) == 0) {
                    std::cout << ": (Array)(" << array_name_key << ": \"" << l_name << "\")" << std::endl;
                } else {
                    std::cout << ": (Array)(left " << array_name_key << ": \"" << l_name << "\")(right " << array_name_key << ": \"" << r_name << "\")" << std::endl;
                }
            } else {
                std::cout << ": (Array)" << std::endl;
            }

            for (u32 i = 0; i < left.GetDataCount(); ++i) {

                /* Get Byaml Data */
                vp::res::ByamlData left_data;
                vp::res::ByamlData right_data;
                const bool result0 = left.TryGetByamlDataByIndex(std::addressof(left_data), i);
                if (result0 == false) { continue; }
                const bool result1 = right.TryGetByamlDataByIndex(std::addressof(right_data), i);
                if (result1 == false) { continue; }

                /* Diff data */
                const s32 compare_result1 = CompareByamlData(std::addressof(left), std::addressof(right), left_data, right_data);
                if (compare_result1 == 0) { continue; }
                DiffByamlData(std::addressof(left), std::addressof(right), left_data, right_data, i, i, nullptr, override_callback, indent_level + 1);
            }

            for (u32 i = left.GetDataCount(); i < right.GetDataCount(); ++i) {

                /* Get Byaml Data */
                vp::res::ByamlData data;
                right.TryGetByamlDataByIndex(std::addressof(data), i);

                PrintByamlDataSingle(std::addressof(right), data, i, nullptr, indent_level + 1, PrintSide::Right);
            }

            for (u32 i = right.GetDataCount(); i < left.GetDataCount(); ++i) {

                /* Get Byaml Data */
                vp::res::ByamlData data;
                left.TryGetByamlDataByIndex(std::addressof(data), i);

                PrintByamlDataSingle(std::addressof(left), data, i, nullptr, indent_level + 1, PrintSide::Left);
            }
        }
        break;
        case vp::res::ByamlDataType::DictionaryWithRemap:
        case vp::res::ByamlDataType::Dictionary:
        {
            vp::res::ByamlIterator left;
            vp::res::ByamlIterator right;
            l_iterator->TryGetIteratorByData(std::addressof(left), lhs);
            r_iterator->TryGetIteratorByData(std::addressof(right), rhs);

            PrintIndent(indent_level);
            std::cout << "Different";
            PrintByamlDataNames(l_iterator, r_iterator, lhs, rhs, l_array_index, r_array_index);

            const char *l_name = "";
            const char *r_name = "";
            const bool l_name_result = (array_name_key == nullptr) ? false : left.TryGetStringByKey(std::addressof(l_name), array_name_key);
            const bool r_name_result = (array_name_key == nullptr) ? false : right.TryGetStringByKey(std::addressof(r_name), array_name_key);

            if (l_name_result == true || r_name_result == true) {
                if (::strcmp(l_name, r_name) == 0) {
                    std::cout << ": (Dictionary)(" << array_name_key << ": \"" << l_name << "\")" << std::endl;
                } else {
                    std::cout << ": (Dictionary)(left " << array_name_key << ": \"" << l_name << "\")(right " << array_name_key << ": \"" << r_name << "\")" << std::endl;
                }
            } else {
                std::cout << ": (Dictionary)" << std::endl;
            }

            /* Both */
            for (u32 i = 0; i < left.GetDataCount(); ++i) {

                /* Get Byaml Data */
                vp::res::ByamlData left_data;
                vp::res::ByamlData right_data;
                left.TryGetByamlDataByIndex(std::addressof(left_data), i);
                const char *left_key = nullptr;
                left.TryGetKeyByData(std::addressof(left_key), left_data);
                const bool get_result = right.TryGetByamlDataByKey(std::addressof(right_data), left_key);
                if (get_result == false) { continue; }

                u32 r_index = 0;
                const bool index_result = right.TryRemapIndexByKey(std::addressof(r_index), left_key);
                if (index_result == false) { return; }

                /* Diff data */
                const s32 compare_result1 = CompareByamlData(std::addressof(left), std::addressof(right), left_data, right_data);
                if (compare_result1 == 0) { continue; }
                DiffByamlData(std::addressof(left), std::addressof(right), left_data, right_data, i, r_index, nullptr, override_callback, indent_level + 1);
            }

            /* Right */
            for (u32 i = 0; i < right.GetDataCount(); ++i) {

                /* Get Byaml Data */
                vp::res::ByamlData left_data;
                vp::res::ByamlData right_data;
                right.TryGetByamlDataByIndex(std::addressof(right_data), i);

                /* Find if the key is not found in the other byaml */
                const char *key = nullptr;
                right.TryGetKeyByData(std::addressof(key), right_data);
                const bool get_result = left.TryGetByamlDataByKey(std::addressof(left_data), key);
                if (get_result == true) { continue; }

                PrintByamlDataSingle(std::addressof(right), right_data, i, nullptr, indent_level + 1, PrintSide::Right);
            }

            /* Left */
            for (u32 i = 0; i < left.GetDataCount(); ++i) {

                /* Get Byaml Data */
                vp::res::ByamlData left_data;
                vp::res::ByamlData right_data;
                left.TryGetByamlDataByIndex(std::addressof(left_data), i);

                /* Find if the key is not found in the other byaml */
                const char *left_key = nullptr;
                left.TryGetKeyByData(std::addressof(left_key), left_data);
                const bool get_result = right.TryGetByamlDataByKey(std::addressof(right_data), left_key);
                if (get_result == true) { continue; }

                PrintByamlDataSingle(std::addressof(left), left_data, i, nullptr, indent_level + 1, PrintSide::Left);
            }
        }
        break;
        default:
        {
            PrintIndent(indent_level);
            std::cout << "Different";
            PrintByamlDataNames(l_iterator, r_iterator, lhs, rhs, l_array_index, r_array_index);
            std::cout << "(DataType: " << vp::res::ByamlDataTypeToString(static_cast<vp::res::ByamlDataType>(left_type)) << ")";
            std::cout << ": (left: ";
            PrintOnlyByamlData(l_iterator, lhs);
            std::cout << ")(right: ";
            PrintOnlyByamlData(r_iterator, rhs);
            std::cout << ")" << std::endl;
        }
        break;
    }
    return;
}

void DiffByamlIterator(vp::res::ByamlIterator *l_iterator, vp::res::ByamlIterator *r_iterator, u32 indent_level) {

    u8 l_data_type = l_iterator->GetDataType();
    u8 r_data_type = r_iterator->GetDataType();
    if (r_data_type != l_data_type) {
        PrintIndent(indent_level);
        std::cout << "DataType Change(root)";
        std::cout << ": (left: " << vp::res::ByamlDataTypeToString(static_cast<vp::res::ByamlDataType>(l_data_type)) << ")(right: " << vp::res::ByamlDataTypeToString(static_cast<vp::res::ByamlDataType>(r_data_type)) << ")" << std::endl;
        return;
    }

    if (static_cast<vp::res::ByamlDataType>(l_data_type) == vp::res::ByamlDataType::Array || static_cast<vp::res::ByamlDataType>(l_data_type) == vp::res::ByamlDataType::MonoTypedArray) {
        
        for (u32 i = 0; i < l_iterator->GetDataCount(); ++i) {

            /* Get Byaml Data */
            vp::res::ByamlData left_data;
            vp::res::ByamlData right_data;
            const bool result0 = l_iterator->TryGetByamlDataByIndex(std::addressof(left_data), i);
            const bool result1 = r_iterator->TryGetByamlDataByIndex(std::addressof(right_data), i);
            if (result0 == false || result1 == false) { continue; }

            /* Diff data */
            const s32 compare_result1 = CompareByamlData(l_iterator, r_iterator, left_data, right_data);
            if (compare_result1 == 0) { continue; }

            DiffByamlData(l_iterator, r_iterator, left_data, right_data, i, i, nullptr, nullptr, indent_level + 1);
        }

        for (u32 i = l_iterator->GetDataCount(); i < r_iterator->GetDataCount(); ++i) {

            /* Get Byaml Data */
            vp::res::ByamlData data;
            r_iterator->TryGetByamlDataByIndex(std::addressof(data), i);

            PrintByamlDataSingle(r_iterator, data, i, nullptr, indent_level + 1, PrintSide::Right);
        }

        for (u32 i = r_iterator->GetDataCount(); i < l_iterator->GetDataCount(); ++i) {

            /* Get Byaml Data */
            vp::res::ByamlData data;
            l_iterator->TryGetByamlDataByIndex(std::addressof(data), i);

            PrintByamlDataSingle(l_iterator, data, i, nullptr, indent_level + 1, PrintSide::Left);
        }
        
    } else if (static_cast<vp::res::ByamlDataType>(l_data_type) != vp::res::ByamlDataType::Dictionary && static_cast<vp::res::ByamlDataType>(l_data_type) != vp::res::ByamlDataType::DictionaryWithRemap) {
        std::cout << "Unknown root type" << std::endl;
        return;
    }

    for (u32 i = 0; i < l_iterator->GetDataCount(); ++i) {

        /* Get Byaml Data */
        vp::res::ByamlData left_data;
        vp::res::ByamlData right_data;
        const bool result0 = l_iterator->TryGetByamlDataByIndex(std::addressof(left_data), i);
        if (result0 == false) { continue; }
        const char *left_key = nullptr;
        const bool result1 = l_iterator->TryGetKeyByData(std::addressof(left_key), left_data);
        if (result1 == false) { continue; }
        const bool result2 = r_iterator->TryGetByamlDataByKey(std::addressof(right_data), left_key);
        if (result2 == false) { continue; }

        u32 r_index = 0;
        const bool index_result = r_iterator->TryRemapIndexByKey(std::addressof(r_index), left_key);
        if (index_result == false) { continue; }

        /* Diff data */
        const s32 compare_result1 = CompareByamlData(l_iterator, r_iterator, left_data, right_data);
        if (compare_result1 == 0) { continue; }
        DiffByamlData(l_iterator, r_iterator, left_data, right_data, i, r_index, nullptr, nullptr, indent_level + 1);
    }

    /* Right */
    for (u32 i = 0; i < r_iterator->GetDataCount(); ++i) {

        /* Get Byaml Data */
        vp::res::ByamlData left_data;
        vp::res::ByamlData right_data;
        r_iterator->TryGetByamlDataByIndex(std::addressof(right_data), i);

        /* Find if the key is not found in the other byaml */
        const char *key = nullptr;
        r_iterator->TryGetKeyByData(std::addressof(key), right_data);
        const bool get_result = l_iterator->TryGetByamlDataByKey(std::addressof(left_data), key);
        if (get_result == true) { continue; }

        PrintByamlDataSingle(r_iterator, right_data, i, nullptr, indent_level + 1, PrintSide::Right);
    }

    /* Left */
    for (u32 i = 0; i < l_iterator->GetDataCount(); ++i) {

        /* Get Byaml Data */
        vp::res::ByamlData left_data;
        vp::res::ByamlData right_data;
        l_iterator->TryGetByamlDataByIndex(std::addressof(left_data), i);

        /* Find if the key is not found in the other byaml */
        const char *key = nullptr;
        l_iterator->TryGetKeyByData(std::addressof(key), left_data);
        const bool get_result = r_iterator->TryGetByamlDataByKey(std::addressof(right_data), key);
        if (get_result == true) { continue; }

        PrintByamlDataSingle(l_iterator, left_data, i, nullptr, indent_level + 1, PrintSide::Left);
    }
}


/* Note; only works when unique values */
class ByamlDictionaryParser {
    private:
        struct IdentifierNode {
            vp::res::ByamlIterator identifier_iter;
            vp::res::ByamlData     identifier_value;
            u32                    index;
        };
    public:
        static constexpr u32 InvalidIndex = 0xffff'ffff;
    public:
        IdentifierNode          *m_identifier_node_array;
        const char              *identifier_key;
        vp::res::ByamlIterator   m_root_iter;
    private:
        static int SortIdentifiers(IdentifierNode *lhs, IdentifierNode *rhs) {
            return CompareByamlData(std::addressof(lhs->identifier_iter), std::addressof(rhs->identifier_iter), lhs->identifier_value, rhs->identifier_value);
        }
    public:
        constexpr ALWAYS_INLINE ByamlDictionaryParser() : m_identifier_node_array(nullptr), identifier_key(nullptr) {/*...*/}
        ALWAYS_INLINE ~ByamlDictionaryParser() {
            this->Finalize();
        }

        bool Initialize(vp::res::ByamlIterator *root_iter, const char *identifier_key) {

            /* Integrity checks */
            if (root_iter == nullptr || identifier_key == nullptr || root_iter->IsValid() == false || root_iter->GetDataType() == 0) { return false; }

            /* Copy iterator */
            m_root_iter = *root_iter;

            /* Create identifier nodes */
            m_identifier_node_array = new IdentifierNode[m_root_iter.GetDataCount()];

            /* Read all identifier key values into nodes */
            for (u32 i = 0; i < m_root_iter.GetDataCount(); ++i) {
                const bool result0 = m_root_iter.TryGetIteratorByIndex(std::addressof(m_identifier_node_array[i].identifier_iter), i);
                if (result0 == false) { return false; }
                const bool result1 = m_identifier_node_array[i].identifier_iter.TryGetByamlDataByKey(std::addressof(m_identifier_node_array[i].identifier_value), identifier_key);
                if (result1 == false) { return false; }
                m_identifier_node_array[i].index = i;
                if (m_identifier_node_array[i].identifier_value.data_type != m_identifier_node_array[0].identifier_value.data_type) { return false; }
            }
            return true;
        }

        void Finalize() {
            if (m_identifier_node_array != nullptr) {
                delete [] m_identifier_node_array;
            }
            m_identifier_node_array = nullptr;
            identifier_key          = nullptr;
        }

        u32 FindEntryIndex(IdentifierNode *identifier_node) {

            /* Binary search for equivalent node */
            u32 size = m_root_iter.GetDataCount();
            for (u32 i = 0; i < size; ++i) {
                const s32 result = CompareByamlData(std::addressof(identifier_node->identifier_iter), std::addressof(m_identifier_node_array[i].identifier_iter), identifier_node->identifier_value, m_identifier_node_array[i].identifier_value);
                if (result == 0) {
                    return i;
                }
            }
            return 0xffff'ffff;
        }

        ALWAYS_INLINE bool DiffAllByamlData(ByamlDictionaryParser *r_parser, const char *array_name_key, ByamlDataOverrideCallback override_callback, u32 indent_level, bool is_print) {

            /* Compare and print all shared byaml data */
            ByamlDictionaryParser::IdentifierNode *l_nodes = this->GetNodeArray();
            for (u32 i = 0; i < this->GetNodeCount(); ++i) {
                const u32 r_entry_index = r_parser->FindEntryIndex(std::addressof(l_nodes[i]));
                if (r_entry_index == ByamlDictionaryParser::InvalidIndex) { if (is_print == false) { return false; } continue; }

                vp::res::ByamlData l_data;
                m_root_iter.TryGetByamlDataByIndex(std::addressof(l_data), i);
                vp::res::ByamlData r_data;
                r_parser->m_root_iter.TryGetByamlDataByIndex(std::addressof(r_data), r_entry_index);

                const bool cmp = CompareByamlData(std::addressof(m_root_iter), std::addressof(r_parser->m_root_iter), l_data, r_data);
                if (cmp == 0) { continue; }
                if (is_print == false) { return false; }
                DiffByamlData(std::addressof(m_root_iter), std::addressof(r_parser->m_root_iter), l_data, r_data, i, r_entry_index, array_name_key, override_callback, indent_level);
            }
            return true;
        }

        ALWAYS_INLINE void ProcessAllByamlDataOnly(ByamlDictionaryParser *other_parser, const char *array_name_key,  u32 indent_level, PrintSide print_side) {

            /* Print all data not found in other parser */
            ByamlDictionaryParser::IdentifierNode *nodes = this->GetNodeArray();
            for (u32 i = 0; i < this->GetNodeCount(); ++i) {
                const u32 r_entry_index = other_parser->FindEntryIndex(std::addressof(nodes[i]));
                if (r_entry_index != ByamlDictionaryParser::InvalidIndex) { continue; }

                vp::res::ByamlData data;
                m_root_iter.TryGetByamlDataByIndex(std::addressof(data), i);
                PrintByamlDataSingle(std::addressof(m_root_iter), data, i, array_name_key, indent_level, print_side);
            }
        }
        ALWAYS_INLINE void ProcessAllByamlDataSingle(const char *array_name_key, u32 indent_level, PrintSide print_side) {

            /* Print all data not found in other parser */
            //ByamlDictionaryParser::IdentifierNode *nodes = this->GetNodeArray();
            for (u32 i = 0; i < this->GetNodeCount(); ++i) {

                vp::res::ByamlData data;
                m_root_iter.TryGetByamlDataByIndex(std::addressof(data), i);
                PrintByamlDataSingle(std::addressof(m_root_iter), data, i, array_name_key, indent_level, print_side);
            }
        }

        ALWAYS_INLINE u32 GetNodeCount() const { return m_root_iter.GetDataCount();}
        constexpr ALWAYS_INLINE IdentifierNode *GetNodeArray() const { return m_identifier_node_array;}
};

enum class ByamlFileType {
    Generic,
    Bgyml,
    AiDefn,
    Rstbl,
    Esetb,
    Bcett,
};

enum class ByamlRstblFileVariation {
    Unknown,
    RowIdDictionary,
    IdDictionary,
    TypeDictionary,
    NameDictionary,
};
enum class ByamlGenericFileVariation {
    Unknown,
    PhiveConfig,
    EffectFileInfo,
    BarslistPack,
    Bkdat,
};

ByamlFileType GetByamlFileTypeByExtension(dd::util::FixedString<dd::util::MaxPath> *extension) {

    if (extension->Contains(".bgyml") != 0xffff'ffff)  { return ByamlFileType::Bgyml; }
    if (extension->Contains(".aidefn") != 0xffff'ffff) { return ByamlFileType::AiDefn; }
    if (extension->Contains(".rstbl") != 0xffff'ffff)  { return ByamlFileType::Rstbl; }
    if (extension->Contains(".esetb") != 0xffff'ffff)  { return ByamlFileType::Esetb; }
    if (extension->Contains(".bcett") != 0xffff'ffff)  { return ByamlFileType::Bcett; }

    return ByamlFileType::Generic;
}
ByamlRstblFileVariation GetByamlRstblFileVariationByName(dd::util::FixedString<dd::util::MaxPath> *file_name) {

    if (file_name->Contains("ActorInfo") != 0xffff'ffff)                   { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("AmiiboData") != 0xffff'ffff)                  { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("AmiiboId") != 0xffff'ffff)                   { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("BadgeInfo") != 0xffff'ffff)                   { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("BgmInfo") != 0xffff'ffff)                     { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("BottomInfo") != 0xffff'ffff)                  { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("BulletReplicaInfo") != 0xffff'ffff)           { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("BulletSettingInfo") != 0xffff'ffff)           { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("BynameAdjectiveInfo") != 0xffff'ffff)         { return ByamlRstblFileVariation::IdDictionary; }
    if (file_name->Contains("BynameSubjectInfo") != 0xffff'ffff)           { return ByamlRstblFileVariation::IdDictionary; }
    if (file_name->Contains("CanolaEffectSceneSetting") != 0xffff'ffff)    { return ByamlRstblFileVariation::Unknown; }
    if (file_name->Contains("CoopEnemyInfo") != 0xffff'ffff)               { return ByamlRstblFileVariation::TypeDictionary; }
    if (file_name->Contains("CoopItemInfo") != 0xffff'ffff)                { return ByamlRstblFileVariation::TypeDictionary; }
    if (file_name->Contains("CoopSceneInfo") != 0xffff'ffff)               { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("CoopSkinInfo") != 0xffff'ffff)                { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("EmoteInfo") != 0xffff'ffff)                   { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("EventInfo") != 0xffff'ffff)                   { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("EyebrowInfo") != 0xffff'ffff)                 { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("GearInfoClothes") != 0xffff'ffff)             { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("GearInfoHead") != 0xffff'ffff)                { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("GearInfoShoes") != 0xffff'ffff)               { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("GraffitiParam") != 0xffff'ffff)               { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("HairInfo") != 0xffff'ffff)                    { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("InkTexInfo") != 0xffff'ffff)                  { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("ItemCanSpecialInfo") != 0xffff'ffff)          { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("ItemInfo") != 0xffff'ffff)                    { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("LocatorInfo") != 0xffff'ffff)                 { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("LockerGoodsFigureInfo") != 0xffff'ffff)       { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("LockerGoodsMainPartInfo") != 0xffff'ffff)     { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("LockerGoodsStickerInfo") != 0xffff'ffff)      { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("LockerGoodsSubPartInfo") != 0xffff'ffff)      { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("LockerGoodsSubWeaponInfo") != 0xffff'ffff)    { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("LockerObjInfo") != 0xffff'ffff)               { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("LODThreshold") != 0xffff'ffff)                { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("LotRewardData") != 0xffff'ffff)               { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("MapObjReplicaInfo") != 0xffff'ffff)           { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("MiniGameCardInfo") != 0xffff'ffff)            { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("MiniGameCardSleeve") != 0xffff'ffff)          { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("MiniGameGameNpcData") != 0xffff'ffff)         { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("MiniGameCardPresetDeck") != 0xffff'ffff)      { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("MiniGamePresetDeck") != 0xffff'ffff)          { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("MissionConversationSetParam") != 0xffff'ffff) { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("MissionMapInfo") != 0xffff'ffff)              { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("MysteryBoxData") != 0xffff'ffff)              { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("NamePlateBgInfo") != 0xffff'ffff)             { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("NpcInfo") != 0xffff'ffff)                     { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("NpcSet") != 0xffff'ffff)                      { return ByamlRstblFileVariation::Unknown; }
    if (file_name->Contains("PhysicsBindModelSettingInfo") != 0xffff'ffff) { return ByamlRstblFileVariation::Unknown; }
    if (file_name->Contains("RumbleCall") != 0xffff'ffff)                  { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("SceneInfo") != 0xffff'ffff)                   { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("SeasonInfo") != 0xffff'ffff)                  { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("ShopLineupGearData") != 0xffff'ffff)          { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("SplActorInfo") != 0xffff'ffff)                { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("TankInfo") != 0xffff'ffff)                    { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("TeamColorDataSet") != 0xffff'ffff)            { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("TeamColorOffset") != 0xffff'ffff)             { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("UdemaeInfo") != 0xffff'ffff)                  { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("UIScreen") != 0xffff'ffff)                    { return ByamlRstblFileVariation::NameDictionary; }
    if (file_name->Contains("UIScreenInfo") != 0xffff'ffff)                { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("VersionedModelResource") != 0xffff'ffff)      { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("VersusSceneInfo") != 0xffff'ffff)             { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("WallaObjInfo") != 0xffff'ffff)                { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("WeaponInfoMain") != 0xffff'ffff)              { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("WeaponInfoSpecial") != 0xffff'ffff)           { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("WeaponInfoSub") != 0xffff'ffff)               { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("XLinkPropertyTableList") != 0xffff'ffff)      { return ByamlRstblFileVariation::RowIdDictionary; }
    if (file_name->Contains("XLinkPropertyTable") != 0xffff'ffff)          { return ByamlRstblFileVariation::RowIdDictionary; }

    return ByamlRstblFileVariation::Unknown;
}
ByamlGenericFileVariation GetByamlGenericFileVariationByName(dd::util::FixedString<dd::util::MaxPath> *file_name) {

    if (file_name->Contains("PhiveConfig") != 0xffff'ffff)    { return ByamlGenericFileVariation::PhiveConfig; }
    if (file_name->Contains("EffectFileInfo") != 0xffff'ffff) { return ByamlGenericFileVariation::EffectFileInfo; }
    if (file_name->Contains("BarslistPack") != 0xffff'ffff)   { return ByamlGenericFileVariation::BarslistPack; }
    if (file_name->Contains("bkdat") != 0xffff'ffff)          { return ByamlGenericFileVariation::Bkdat; }

    return ByamlGenericFileVariation::Unknown;
}
