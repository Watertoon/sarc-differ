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
                ::puts("Invalid bamta");
                continue;
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
                continue;
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
            const u32 index = right_bars.FindEntryIndexByName(right_paths[i]);

            dd::res::ResAmta *amta = right_bars.GetAmtaByIndex(index);
            dd::res::ResBwav *bwav = right_bars.GetBwavByIndex(index);
            
            const u32 amta_size = amta->file_size;
            const u32 bwav_size = CalculateBwavSize(bwav);
            
            char path[MAX_PATH] = {};
            ::snprintf(path, sizeof(path), "%s.bamta", right_paths[i]);
            
            ProcessSingleImpl(reinterpret_cast<void*>(amta), amta_size, path, indent_level + 1, PrintSide::Right);

            if (bwav->is_prefetch == true) {
                ::snprintf(path, sizeof(path), "%s.prefetch.bwav", right_paths[i]);
            } else {
                ::snprintf(path, sizeof(path), "%s.bwav", right_paths[i]);
            }

            ProcessSingleImpl(reinterpret_cast<void*>(bwav), bwav_size, path, indent_level + 1, PrintSide::Right);
        }
    }

    /* Print left only */
    for (u32 i = 0; i < left_iterator.GetFileCount(); ++i) {
        u32 index = right_iterator.FindPathIndex(left_paths[i]);
        if (index == RomfsDirectoryParser::InvalidIndex) {

            const u32 index = left_bars.FindEntryIndexByName(left_paths[i]);

            dd::res::ResAmta *amta = left_bars.GetAmtaByIndex(index);
            dd::res::ResBwav *bwav = left_bars.GetBwavByIndex(index);
            
            const u32 amta_size = amta->file_size;
            const u32 bwav_size = CalculateBwavSize(bwav);
            
            char path[MAX_PATH] = {};
            ::snprintf(path, sizeof(path), "%s.bamta", left_paths[i]);
            
            ProcessSingleImpl(reinterpret_cast<void*>(amta), amta_size, path, indent_level + 1, PrintSide::Left);

            if (bwav->is_prefetch == true) {
                ::snprintf(path, sizeof(path), "%s.prefetch.bwav", left_paths[i]);
            } else {
                ::snprintf(path, sizeof(path), "%s.bwav", left_paths[i]);
            }

            ProcessSingleImpl(reinterpret_cast<void*>(bwav), bwav_size, path, indent_level + 1, PrintSide::Left);
        }
    }

    /* Cleanup */
    left_iterator.Finalize();
    right_iterator.Finalize();
}

void ProcessBarsSingle(void *file, u32 indent_level, PrintSide print_side) {

    /* Parse sarc directories */
    RomfsDirectoryParser iterator = {};
    if (iterator.InitializeByBars(file) == false)   { std::cout << "archive failure" << std::endl; return; }

    /* Create a sarc extractor */
    dd::res::BarsExtractor bars = {};

    if (bars.Initialize(file)   == false) { return; }

    /* Process every file embedded in the sarc */
    char **paths = iterator.GetFilePathArray();
    for (u32 i = 0; i < iterator.GetFileCount(); ++i) {
        /* Get path index */
        const u32 index = bars.FindEntryIndexByName(paths[i]);
        
        /* Get Amta file entries */
        dd::res::ResAmta *amta = bars.GetAmtaByIndex(index);
        
        if (amta->IsValid() == false) {
            ::puts("Invalid bamta");
            continue;
        }

        const u32 amta_size = amta->file_size;

        /* Process Amta */
        char path[MAX_PATH] = {};
        ::snprintf(path, sizeof(path), "%s.bamta", paths[i]);
        ProcessSingleImpl(reinterpret_cast<void*>(amta), amta_size, path, indent_level, print_side);
        
        /* Get Bwav file entries */
        dd::res::ResBwav *bwav = bars.GetBwavByIndex(index);
        
        if (bwav->IsValid() == false) {
            ::puts("Invalid bwav");
            continue;
        }

        const u32 bwav_size = CalculateBwavSize(bwav);

        /* Process bwav */
        if (bwav->is_prefetch == true) {
            ::snprintf(path, sizeof(path), "%s.prefetch.bwav", paths[i]);
        } else {
            ::snprintf(path, sizeof(path), "%s.bwav", paths[i]);
        }

        ProcessSingleImpl(reinterpret_cast<void*>(bwav), bwav_size, path, indent_level, print_side);
    }
        
    /* Cleanup */
    iterator.Finalize();
}

   