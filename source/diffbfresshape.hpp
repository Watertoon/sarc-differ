#pragma once

bool DiffBfresShape(dd::res::ResBfresShape *l_shape, dd::res::ResBfresShape *r_shape, u32 indent_level, bool is_print) {
    
    /* Diff name */
    DIFF_ASSERT(::strcmp(l_shape->shape_name + 2, r_shape->shape_name + 2) == 0, 
                "Different names (left: \"%s\")(right: \"%s\")", l_shape->shape_name + 2, r_shape->shape_name + 2);


    /* Check for ResBfresVertex */
    if (l_shape->has_vertex_buffer != r_shape->has_vertex_buffer) {
        CONDITIONAL_DIFF(print_diff, indent_level, "Different has_vertex_buffer (left: %d)(right: %d)", l_shape->has_vertex_buffer, r_shape->has_vertex_buffer);

        /* Print single */
        if (is_print == false) { return false; }
        if (l_shape->has_vertex_buffer == 1) {
            DiffBfresVertexSingle(l_shape->shape_vertex, indent_level + 1);
        } else {
            DiffBfresVertexSingle(r_shape->shape_vertex, indent_level + 1);
        }
    }

    /* Diff ResBfresVertex */
    const bool diff2 = DiffBfresVertex(l_shape->shape_vertex, r_shape->shape_vertex, 0, false);
    if (diff2 == false) {
        CONDITIONAL_DIFF(print_diff, indent_level, "Different ResBfresVertex");
        DiffBfresVertex(l_shape->shape_vertex, r_shape->shape_vertex, indent_level + 1, true);
    }

    /* Diff ResBfresMesh */
    CONDITIONAL_DIFF(print_diff, indent_level, "Different mesh count (left: %s)(right %d)", ,);
    for () {
        
    }
    const bool diff2 = DiffBfresMesh(l_shape->shape_vertex, r_shape->shape_vertex, 0, false);
    if (diff2 == false) {
        CONDITIONAL_DIFF(print_diff, indent_level, "Different ResBfresMesh");
        DiffBfresVertex(l_shape->shape_vertex, r_shape->shape_vertex, indent_level + 1, true);
    }

    return false
}
