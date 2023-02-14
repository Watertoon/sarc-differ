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

void DiffBea(void *left_file, void *right_file, u32 indent_level) {

    /* Res cast files */
    dd::res::ResBea *l_bea = dd::res::ResBea::ResCast(left_file);
    dd::res::ResBea *r_bea = dd::res::ResBea::ResCast(right_file);

    /* Parse bea directories */
    RomfsDirectoryParser left_iterator = {};
    RomfsDirectoryParser right_iterator = {};
    if (left_iterator.InitializeByResDic(l_bea->file_dictionary) == false)  { std::cout << "left archive failure" << std::endl; return; }
    if (right_iterator.InitializeByResDic(r_bea->file_dictionary) == false) { std::cout << "right archive failure" << std::endl; return; }

    /* Compare every file embedded in the bea */
    char **left_paths = left_iterator.GetFilePathArray();
    char **right_paths = right_iterator.GetFilePathArray();
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index != RomfsDirectoryParser::InvalidIndex) {
            /* Get file entries */
            dd::res::ResBeaFileEntry *l_file_entry = l_bea->GetFileEntry(left_paths[i]);
            dd::res::ResBeaFileEntry *r_file_entry = r_bea->GetFileEntry(left_paths[i]);

            ProcessFilesImpl(l_bea->GetFile(l_file_entry), l_file_entry->compressed_size, r_bea->GetFile(r_file_entry), r_file_entry->compressed_size, right_paths[index], indent_level + 1);
        }
    }

    /* Print right only */
    for (u32 i = 0; i < right_iterator.GetFileCount(); ++i) {
        u32 index = left_iterator.FindPathIndex(right_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {
            s32 entry = r_bea->file_dictionary->FindEntryIndex(right_paths[i]);
            dd::res::ResBeaFileEntry *file_entry = r_bea->file_entry_array[entry];

            ProcessSingleImpl(r_bea->GetFile(file_entry), file_entry->compressed_size, right_paths[i], indent_level + 1, PrintSide::Right);
        }
    }

    /* Print left only */
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {
            s32 entry = l_bea->file_dictionary->FindEntryIndex(left_paths[i]);
            dd::res::ResBeaFileEntry *file_entry = l_bea->file_entry_array[entry];

            ProcessSingleImpl(l_bea->GetFile(file_entry), file_entry->compressed_size, left_paths[i], indent_level + 1, PrintSide::Left);
        }
    }

    /* Cleanup */
    left_iterator.Finalize();
    right_iterator.Finalize();
}

void ProcessBeaSingle(void *bea_file, u32 indent_level, PrintSide print_side) {

    /* Res cast */
    dd::res::ResBea *bea = dd::res::ResBea::ResCast(bea_file);

    /* Parse sarc directories */
    RomfsDirectoryParser iterator = {};
    if (iterator.InitializeByResDic(bea->file_dictionary) == false) { std::cout << "single bea archive failure" << std::endl; return; }

    /* Process every file embedded in the bea */
    char **paths = iterator.GetFilePathArray();
    for (u32 i = 0; i < iterator.GetFileCount(); ++i) {
        s32 entry = bea->file_dictionary->FindEntryIndex(paths[i]);
        dd::res::ResBeaFileEntry *file_entry = bea->file_entry_array[entry];

        ProcessSingleImpl(bea->GetFile(file_entry), file_entry->compressed_size, paths[i], indent_level + 1, print_side);
    }

    /* Cleanup */
    iterator.Finalize();
}
