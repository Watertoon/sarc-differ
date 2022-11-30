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
#include <filesystem>
#include <iostream>
#include <string.h>
#include <algorithm>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "res_sarc.hpp"
#include "util_crc32b.hpp"
#include "res_amta.hpp"
#include "res_bwav.hpp"
#include "res_bars.hpp"

const char *GetRomfsPath(const char *string) {
    const char *string_iter = string;
    while (*string_iter != '\0') {
        if (*string_iter == 'r') {
            if (::strncmp("romfs\\", string_iter, 5) == 0) {
                return string_iter + 6;
            }
        }
        ++string_iter;
    }
    return "";
}

/*const char *GetVersion(const char *path) {
    u32 len = ::strlen(path);
    while (*path != '\0') {
        if (*path == '.' && 5 < len && isdigit(*(path + 1)) == true && isdigit(*(path + 2)) == true && isdigit(*(path + 3)) == true && *(path + 4) == '.') {
            return ++path;
        }
        ++path;
        --len;
    }
    return nullptr;
}*/

struct StringCompare {
    bool operator() (const char *lhs, const char *rhs) const {
        if (lhs != nullptr && rhs != nullptr && ::strncmp(lhs, rhs, MAX_PATH) <= 0) {
            return true;
        } else {
            return false;
        }
    }
};

class RomfsDirectoryParser {
    public:
        static constexpr inline u32 InvalidIndex = 0xFFFF'FFFF;
    private:
        u32    m_file_count;
        char **m_file_paths;
        char  *m_storage;
    public:
        constexpr ALWAYS_INLINE RomfsDirectoryParser() : m_file_count(0), m_file_paths(nullptr), m_storage(nullptr) {/*...*/}

        bool Initialize(const char *path) {

            /* Countup romfs files */
            {
                std::filesystem::recursive_directory_iterator dir_counter(path);
                for (const auto &dir_entry : dir_counter) {
                    if (dir_entry.is_regular_file() == true) {
                        std::string r_path = dir_entry.path().string();
                        const char *string = GetRomfsPath(r_path.c_str());
                        if (::strcmp(string, "") == 0) { continue; }
                        ++m_file_count;
                    }
                }
            }

            /* Allocate storage for file path strings */
            m_file_paths = new char*[m_file_count];
            if (m_file_paths == nullptr) { return false; }

            m_storage = new char[m_file_count * MAX_PATH];
            if (m_storage == nullptr) { delete [] m_file_paths; m_file_paths = nullptr; return false; }

            for (u32 i = 0; i < m_file_count; ++i) {
                m_file_paths[i] = std::addressof(m_storage[i * MAX_PATH]);
                m_file_paths[i][0] = '\0';
                m_file_paths[i][MAX_PATH - 1] = '\0';
            }

            /* Copy all romfs file path strings */
            {
                std::filesystem::recursive_directory_iterator dir_iterator(path);
                u32 i = 0;
                for (const auto &dir_entry : dir_iterator) {
                    if (dir_entry.is_regular_file() == true && i != m_file_count) {
                        std::string r_path = dir_entry.path().string();
                        const char *string = GetRomfsPath(r_path.c_str());
                        if (::strcmp(string, "") == 0) { continue; }
                        ::strncpy(m_file_paths[i], string, MAX_PATH);
                        m_file_paths[i][MAX_PATH - 1] = '\0';
                        ++i;
                    }
                }
            }

            if (1 < m_file_count) {
                std::sort(m_file_paths, m_file_paths + m_file_count, StringCompare());
            }
            

            return true;
        }

        bool InitializeByArrayNW(const char **array, u32 count) {

            if (array == nullptr) { return false; }

            /* Allocate storage for file path strings */
            m_file_paths = new char*[count];
            if (m_file_paths == nullptr) { return false; }
            m_file_count = count;

            m_storage = new char[m_file_count * MAX_PATH];
            if (m_storage == nullptr) { delete [] m_file_paths; m_file_paths = nullptr; return false; }

            for (u32 i = 0; i < m_file_count; ++i) {
                m_file_paths[i] = std::addressof(m_storage[i * MAX_PATH]);
                m_file_paths[i][MAX_PATH - 1] = '\0';
            }

            /* Copy all romfs file path strings */
            for (u32 i = 0; i < m_file_count; ++i) {
                const char *string = array[i] + 2;
                if (::strcmp(string, "") == 0) { return false; }
                ::strcpy(m_file_paths[i], string);
                m_file_paths[i][MAX_PATH - 1] = '\0';
            }

            return true;
        }

        bool InitializeBySarc(void *sarc) {

            /* Initialize sarc extractor */
            dd::res::SarcExtractor extractor = {};
            if (extractor.Initialize(sarc) == false) { return false; }

            /* Allocate storage for file path strings */
            m_file_paths = new char*[extractor.GetFileCount()];
            if (m_file_paths == nullptr) { return false; }
            m_file_count = extractor.GetFileCount();

            m_storage = new char[m_file_count * MAX_PATH];
            if (m_storage == nullptr) { delete [] m_file_paths; m_file_paths = nullptr; return false; }

            for (u32 i = 0; i < m_file_count; ++i) {
                m_file_paths[i] = std::addressof(m_storage[i * MAX_PATH]);
                m_file_paths[i][MAX_PATH - 1] = '\0';
            }

            /* Copy all romfs file path strings */
            for (u32 i = 0; i < m_file_count; ++i) {
                const char *string = extractor.ConvertEntryIdToPath(i);
                if (::strcmp(string, "") == 0) { return false; }
                ::strcpy(m_file_paths[i], string);
                m_file_paths[i][MAX_PATH - 1] = '\0';
            }

            std::sort(m_file_paths, m_file_paths + m_file_count, StringCompare());

            return true;
        }

        bool InitializeByBars(void *bars_file) {

            /* Initialize sarc extractor */
            dd::res::BarsExtractor extractor = {};
            if (extractor.Initialize(bars_file) == false) { return false; }

            /* Allocate storage for file path strings */
            m_file_paths = new char*[extractor.GetFileCount()];
            if (m_file_paths == nullptr) { return false; }
            m_file_count = extractor.GetFileCount();

            m_storage = new char[m_file_count * MAX_PATH];
            if (m_storage == nullptr) { delete [] m_file_paths; m_file_paths = nullptr; return false; }

            for (u32 i = 0; i < m_file_count; ++i) {
                m_file_paths[i] = std::addressof(m_storage[i * MAX_PATH]);
                m_file_paths[i][MAX_PATH - 1] = '\0';
            }

            /* Copy all romfs file path strings */
            for (u32 i = 0; i < m_file_count; ++i) {
                const char *string = extractor.ConvertEntryIndexToPath(i);
                if (::strcmp(string, "") == 0) { return false; }
                ::strcpy(m_file_paths[i], string);
                m_file_paths[i][MAX_PATH - 1] = '\0';
            }

            std::sort(m_file_paths, m_file_paths + m_file_count, StringCompare());

            return true;
        }

        bool InitializeByResDic(dd::res::ResNintendoWareDictionary *res_dic) {

            if (res_dic == nullptr) { return false; }

            /* Allocate storage for path strings */
            m_file_paths = new char*[res_dic->node_count];
            if (m_file_paths == nullptr) { return false; }
            m_file_count = res_dic->node_count;

            m_storage = new char[m_file_count * MAX_PATH];
            if (m_storage == nullptr) { delete [] m_file_paths; m_file_paths = nullptr; return false; }

            for (u32 i = 0; i < m_file_count; ++i) {
                m_file_paths[i] = std::addressof(m_storage[i * MAX_PATH]);
                m_file_paths[i][MAX_PATH - 1] = '\0';
            }

            /* Copy all res dic path strings */
            for (u32 i = 0; i < m_file_count; ++i) {
                const char *string = (std::addressof(res_dic->root_node) + 1)[i].key + 2;
                if (::strcmp(string, "") == 0) { return false; }
                ::strcpy(m_file_paths[i], string);
                m_file_paths[i][MAX_PATH - 1] = '\0';
            }

            std::sort(m_file_paths, m_file_paths + m_file_count, StringCompare());

            return true;
        }

        void Finalize() {
            if (m_file_paths != nullptr) {
                delete [] m_storage;
                delete [] m_file_paths;
            }
            m_file_count = 0;
        }

        u32 FindPathIndex(const char *path) {
            for (u32 i = 0; i < m_file_count; ++i) {
                /* Special naive strcmp that omits versioning info */
                const char *left_path  = path;
                const char *right_path = m_file_paths[i];
                u32 left_len  = ::strlen(path);
                u32 right_len = ::strlen(m_file_paths[i]);
                while (*left_path != '\0' && *right_path != '\0') {
                    if (*left_path == '.' && *right_path == '.') {
                        ++left_path;
                        ++right_path;
                        --left_len;
                        --right_len;
                        if (5 < left_len && 5 < right_len &&
                            *(left_path - 1) == '.' && *(right_path - 1) == '.' && 
                            isdigit(*left_path) == true && isdigit(*right_path) == true &&
                            isdigit(*(left_path + 1)) == true && isdigit(*(right_path + 1)) == true &&
                            isdigit(*(left_path + 2)) == true && isdigit(*(right_path + 2)) == true &&
                            *(left_path + 3) == '.' && *(right_path + 3) == '.') {
                            left_path  = left_path  + 3;
                            right_path = right_path + 3;
                        }
                    }
                    if (*left_path != *right_path) {
                        break;
                    }
                    ++left_path;
                    ++right_path;
                    --left_len;
                    --right_len;
                }
                if (*left_path == '\0' && *right_path == '\0') {
                    return i;
                }
            }
            
            return InvalidIndex;
        }

        constexpr ALWAYS_INLINE char **GetFilePathArray()       { return m_file_paths; }
        constexpr ALWAYS_INLINE u32    GetFileCount()     const { return m_file_count; }
};
