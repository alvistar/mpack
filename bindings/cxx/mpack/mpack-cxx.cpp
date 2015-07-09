/*
 * Copyright (c) 2015 Nicholas Fraser
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mpack-cxx.h"

#if defined(__cplusplus)
#if MPACK_CXX

#if MPACK_NODE
size_t Node::count() {
    if (node->tree->error != mpack_ok)
        return &node->tree->nil_node;

    switch (mpack_node_type(node)) {
        case mpack_type_str:
        case mpack_type_bin:
        case mpack_type_ext:
            return mpack_node_data_len(node);
        case mpack_type_array:
            return mpack_node_array_length(node);
        case mpack_type_map:
            return mpack_node_map_count(node);
        default: break;
    }

    mpack_node_flag_error(node, mpack_error_type);
    return &node->tree->nil_node;
}

Node Node::at(int64_t i) {
    if (node->tree->error != mpack_ok)
        return &node->tree->nil_node;

    switch (mpack_node_type(node)) {
        case mpack_type_array:
            if (i < 0 || i > SIZE_MAX) {
                // this could be considered a bug
                mpack_node_flag_error(node, mpack_error_type);
                return &node->tree->nil_node;
            }
            return mpack_node_array_at(node, (size_t)i);

        case mpack_type_map:
            return mpack_node_map_int(node, i);

        default: break;
    }

    mpack_node_flag_error(node, mpack_error_type);
    return &node->tree->nil_node;
}

Node Node::at(uint64_t i) {
    if (node->tree->error != mpack_ok)
        return &node->tree->nil_node;

    switch (mpack_node_type(node)) {
        case mpack_type_array:
            if (i > SIZE_MAX) {
                // this could be considered a bug
                mpack_node_flag_error(node, mpack_error_type);
                return &node->tree->nil_node;
            }
            return mpack_node_array_at(node, (size_t)i);

        case mpack_type_map:
            return mpack_node_map_uint(node, i);

        default: break;
    }

    mpack_node_flag_error(node, mpack_error_type);
    return &node->tree->nil_node;
}

Node Node::at(size_t i) {
    if (node->tree->error != mpack_ok)
        return &node->tree->nil_node;

    switch (mpack_node_type(node)) {
        case mpack_type_array:
            return mpack_node_array_at(node, i);

        case mpack_type_map:
            return mpack_node_map_uint(node, i);

        default: break;
    }

    mpack_node_flag_error(node, mpack_error_type);
    return &node->tree->nil_node;
}

#endif

#endif
#endif
