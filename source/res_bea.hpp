#pragma once

namespace dd::res {

    struct ResBeaFileEntry : public ResNintendoWareSubHeader {
        u16     flag0;
        u16     alignment;
        u32     compressed_size;
        u32     uncompressed_size;
        u32     reserve0;
        size_t  file_offset;
        char   *file_name;

        static constexpr inline u32 Magic = util::TCharCode32("ASST");
    };
    static_assert(sizeof(ResBeaFileEntry) == 0x30);

    struct ResBea : public ResNintendoWareFileHeader {
        s64                         file_count;
        ResBeaFileEntry           **file_entry_array;
        ResNintendoWareDictionary  *file_dictionary;
        u64                         reserve0;
        char                       *archive_name;
        
        static ResBea *ResCast(void *file) {
            ResBea *bea = reinterpret_cast<ResBea*>(file);
            if (bea == nullptr || bea->ResNintendoWareFileHeader::IsValid(Magic, 1, 1) == false) { return nullptr; }
            bea->Relocate();
            return bea;
        }

        static bool IsValid(void *file) {
            ResBea *bea = reinterpret_cast<ResBea*>(file);
            return bea->ResNintendoWareFileHeader::IsValid(Magic, 1, 1, 0);
        }

        ALWAYS_INLINE ResBeaFileEntry *GetFileEntry(const char *file_path) {
            const s32 entry_index = file_dictionary->FindEntryIndex(file_path);
            if (entry_index == ResNintendoWareDictionary::NPos) { return nullptr; }

            return file_entry_array[entry_index];
        }
        
        constexpr ALWAYS_INLINE void *GetFile(ResBeaFileEntry *file_entry) {
            return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) + file_entry->file_offset);
        }

        static constexpr inline u64 Magic = util::TCharCode32("SCNE");
    };
    static_assert(sizeof(ResBea) == 0x48);
}

