#pragma once

bool DiffBfresBone(dd::res::ResBfresBone *l_bone, dd::res::ResBfresBone *r_bone, u32 indent_level, bool is_print) {

    /* Compare transform mode */
    
    /* Compare Hierarchy transform mode */
    
    /*  */
}

bool DiffBfresSkeleton(dd::res::ResBfresSkeleton *l_skeleton, dd:;res::ResBfresSkeleton *r_skeleton, u32 indent_level, bool is_print) {

    /* Compare mirror mode */
    
    /* Compare scale mode */
    
    /* Compare rotation mode */

    /* Compare bone count */

    /* Compare bones */
    RomfsDirectoryParser l_bone_iterator = {};
    l_bone_iterator.InitializeByResDic(l_skeleton->bone_dictionary);
    RomfsDirectoryParser r_bone_iterator = {};
    r_bone_iterator.InitializeByResDic(r_skeleton->bone_dictionary);

    

    return true;
}
