 /*
 *  Copyright (C) W. Michael Knudson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along with this program; 
 *  if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#define ZSTD_STATIC_LINKING_ONLY
#include <zstd.h>
#include <bit>
#include <array>

void _consteval_fail();

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
#define ALWAYS_INLINE __attribute__((always_inline)) inline 

#include "util_tstring.hpp"
#include "util_charactercode.hpp"
#include "util_alignment.hpp"
#include "directoryparser.hpp"
//#include "res_sarc.hpp"

bool ProcessFilesImpl(void *left_file, size_t left_size, void *right_file, size_t right_size, const char *right_path, u32 indent_level);

void PrintIndent(u32 indentation_level) {
    for (u32 i = 0; i < indentation_level; ++i) {
        std::cout << "    ";
    }
}

u32 CalculateBwavSize(dd::res::ResBwav *bwav) {
    
    /* Header and channels sizes with alignment */
    u32 size = dd::util::AlignUp(sizeof(dd::res::ResBwav) + bwav->channel_count * sizeof(dd::res::ResBwavChannelInfo), dd::res::ResBwav::SampleArrayAlignment);
    
    dd::res::ResBwavChannelInfo *channel_info = reinterpret_cast<dd::res::ResBwavChannelInfo*>(reinterpret_cast<uintptr_t>(bwav) + sizeof(dd::res::ResBwav));
    
    /* Aligned channel sizes */
    for (int i = 0; i < bwav->channel_count - 1; ++i) {
        size = size + (channel_info[i].sample_count / 14) * sizeof(u64);
        if (channel_info[bwav->channel_count - 1].sample_count % 14 != 0) {
            size += (channel_info[bwav->channel_count - 1].sample_count % 14) / 2 + channel_info[bwav->channel_count - 1].sample_count % 2 + 1;
        }
        size = dd::util::AlignUp(size, dd::res::ResBwav::SampleArrayAlignment);
    }

    /* Last channel is unaligned */
    size = size + (channel_info[bwav->channel_count - 1].sample_count / 14) * sizeof(u64);
    if (channel_info[bwav->channel_count - 1].sample_count % 14 != 0) {
        size += (channel_info[bwav->channel_count - 1].sample_count % 14) / 2 + channel_info[bwav->channel_count - 1].sample_count % 2 + 1;
    }
    return size;
}

void DiffBars(void *left_file, void *right_file, u32 indent_level) {

    /* Parse sarc directories */
    RomfsDirectoryParser left_iterator;
    RomfsDirectoryParser right_iterator;
    if (left_iterator.InitializeByBars(left_file) == false)   { std::cout << "archive failure" << std::endl; return; }
    if (right_iterator.InitializeByBars(right_file) == false) { std::cout << "archive failure" << std::endl; return; }

    /* Create a sarc extractor */
    dd::res::BarsExtractor left_bars;
    dd::res::BarsExtractor right_bars;

    if (left_bars.Initialize(left_file)   == false) { return; }
    if (right_bars.Initialize(right_file) == false) { return; }

    /* Compare every file embedded in the sarc */
    char **left_paths = left_iterator.GetFilePathArray();
    char **right_paths = right_iterator.GetFilePathArray();
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index != RomfsDirectoryParser::InvalidIndex) {
            
            /* Get path indexs */
            const u32 l_index = left_bars.FindEntryIndexByName(left_paths[i]);
            const u32 r_index = right_bars.FindEntryIndexByName(right_paths[index]);
            
            /* Get Amta file entries */
            dd::res::ResAmta *l_amta = left_bars.GetAmtaByIndex(l_index);
            dd::res::ResAmta *r_amta = right_bars.GetAmtaByIndex(r_index);
            
            if (l_amta->IsValid() == false || r_amta->IsValid() == false) {
                ::puts("Invalid bwav");
            }

            const u32 l_amta_size = l_amta->file_size;
            const u32 r_amta_size = r_amta->file_size;

            /* Process Amta */
            char path[MAX_PATH] = {};
            ::snprintf(path, sizeof(path), "%s.bamta", right_paths[index]);
            ProcessFilesImpl(reinterpret_cast<void*>(l_amta), l_amta_size, reinterpret_cast<void*>(r_amta), r_amta_size, path, indent_level + 1);
            
            /* Get Bwav file entries */
            dd::res::ResBwav *l_bwav = left_bars.GetBwavByIndex(l_index);
            dd::res::ResBwav *r_bwav = right_bars.GetBwavByIndex(r_index);
            
            if (l_bwav->IsValid() == false || r_bwav->IsValid() == false) {
                ::puts("Invalid bwav");
            }

            const u32 l_bwav_size = CalculateBwavSize(l_bwav);
            const u32 r_bwav_size = CalculateBwavSize(r_bwav);

            /* Process bwav */
            if (l_bwav->is_prefetch == true && r_bwav->is_prefetch == true) {
                ::snprintf(path, sizeof(path), "%s.prefetch.bwav", right_paths[index]);
            } else if (l_bwav->is_prefetch == false && r_bwav->is_prefetch == false) {
                ::snprintf(path, sizeof(path), "%s.bwav", right_paths[index]);
            } else {
                if (l_bwav->is_prefetch == true) {
                    ::snprintf(path, sizeof(path), "%s.bwav (left was prefetch)", right_paths[index]);
                } else {
                    ::snprintf(path, sizeof(path), "%s.prefetch.bwav (left was non-prefetch)", right_paths[index]);
                }
            }

            ProcessFilesImpl(reinterpret_cast<void*>(l_bwav), l_bwav_size, reinterpret_cast<void*>(r_bwav), r_bwav_size, path, indent_level + 1);
        }
    }
    
    /* Print right only */
    for (u32 i = 0; i < right_iterator.GetFileCount(); ++i) {
        u32 index = left_iterator.FindPathIndex(right_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << right_paths[i] << ".bamta" << std::endl;
            PrintIndent(indent_level + 1);
            
            dd::res::ResBwav *bwav = right_bars.GetBwavByIndex(i);
            if (bwav->is_prefetch == true) {
                std::cout << "Right only: " << right_paths[i] << ".prefetch.bwav"  << std::endl;
            } else {
                std::cout << "Right only: " << right_paths[i] << ".bwav"  << std::endl;
            }
        }
    }

    /* Print left only */
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Left only: " << left_paths[i] << ".bamta" << std::endl;
            PrintIndent(indent_level + 1);
            
            dd::res::ResBwav *bwav = left_bars.GetBwavByIndex(i);
            if (bwav->is_prefetch == true) {
                std::cout << "Left only: " << left_paths[i] << ".prefetch.bwav"  << std::endl;
            } else {
                std::cout << "Left only: " << left_paths[i] << ".bwav"  << std::endl;
            }
        }
    }

    /* Cleanup */
    left_iterator.Finalize();
    right_iterator.Finalize();
}

void DiffSarc(void *left_file, void *right_file, u32 indent_level) {
    
    /* Parse sarc directories */
    RomfsDirectoryParser left_iterator;
    RomfsDirectoryParser right_iterator;
    if (left_iterator.InitializeBySarc(left_file) == false)   { std::cout << "archive failure" << std::endl; return; }
    if (right_iterator.InitializeBySarc(right_file) == false) { std::cout << "archive failure" << std::endl; return; }

    /* Create a sarc extractor */
    dd::res::SarcExtractor left_sarc;
    dd::res::SarcExtractor right_sarc;

    if (left_sarc.Initialize(left_file)   == false) { return; }
    if (right_sarc.Initialize(right_file) == false) { return; }

    /* Compare every file embedded in the sarc */
    char **left_paths = left_iterator.GetFilePathArray();
    char **right_paths = right_iterator.GetFilePathArray();
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index != RomfsDirectoryParser::InvalidIndex) {
            /* Get file entries */
            u32 l_size = 0;
            u32 r_size = 0;
            void *l_file = left_sarc.GetFileFast(std::addressof(l_size), left_sarc.ConvertPathToEntryId(left_paths[i]));
            void *r_file = right_sarc.GetFileFast(std::addressof(r_size), right_sarc.ConvertPathToEntryId(right_paths[index]));
            
            ProcessFilesImpl(l_file, l_size, r_file, r_size, right_paths[index], indent_level + 1);
        }
    }
    
    /* Print right only */
    for (u32 i = 0; i < right_iterator.GetFileCount(); ++i) {
        u32 index = left_iterator.FindPathIndex(right_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Right only: " << right_paths[i] << std::endl;
        }
    }

    /* Print left only*/
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout << "Left only: " << left_paths[i] << std::endl;
        }
    }

    /* Cleanup */
    left_iterator.Finalize();
    right_iterator.Finalize();
}

bool ProcessFileByWin32(const char *left_dir, const char *right_dir, const char *left_file_path, const char *right_file_path) {

    /* Format paths */
    char left_path[MAX_PATH]  = {};
    char right_path[MAX_PATH] = {};
    ::snprintf(left_path,  MAX_PATH, "%s/%s", left_dir,  left_file_path);
    ::snprintf(right_path, MAX_PATH, "%s/%s", right_dir, right_file_path);

    /* Open files */
    HANDLE left_file_handle  = ::CreateFileA(left_path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (left_file_handle == INVALID_HANDLE_VALUE) { return false; }
    HANDLE right_file_handle = ::CreateFileA(right_path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (right_file_handle == INVALID_HANDLE_VALUE) { ::CloseHandle(left_file_handle); return false; }

    /* Get sizes */
    size_t left_file_size = 0;
    size_t right_file_size = 0;
    bool left_size_result  = ::GetFileSizeEx(left_file_handle,  reinterpret_cast<LARGE_INTEGER*>(std::addressof(left_file_size)));
    bool right_size_result = ::GetFileSizeEx(right_file_handle, reinterpret_cast<LARGE_INTEGER*>(std::addressof(right_file_size)));
    if (left_size_result == false || right_size_result == false || left_file_size == INVALID_FILE_SIZE || right_file_size == INVALID_FILE_SIZE) {
        ::CloseHandle(left_file_handle);
        ::CloseHandle(right_file_handle);
        return false;
    }

    /* Create file buffers */
    void *left_file  = reinterpret_cast<void*>(new char[left_file_size]);
    void *right_file = reinterpret_cast<void*>(new char[right_file_size]);

    /* Load files */
    long unsigned int left_read_size = 0;
    long unsigned int right_read_size = 0;
    for (u32 i = 0; i < (left_file_size / 0xFFFF'FFFF); ++i) {
        bool read_result  = ::ReadFile(left_file_handle,  left_file,  0xFFFF'FFFF,  std::addressof(left_read_size),  nullptr);
        if (read_result == false || left_read_size != 0xFFFF'FFFF) { 
            ::CloseHandle(left_file_handle);
            ::CloseHandle(right_file_handle);
            delete reinterpret_cast<char*>(left_file);
            delete reinterpret_cast<char*>(right_file);
            return false;
        }
    }
    if (::ReadFile(left_file_handle,  left_file,  left_file_size % 0xFFFFFFFF,  std::addressof(left_read_size),  nullptr) == false || left_read_size != left_file_size % 0xFFFFFFFF) { 
        ::CloseHandle(left_file_handle);
        ::CloseHandle(right_file_handle);
        delete reinterpret_cast<char*>(left_file);
        delete reinterpret_cast<char*>(right_file);
        return false;
    }
    for (u32 i = 0; i < (right_file_size / 0xFFFF'FFFF); ++i) {
        bool read_result  = ::ReadFile(right_file_handle,  right_file,  0xFFFF'FFFF,  std::addressof(right_read_size),  nullptr);
        if (read_result == false || left_read_size != 0xFFFF'FFFF) { 
            ::CloseHandle(left_file_handle);
            ::CloseHandle(right_file_handle);
            delete reinterpret_cast<char*>(left_file);
            delete reinterpret_cast<char*>(right_file);
            return false;
        }
    }
    if (::ReadFile(right_file_handle,  right_file,  right_file_size % 0xFFFFFFFF,  std::addressof(right_read_size),  nullptr) == false || right_read_size != right_file_size % 0xFFFFFFFF) { 
        ::CloseHandle(left_file_handle);
        ::CloseHandle(right_file_handle);
        delete reinterpret_cast<char*>(left_file);
        delete reinterpret_cast<char*>(right_file);
        return false;
    }
    

    /* Process files impl */
    if (ProcessFilesImpl(left_file, left_file_size, right_file, right_file_size, right_file_path, 0) == false) {
        std::cout << "false" << std::endl;
    }

    /* Cleanup */
    ::CloseHandle(left_file_handle);
    ::CloseHandle(right_file_handle);
    delete reinterpret_cast<char*>(left_file);
    delete reinterpret_cast<char*>(right_file);
    
    return true;
}

bool TryDecompressZstd(void **out_file, size_t *out_size, void *in_file, size_t file_size) {
    /* Check for zstd compression */
    const size_t zstd_result  = ::ZSTD_getFrameContentSize(in_file, file_size);
    if (zstd_result == ZSTD_CONTENTSIZE_ERROR || zstd_result == ZSTD_CONTENTSIZE_UNKNOWN) {
        return false;
    }

    /* Allocate file buffer */
    void *decompressed_file  = reinterpret_cast<void*>(new char[zstd_result]);
    if (decompressed_file == nullptr) { return false; }

    /* Decompress */
    ::ZSTD_decompress(decompressed_file, zstd_result, in_file, file_size);
    *out_file = decompressed_file;
    *out_size = zstd_result;

    return true;
}

enum FileType {
    FileType_Invalid = 0,
    FileType_Sarc    = 1,
    FileType_Bfres   = 2,
    FileType_Bntx    = 3,
    FileType_Bars    = 4,
};

FileType GetFileType(void *left, void *right) {
    if (reinterpret_cast<dd::res::ResSarc*>(left)->magic == dd::res::ResSarc::Magic && reinterpret_cast<dd::res::ResSarc*>(right)->magic == dd::res::ResSarc::Magic) {
        return FileType_Sarc;
    } else if (reinterpret_cast<dd::res::ResBars*>(left)->IsValid() == true && reinterpret_cast<dd::res::ResBars*>(right)->IsValid() == true) {
        return FileType_Bars;
    }
    return FileType_Invalid;
}

bool ProcessFilesImpl(void *left_file, size_t left_size, void *right_file, size_t right_size, const char *right_path, u32 indent_level) {

    /* Check files are different on a byte level */
    if (left_size == right_size && ::memcmp(left_file, right_file, left_size) == 0) { return true; }
    if (0x10 > left_size || 0x10 > right_size) { return false; }

    /* Decompress files if necessary */
    void   *left_full       = left_file;
    void   *right_full      = right_file;
    size_t  left_full_size  = left_size;
    size_t  right_full_size = right_size;

    bool  need_free_left  = false;
    bool  need_free_right = false;
    need_free_left  = TryDecompressZstd(std::addressof(left_full),  std::addressof(left_full_size), left_file,  left_size);
    need_free_right = TryDecompressZstd(std::addressof(right_full), std::addressof(right_full_size), right_file, right_size);

    /* Print out that file is different */
    PrintIndent(indent_level);
    std::cout << std::hex << "Different(left: 0x" << std::setfill('0') << std::setw(8) << left_full_size << " bytes)(right: 0x" << std::setfill('0') << std::setw(8) << right_full_size << " bytes): " << right_path << std::endl;

    /* Pass off to specific diff function */
    switch (GetFileType(left_full, right_full)) {
        case FileType_Sarc:
            DiffSarc(left_full, right_full, indent_level);
            break;
        case FileType_Bars:
            DiffBars(left_full, right_full, indent_level);
            break;
        default:
            break;
    }

    /* Cleanup */
    if (need_free_left == true) {
        delete reinterpret_cast<char*>(left_full);
    }
    if (need_free_right == true) {
        delete reinterpret_cast<char*>(right_full);
    }
    
    return true;
}

int main(int argc, char **argv) {
    
    /* Parse Options */
    if (argc != 3 || ::strcmp("-h", argv[1]) == 0 || ::strcmp("-h", argv[2]) == 0) {
        std::cout << "options(required): [left romfs dir] [right romfs dir]" << std::endl;
        return 0;
    }

    /* Directory iteration */
    RomfsDirectoryParser left_iterator = {};
    RomfsDirectoryParser right_iterator = {};
    left_iterator.Initialize(argv[1]);
    right_iterator.Initialize(argv[2]);

    /* Compare files in both paths */
    char **left_paths = left_iterator.GetFilePathArray();
    char **right_paths = right_iterator.GetFilePathArray();
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index != RomfsDirectoryParser::InvalidIndex) {
            if (ProcessFileByWin32(argv[1], argv[2], left_paths[i], right_paths[index]) == false) {
                std::cout << "process win32 failed" << std::endl;
            }
        }
    }

    /* Print right only */
    for (u32 i = 0; i < right_iterator.GetFileCount(); ++i) {
        u32 index = left_iterator.FindPathIndex(right_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {
            std::cout << "Right only: " << right_paths[i] << std::endl;
        }
    }

    /* Print left only*/
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {
            std::cout << "Left only: " << left_paths[i] << std::endl;
        }
    }

    left_iterator.Finalize();
    right_iterator.Finalize();
    return 0;
}
