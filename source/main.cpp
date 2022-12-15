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
#include <cstring>
#include <iostream>

void _consteval_fail();

namespace dd::util::math {
    
    using Vector2f  = float[2];
    using Vector3f  = float[3];
    using Vector4f  = float[4];
    using Matrix22f = float[2][2];
    using Matrix34f = float[3][4];
    using Matrix44f = float[4][4];
}

struct ResBfresAnimCurve;

typedef unsigned long long u64;
typedef long long s64;
typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;
typedef short s16;
typedef unsigned char u8;
typedef signed char s8;
#define ALWAYS_INLINE __attribute__((always_inline)) inline 

#include "util_tstring.hpp"
#include "util_charactercode.hpp"
#include "util_alignment.hpp"
#include "res_nintendowarerelocationtable.h"
#include "res_nintendowarefileheader.hpp"
#include "res_nintendowarerelocationtableimpl.hpp"
#include "res_nintendowaredictionary.hpp"
#include "res_gfxcommon.hpp"
#include "res_bntx.hpp"
#include "res_bfresshape.hpp"
#include "res_bfresmaterial.hpp"
#include "res_bfresskeleton.hpp"
#include "res_bfresmodel.hpp"
#include "res_bfresskeletalanim.hpp"
#include "res_bfresmaterialanim.hpp"
#include "res_bfresbonevisibilityanim.hpp"
#include "res_bfresshapeanim.hpp"
#include "res_bfressceneanim.hpp"
#include "res_bfres.hpp"
#include "res_szs.hpp"
#include "res_bea.hpp"
#include "util_alignment.hpp"

#include "directoryparser.hpp"

bool ProcessFilesImpl(void *left_file, size_t left_size, void *right_file, size_t right_size, const char *right_path, u32 indent_level);
void ProcessSingleImpl(void *file, size_t size, const char *path, u32 indent_level, bool is_right);

void PrintIndent(u32 indentation_level) {
    for (u32 i = 0; i < indentation_level; ++i) {
        std::cout << "    ";
    }
}

#include "diffsarc.hpp"
#include "diffbars.hpp"
#include "diffbntx.hpp"
#include "diffbfres.hpp"
#include "diffbea.hpp"

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
    void *left_file  = reinterpret_cast<void*>(new (std::align_val_t(0x1000)) char[left_file_size]);
    void *right_file = reinterpret_cast<void*>(new (std::align_val_t(0x1000)) char[right_file_size]);

    /* Load files */
    long unsigned int left_read_size = 0;
    long unsigned int right_read_size = 0;
    for (u32 i = 0; i < (left_file_size / 0xFFFF'FFFF); ++i) {
        bool read_result  = ::ReadFile(left_file_handle,  left_file,  0xFFFF'FFFF,  std::addressof(left_read_size),  nullptr);
        if (read_result == false || left_read_size != 0xFFFF'FFFF) { 
            ::CloseHandle(left_file_handle);
            ::CloseHandle(right_file_handle);
            ::operator delete [] (reinterpret_cast<char*>(left_file),  std::align_val_t(0x1000));
            ::operator delete [] (reinterpret_cast<char*>(right_file), std::align_val_t(0x1000));
            return false;
        }
    }
    if (::ReadFile(left_file_handle,  left_file,  left_file_size % 0xFFFFFFFF,  std::addressof(left_read_size),  nullptr) == false || left_read_size != left_file_size % 0xFFFFFFFF) { 
        ::CloseHandle(left_file_handle);
        ::CloseHandle(right_file_handle);
        ::operator delete [] (reinterpret_cast<char*>(left_file),  std::align_val_t(0x1000));
        ::operator delete [] (reinterpret_cast<char*>(right_file), std::align_val_t(0x1000));
        return false;
    }
    for (u32 i = 0; i < (right_file_size / 0xFFFF'FFFF); ++i) {
        bool read_result  = ::ReadFile(right_file_handle,  right_file,  0xFFFF'FFFF,  std::addressof(right_read_size),  nullptr);
        if (read_result == false || left_read_size != 0xFFFF'FFFF) { 
            ::CloseHandle(left_file_handle);
            ::CloseHandle(right_file_handle);
            ::operator delete [] (reinterpret_cast<char*>(left_file),  std::align_val_t(0x1000));
            ::operator delete [] (reinterpret_cast<char*>(right_file), std::align_val_t(0x1000));
            return false;
        }
    }
    if (::ReadFile(right_file_handle,  right_file,  right_file_size % 0xFFFFFFFF,  std::addressof(right_read_size),  nullptr) == false || right_read_size != right_file_size % 0xFFFFFFFF) { 
        ::CloseHandle(left_file_handle);
        ::CloseHandle(right_file_handle);
        ::operator delete [] (reinterpret_cast<char*>(left_file),  std::align_val_t(0x1000));
        ::operator delete [] (reinterpret_cast<char*>(right_file), std::align_val_t(0x1000));
        return false;
    }
    

    /* Process files impl */
    if (ProcessFilesImpl(left_file, left_file_size, right_file, right_file_size, right_file_path, 0) == false) {
        std::cout << "false" << std::endl;
    }

    /* Cleanup */
    ::CloseHandle(left_file_handle);
    ::CloseHandle(right_file_handle);
    ::operator delete [] (reinterpret_cast<char*>(left_file),  std::align_val_t(0x1000));
    ::operator delete [] (reinterpret_cast<char*>(right_file), std::align_val_t(0x1000));
    
    return true;
}

bool TryDecompressZstd(void **out_file, size_t *out_size, void *in_file, size_t file_size) {

    /* Check for zstd compression */
    const size_t zstd_result  = ::ZSTD_getFrameContentSize(in_file, file_size);
    if (zstd_result == ZSTD_CONTENTSIZE_ERROR || zstd_result == ZSTD_CONTENTSIZE_UNKNOWN) {
        return false;
    }

    /* Allocate file buffer */
    void *decompressed_file  = reinterpret_cast<void*>(new (std::align_val_t(0x1000)) char[zstd_result]);
    if (decompressed_file == nullptr) { return false; }

    /* Decompress */
    ::ZSTD_decompress(decompressed_file, zstd_result, in_file, file_size);
    *out_file = decompressed_file;
    *out_size = zstd_result;

    return true;
}

bool TryDecompressSZS(void **out_file, size_t *out_size, void *in_file, size_t file_size) {

    if (file_size < sizeof(dd::res::ResSzs)) { return false; } 

    /* Check for SZS compression */
    dd::res::ResSzs *szs = reinterpret_cast<dd::res::ResSzs*>(in_file);
    if (szs->magic != dd::res::ResSzs::Magic) {
        return false;
    }

    /* Allocate file buffer */
    const u32 decomp_size = dd::util::SwapEndian32(szs->decompressed_size);
    void *decompressed_file = reinterpret_cast<void*>(new (std::align_val_t(0x1000)) char[decomp_size]);
    if (decompressed_file == nullptr) { return false; }

    /* Decompress */
    dd::res::DecodeSZS(reinterpret_cast<u8*>(decompressed_file), szs);
    *out_file = decompressed_file;
    *out_size = decomp_size;

    return true;
}

enum FileType {
    FileType_Invalid = 0,
    FileType_Sarc    = 1,
    FileType_Bfres   = 2,
    FileType_Bntx    = 3,
    FileType_Bars    = 4,
    FileType_Bea     = 5,
};

FileType GetFileTypeSingle(void *file) {
    dd::res::SarcExtractor sarc_test = {};
    if (sarc_test.Initialize(file) == true) {
        return FileType_Sarc;
    } else if (reinterpret_cast<dd::res::ResBars*>(file)->IsValid() == true) {
        return FileType_Bars;
    } else if (dd::res::ResBntx::IsValid(file) == true) {
        return FileType_Bntx;
    } else if (dd::res::ResBfres::IsValid(file) == true) {
        return FileType_Bfres;
    } else if (dd::res::ResBea::IsValid(file) == true) {
        return FileType_Bea;
    }
    return FileType_Invalid;
}
FileType GetFileType(void *left, void *right) {
    FileType l_type = GetFileTypeSingle(left);
    FileType r_type = GetFileTypeSingle(right);
    return (l_type == r_type) ? l_type : FileType_Invalid;
}

void ProcessSingleImpl(void *file, size_t size, const char *file_path, u32 indent_level, bool is_right) {

    /* Decompress file if necessary */
    void   *full      = file;
    size_t  full_size = size;

    bool  need_free = false;
    need_free       = TryDecompressZstd(std::addressof(full), std::addressof(full_size), file, size);
    if (need_free == false) {
        need_free = TryDecompressSZS(std::addressof(full), std::addressof(full_size), file, size);
    }

    /* Print file */
    PrintIndent(indent_level);
    if (is_right == false) {
        std::cout << "Left only (size: 0x" << std::setfill('0') << std::setw(8) << full_size << "): " << file_path << std::endl;
    } else {
        std::cout << "Right only(size: 0x" << std::setfill('0') << std::setw(8) << full_size << "): " << file_path << std::endl;
    }

    /* Check archive type */
    FileType type = FileType_Invalid;
    if (0x20 <= full_size) {
        type = GetFileTypeSingle(full);
    }
    switch (type) {
        case FileType_Sarc:
            ProcessSarcSingle(full, indent_level + 1, is_right);
            break;
        case FileType_Bars:
            ProcessBarsSingle(full, indent_level + 1, is_right);
            break;
        case FileType_Bntx:
            ProcessBntxSingle(full, indent_level + 1, is_right);
            break;
        case FileType_Bfres:
            ProcessBfresSingle(full, indent_level + 1, is_right);
            break;
        case FileType_Bea:
            ProcessBeaSingle(full, indent_level + 1, is_right);
            break;
        default:
            break;
    };

    if (need_free == true) {
        ::operator delete [] (reinterpret_cast<char*>(full),  std::align_val_t(0x1000));
    }
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
    if (need_free_left == false) {
        need_free_left = TryDecompressSZS(std::addressof(left_full), std::addressof(left_full_size), left_file, left_size);
    }
    if (need_free_right == false) {
        need_free_right = TryDecompressSZS(std::addressof(right_full), std::addressof(right_full_size), right_file, right_size);
    }

    /* Print out that file is different */
    PrintIndent(indent_level);
    std::cout << std::hex << "Different(left: 0x" << std::setfill('0') << std::setw(8) << left_full_size << " bytes)(right: 0x" << std::setfill('0') << std::setw(8) << right_full_size << " bytes): " << right_path << std::endl;

    /* Pass off to specific diff function */
    FileType type = FileType_Invalid;
    if (0x20 <= left_full_size && 0x20 <= right_full_size) {
        type = GetFileType(left_full, right_full);
    }
    switch (type) {
        case FileType_Sarc:
            DiffSarc(left_full, right_full, indent_level);
            break;
        case FileType_Bars:
            DiffBars(left_full, right_full, indent_level);
            break;
        case FileType_Bntx:
            DiffBntx(left_full, right_full, indent_level);
            break;
        case FileType_Bfres:
            DiffBfres(left_full, right_full, indent_level);
            break;
        case FileType_Bea:
            DiffBea(left_full, right_full, indent_level);
            break;
        default:
            break;
    }

    /* Cleanup */
    if (need_free_left == true) {
        ::operator delete [] (reinterpret_cast<char*>(left_full),  std::align_val_t(0x1000));
    }
    if (need_free_right == true) {
        ::operator delete [] (reinterpret_cast<char*>(right_full),  std::align_val_t(0x1000));
    }
    
    return true;
}

bool ProcessWin32Single(const char *dir_path, const char *file_path, bool is_right) {
    
    char path[MAX_PATH]  = {};
    ::snprintf(path,  MAX_PATH, "%s/%s", dir_path,  file_path);

    /* Open file */
    HANDLE file_handle  = ::CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_handle == INVALID_HANDLE_VALUE) { return false; }

    /* Get size */
    size_t file_size = 0;
    const bool size_result  = ::GetFileSizeEx(file_handle,  reinterpret_cast<LARGE_INTEGER*>(std::addressof(file_size)));
    if (size_result == false || file_size == INVALID_FILE_SIZE) {
        ::CloseHandle(file_handle);
        return false;
    }

    /* Create file buffer */
    void *file  = reinterpret_cast<void*>(new (std::align_val_t(0x1000)) char[file_size]);

    /* Load file */
    long unsigned int read_size = 0;
    for (u32 i = 0; i < (file_size / 0xFFFF'FFFF); ++i) {
        bool read_result  = ::ReadFile(file_handle, file, 0xFFFF'FFFF, std::addressof(read_size),  nullptr);
        if (read_result == false || read_size != 0xFFFF'FFFF) { 
            ::CloseHandle(file_handle);
            ::operator delete [] (reinterpret_cast<char*>(file),  std::align_val_t(0x1000));
            return false;
        }
    }
    if (::ReadFile(file_handle, file, file_size % 0xFFFFFFFF, std::addressof(read_size), nullptr) == false || read_size != file_size % 0xFFFFFFFF) { 
        ::CloseHandle(file_handle);
        ::operator delete [] (reinterpret_cast<char*>(file),  std::align_val_t(0x1000));
        return false;
    }

    /* Process impl */
    ProcessSingleImpl(file, file_size, file_path, 0, is_right);

    /* Cleanup */
    ::CloseHandle(file_handle);
    ::operator delete [] (reinterpret_cast<char*>(file),  std::align_val_t(0x1000));

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
            ProcessWin32Single(argv[2], right_paths[i], true);
        }
    }

    /* Print left only*/
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {
            ProcessWin32Single(argv[1], left_paths[i], false);
        }
    }

    left_iterator.Finalize();
    right_iterator.Finalize();
    return 0;
}
