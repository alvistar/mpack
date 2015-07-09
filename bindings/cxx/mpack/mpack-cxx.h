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

/**
 * @file
 *
 * Defines the C++ interface to MPack.
 */

#ifndef MPACK_CXX_H
#define MPACK_CXX_H 1

#include "mpack.h"

#if defined(__cplusplus)
#if MPACK_CXX

#if __cplusplus >= 201103L
#define MPACK_CXX11 1
#endif

#if MPACK_CXX_STL
#include <string>
#include <vector>
#endif

#if MPACK_SETJMP
#define mpack_assert_no_jump(obj) \
    mpack_assert((obj)->jump_env == NULL, "cannot use C++ bindings with setjmp/longjmp!")
#else
#define mpack_assert_no_jump(obj) ((void)(obj))
#endif



/*
 * First we add C++ type support to the existing C/C++ readers/writers
 */

#if MPACK_CXX_STL
#if MPACK_READER
/**
 * Reads count bytes from a string, binary blob or extension object, returning
 * a vector containing the bytes.
 *
 * One of these types must have been opened for reading via mpack_read_tag()
 * or an expect function.
 *
 * @see mpack_read_bytes()
 * @see mpack_expect_str()
 * @see mpack_expect_bin()
 * @see mpack_expect_ext()
 */
static inline std::vector<char> mpack_read_bytes_vector(mpack_reader_t* reader, size_t count) {
    mpack_assert_no_jump(reader);
    std::vector<char> data(count);
    mpack_read_bytes(reader, &buffer[start], count);
    return data;
}

/**
 * Reads count bytes from a string, binary blob or extension object, writing
 * them into the given vector.
 *
 * One of these types must have been opened for reading via mpack_read_tag()
 * or an expect function.
 *
 * @see mpack_read_bytes()
 * @see mpack_expect_str()
 * @see mpack_expect_bin()
 * @see mpack_expect_ext()
 */
static inline void mpack_read_bytes_into_vector(mpack_reader_t* reader, size_t count, std::vector<char>& data) {
    mpack_assert_no_jump(reader);
    size_t start = buffer.size();
    buffer.resize(start + count);
    mpack_read_bytes(reader, &buffer[start], count);
}

/**
 * Reads count bytes from a string, returning a string containing the bytes.
 *
 * The string must have been opened for reading via mpack_read_tag() or an expect function.
 *
 * @see mpack_read_bytes()
 * @see mpack_expect_str()
 * @see mpack_expect_str_max()
 * @see mpack_expect_str_length()
 */
static inline std::string mpack_read_str_string(mpack_reader_t* reader, size_t count) {
    mpack_assert_no_jump(reader);
    if (mpack_should_read_bytes_inplace(reader, count))
        return std::string(mpack_read_bytes_inplace(reader, count));

    // not the most efficient way to do this, but we can't directly write into
    // a string either. we could read in blocks and append but it's probably
    // pointless, most reasonably-sized strings will be in-place.
    std::vector<char> data = mpack_read_bytes(reader, count);
    return std::string(data.data(), data.size());
}
#endif


#if MPACK_EXPECT
static inline std::string mpack_expect_str_string(mpack_reader_t* reader) {
    mpack_assert_no_jump(reader);
    uint32_t count = mpack_expect_str(reader);
    std::string str = mpack_read_str_string(reader);
    mpack_done_str(reader);
    return str;
}

static inline std::vector<char> mpack_expect_bin_vector(mpack_reader_t* reader) {
    mpack_assert_no_jump(reader);
    uint32_t count = mpack_expect_bin(reader);
    std::vector<char> data(count);
    mpack_read_bytes(reader, &buffer[start], count);
    mpack_done_bin(reader);
    return data;
}
#endif


#if MPACK_NODE
static inline std::string mpack_node_str_string(mpack_node_t* node) {
    mpack_assert_no_jump(node);
    size_t size = mpack_node_strlen(node);
    const char* data = mpack_node_data(node);
    if (!data)
        return std::string();
    return std::string(data, size);
}

static inline std::vector<char> mpack_node_bin_vector(mpack_node_t* node) {
    mpack_assert_no_jump(node);
    uint32_t count = mpack_node_bin(node);
    std::vector<char> data(count);
    mpack_read_bytes(node, &buffer[start], count);
    mpack_done_bin(node);
    return data;
}
#endif


#if MPACK_WRITER
static inline void mpack_write_str(mpack_writer_t* writer, const std::string& str) {
    mpack_assert_no_jump(writer);
    mpack_write_str(writer, str.data(), str.size());
}

static inline void mpack_write_bin(mpack_writer_t* writer, const std::vector<char>& data) {
    mpack_assert_no_jump(writer);
    mpack_write_bin(writer, data.data(), data.size());
}

static inline void mpack_write_bin(mpack_writer_t* writer, const std::vector<unsigned char>& data) {
    mpack_assert_no_jump(writer);
    mpack_write_bin(writer, static_cast<const char*>(data.data()), data.size());
}

static inline void mpack_write_bin(mpack_writer_t* writer, const std::vector<uint8_t>& data) {
    mpack_assert_no_jump(writer);
    mpack_write_bin(writer, static_cast<const char*>(data.data()), data.size());
}
#endif
#endif



/*
 * Next we define the C++ Reader, Writer and Node/Tree
 */

namespace mpack {


#if MPACK_WRITER
class Writer {
public:
    Writer(char* buffer, size_t size);
    Writer(mpack_error_t error);
    Writer(const char* filename);

private:
    mpack_writer_t writer;
};
#endif


#if MPACK_READER
class Reader {
public:
    Reader(char* buffer, size_t size);
    Reader(mpack_error_t error);
    Reader(const char* filename);

private:
    mpack_reader_t reader;
};
#endif


#if MPACK_NODE
class Tree;

/**
 * A Node is a value-type handle to a node in a parsed Tree.
 */
class Node {
public:
    const mpack_tag_t& tag() {return node->tag;}
    mpack_type_t type() {return mpack_node_type(node);}

    Tree& tree() {return *static_cast<Tree*>(node->tree->context);}


    // basic types

    void     nil() {return mpack_node_nil(node);}
    bool     b()   {return mpack_node_bool(node);}
    void     t()   {return mpack_node_true(node);}
    void     f()   {return mpack_node_false(node);}
    uint8_t  u8()  {return mpack_node_u8(node);}
    uint16_t u16() {return mpack_node_u16(node);}
    uint32_t u32() {return mpack_node_u32(node);}
    uint64_t u64() {return mpack_node_u64(node);}
    int8_t   i8()  {return mpack_node_i8(node);}
    int16_t  i16() {return mpack_node_i16(node);}
    int32_t  i32() {return mpack_node_i32(node);}
    int64_t  i64() {return mpack_node_i64(node);}
    float    f()   {return mpack_node_float(node);}
    double   d()   {return mpack_node_double(node);}

    operator bool()     {return b();}
    operator uint8_t()  {return u8();}
    operator uint16_t() {return u16();}
    operator uint32_t() {return u32();}
    operator uint64_t() {return u64();}
    operator int8_t()   {return i8();}
    operator int16_t()  {return i16();}
    operator int32_t()  {return i32();}
    operator int64_t()  {return i64();}
    operator float()    {return f();}
    operator double()   {return d();}


    // compound shared

    size_t count();
    operator size_t() {return count();}


    // str/bin/ext

    size_t len() {return mpack_node_data_len(node);}
    const char* data() {return mpack_node_data(node);}

    operator const char*() {return data();}

    #if MPACK_CXX_STL
    std::string str() {return mpack_node_str_string(node);}
    operator std::string() {return str();}
    #endif


    // map/array

    Node at(int64_t i);
    Node at(uint64_t i);
    Node at(size_t i);
    Node operator[](int64_t i) {return at(i);}
    Node operator[](uint64_t i) {return at(i);}
    Node operator[](size_t i) {return at(i);}

    Node at(const char* str, size_t len) {return mpack_node_map_str(node, str, len);}
    Node at(const char* str) {return at(str, strlen(str));}
    Node operator[](const char* str, size_t len) {return at(str, len);}
    Node operator[](const char* str) {return at(str);}

    #if MPACK_CXX_STL
    Node at(const std::string& str) {return at(str.c_str(), str.length());}
    Node operator[](const std::string& str) {return at(str);}
    #endif


private:
    friend class Tree;
    Node node(mpack_node_t* node) : node(node) {}
    mpack_node_t* node;
};

class Tree {
public:
    ~Tree() {mpack_tree_destroy(&tree);}

    Tree(const char* data, size_t length) {mpack_tree_init(&tree, data, length);}

    Tree fromData(const char* data, size_t length) {return Tree(data, length);}

    #if MPACK_STDIO
    Tree fromFile(const char* filename, size_t maxsize) {
        Tree tree;
        mpack_tree_init_from_file(tree.tree, filename, maxsize);
        return tree;
    }
    #endif

    Node root() {return mpack_tree_root(&tree);}
    mpack_error_t error() {return mpack_tree_error(&tree);}


    // Without C++11, copy operators are move operators like auto_ptr.

    #if MPACK_CXX11
    Tree(Tree&& other)
    #else
    Tree(Tree& other)
    #endif
        : tree(other.tree)
    {
        memset(0, &other.tree, sizeof(other.tree));
    }

    #if MPACK_CXX11
    Tree& operator=(Tree&& other)
    #else
    Tree& operator=(Tree& other)
    #endif
    {
        mpack_tree_t tree = other.tree;
        other.tree = this->tree;
        this->tree = tree;
    }


private:
    Tree() {
        #if MPACK_DEBUG
        // to make sure that all of our initialization makes sense, fill
        // this with non-zero values
        memset(&tree, 1, sizeof(tree));
        #endif
    }

    #if MPACK_CXX11
    Tree(const Tree& other) = delete;
    Tree& operator=(const Tree& other) = delete;
    #endif

    mpack_tree_t tree;
};
#endif


}

#endif
#endif

#endif


