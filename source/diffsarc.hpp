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

void DiffSarc(void *left_file, void *right_file, u32 indent_level) {

    /* Parse sarc directories */
    RomfsDirectoryParser left_iterator = {};
    RomfsDirectoryParser right_iterator = {};
    if (left_iterator.InitializeBySarc(left_file) == false)   { std::cout << "left archive failure" << std::endl; return; }
    if (right_iterator.InitializeBySarc(right_file) == false) { std::cout << "right archive failure" << std::endl; return; }

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
            u32 size = 0;
            void *file = right_sarc.GetFileFast(std::addressof(size), right_sarc.ConvertPathToEntryId(right_paths[i]));

            ProcessSingleImpl(file, size, right_paths[i], indent_level + 1, PrintSide::Right);
        }
    }

    /* Print left only */
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {
            u32 size = 0;
            void *file = left_sarc.GetFileFast(std::addressof(size), left_sarc.ConvertPathToEntryId(left_paths[i]));

            ProcessSingleImpl(file, size, left_paths[i], indent_level + 1, PrintSide::Left);
        }
    }

    /* Cleanup */
    left_iterator.Finalize();
    right_iterator.Finalize();
}

void ProcessSarcSingle(void *sarc_file, u32 indent_level, PrintSide print_side) {

    /* Parse sarc directories */
    RomfsDirectoryParser iterator = {};
    if (iterator.InitializeBySarc(sarc_file) == false) { std::cout << "single sarc failure" << std::endl; return; }

    /* Create a sarc extractor */
    dd::res::SarcExtractor sarc = {};

    if (sarc.Initialize(sarc_file) == false) { return; }

    /* Process every file embedded in the sarc */
    char **paths = iterator.GetFilePathArray();
    for (u32 i = 0; i < iterator.GetFileCount(); ++i) {
        /* Get file entries */
        u32 size = 0;
        void *file = sarc.GetFileFast(std::addressof(size), sarc.ConvertPathToEntryId(paths[i]));

        ProcessSingleImpl(file, size,  paths[i], indent_level, print_side);
    }

    /* Cleanup */
    iterator.Finalize();
}
