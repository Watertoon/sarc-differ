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

void DiffBntx(void *l_file, void *r_file, u32 indent_level) {

    /* Rescast */
    dd::res::ResBntx *l_bntx = dd::res::ResBntx::ResCast(l_file);
    dd::res::ResBntx *r_bntx = dd::res::ResBntx::ResCast(r_file);

    /* Create directory iter */
    RomfsDirectoryParser l_iterator = {};
    RomfsDirectoryParser r_iterator = {};
    l_iterator.InitializeByResDic(l_bntx->container.texture_dictionary);
    r_iterator.InitializeByResDic(r_bntx->container.texture_dictionary);

    /* Print diff textures */
    char **l_paths = l_iterator.GetFilePathArray();
    char **r_paths = r_iterator.GetFilePathArray();
    for (u32 i = 0; i < l_iterator.GetFileCount(); ++i) {
        u32 r_index = r_iterator.FindPathIndex(l_paths[i]);
        if (r_index != RomfsDirectoryParser::InvalidIndex) {

            /* Get texture infos */
            dd::res::ResBntxTextureInfo *l_tex_info = l_bntx->FindTexture(l_paths[i]);
            dd::res::ResBntxTextureInfo *r_tex_info = r_bntx->FindTexture(r_paths[r_index]);

            /* Check whether textures are different */
            if (l_tex_info->texture_info.mip_levels == r_tex_info->texture_info.mip_levels) {
                /* Iterate mips */
                bool is_same = true;
                for (u32 y = 0; y < l_tex_info->texture_info.mip_levels; ++y) {

                    /* Calculate texture sizes */
                    const u32 l_width  = ((l_tex_info->texture_info.width >> y) != 0)  ? (l_tex_info->texture_info.width >> y)  : 1;
                    const u32 l_height = ((l_tex_info->texture_info.height >> y) != 0) ? (l_tex_info->texture_info.height >> y) : 1;
                    const u32 l_size = dd::res::CalculateGfxImageSize(static_cast<dd::res::GfxChannelFormat>((l_tex_info->texture_info.image_format >> 8) & 0xff), l_width, l_height, l_tex_info->texture_info.depth);
                    const u32 r_width  = ((r_tex_info->texture_info.width >> y) != 0)  ? (r_tex_info->texture_info.width >> y)  : 1;
                    const u32 r_height = ((r_tex_info->texture_info.height >> y) != 0) ? (r_tex_info->texture_info.height >> y) : 1;
                    const u32 r_size = dd::res::CalculateGfxImageSize(static_cast<dd::res::GfxChannelFormat>((r_tex_info->texture_info.image_format >> 8) & 0xff), r_width, r_height, r_tex_info->texture_info.depth);

                    /* memcmp */
                    if (l_size == r_size) {
                        const s32 result = ::memcmp(l_tex_info->mip_offset_table[y], r_tex_info->mip_offset_table[y], l_size);
                        if (result != 0) {is_same = false; break;}
                    } else { is_same = false; break; }
                }
                if (is_same == true) { continue; }
            }

            /* Print diff */
            PrintIndent(indent_level + 1);
            std::cout <<  "Different(left: 0x"  << std::setfill('0') << std::setw(8) << l_bntx->FindTexture(l_paths[i])->mipmap_total_size << " bytes)(right: 0x"  << std::setfill('0') << std::setw(8) << r_bntx->FindTexture(r_paths[r_index])->mipmap_total_size << " bytes): " << r_paths[r_index]  << ".ftex" << std::endl;
        }
    }

    /* Print right only */
    for (u32 i = 0; i < r_iterator.GetFileCount(); ++i) {
        u32 l_index = l_iterator.FindPathIndex(r_paths[i]);
        if (l_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout <<  "Right only(size: 0x" << std::setfill('0') << std::setw(8) << r_bntx->FindTexture(r_paths[i])->mipmap_total_size << " bytes): " << r_paths[i]  << ".ftex" << std::endl;
        }
    }

    /* Print left only */
    for (u32 i = 0; i < l_iterator.GetFileCount(); ++i) {
        u32 r_index = r_iterator.FindPathIndex(l_paths[i]);
        if (r_index == RomfsDirectoryParser::InvalidIndex) {
            PrintIndent(indent_level + 1);
            std::cout <<  "Left only (size: 0x" << std::setfill('0') << std::setw(8) << l_bntx->FindTexture(l_paths[i])->mipmap_total_size << " bytes): " << l_paths[i]  << ".ftex" << std::endl;
        }
    }

    /* Cleanup */
    l_iterator.Finalize();
    r_iterator.Finalize();
}

void ProcessBntxSingle(void *file, u32 indent_level, PrintSide print_side) {

    /* Rescast */
    dd::res::ResBntx *bntx = dd::res::ResBntx::ResCast(file);

    /* Create directory iter */
    RomfsDirectoryParser iterator = {};
    iterator.InitializeByResDic(bntx->container.texture_dictionary);

    /* Print texture names */
    char **paths = iterator.GetFilePathArray();
    for (u32 i = 0; i < iterator.GetFileCount(); ++i) {
        PrintIndent(indent_level);
        PrintOnlySide(print_side);
        std::cout << "(size: 0x" << std::setfill('0') << std::setw(8) << bntx->FindTexture(paths[i])->mipmap_total_size << " bytes): " << paths[i] << ".ftex" << std::endl;
    }

    /* Cleanup */
    iterator.Finalize();
}
