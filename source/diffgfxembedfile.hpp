#pragma once

bool DiffGfxEmbedFile(dd::res::ResGfxEmbedFile *l_embed_file, dd::res::ResGfxEmbedFile *r_embed_file, const char *path, u32 indent_level, bool is_print) {

	if (is_print == false) {
        return (l_embed_file->file_size == r_embed_file->file_size) && ::memcmp(l_embed_file->file_offset, r_embed_file->file_offset, r_embed_file->file_size);
    }
    ProcessFilesImpl(l_embed_file->file_offset, l_embed_file->file_size, r_embed_file->file_offset, r_embed_file->file_size, path, indent_level);
    return false;
}

//bool DiffGfxUserData();
