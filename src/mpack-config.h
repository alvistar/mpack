
/**
 * This is a sample MPack configuration file. Copy it to mpack-config.h somewhere
 * in your project's include tree and, optionally, edit it to suit your setup.
 *
 * In most cases you can leave this file with the default config.
 *
 * You can also override the default configuration by pre-defining options to 0 or 1.
 */
#include <stdlib.h>
#include <string.h>

#ifndef MPACK_CONFIG_H
#define MPACK_CONFIG_H 1
#endif


/*
 * Features
 */

/** Enables compilation of the base Tag Reader. */
#ifndef MPACK_READER
#define MPACK_READER 1
#endif

/** Enables compilation of the static Expect API. */
#ifndef MPACK_EXPECT
#define MPACK_EXPECT 1
#endif

/** Enables compilation of the dynamic Node API. */
#ifndef MPACK_NODE
#define MPACK_NODE 1
#endif

/** Enables compilation of the Writer. */
#ifndef MPACK_WRITER
#define MPACK_WRITER 1
#endif


/*
 * Dependencies
 */

/**
 * Enables the use of C stdlib. This allows the library to use malloc
 * for debugging and in allocation helpers.
 */
#ifndef MPACK_STDLIB
#define MPACK_STDLIB 1
#endif

/**
 * Enables the use of C stdio. This adds helpers for easily
 * reading/writing C files and makes debugging easier.
 */
#ifndef MPACK_STDIO
#define MPACK_STDIO 0
#endif


/*
 * System Functions
 */

/**
 * \def MPACK_MALLOC
 *
 * Defines the memory allocation function used by mpack. This is used by
 * helpers for automatically allocating data the correct size, and for
 * debugging functions. If this macro is undefined, the allocation helpers
 * will not be compiled.
 *
 * The default is malloc() if MPACK_STDLIB is enabled.
 */
/**
 * \def MPACK_REALLOC
 *
 * Defines the realloc function used by mpack. It is used by growable
 * buffers to resize more efficiently.
 *
 * The default is realloc() if MPACK_MALLOC has not been customized and
 * MPACK_STDLIB is enabled.
 *
 * This is optional, even when MPACK_MALLOC is used. If MPACK_MALLOC is
 * set and MPACK_REALLOC is not, MPACK_MALLOC is used with a simple copy
 * to grow buffers.
 */
#if defined(MPACK_STDLIB) && !defined(MPACK_MALLOC)
#define MPACK_MALLOC malloc
//#define MPACK_REALLOC realloc
#endif

/**
 * \def MPACK_FREE
 *
 * Defines the memory free function used by mpack. This is used by helpers
 * for automatically allocating data the correct size. If this macro is
 * undefined, the allocation helpers will not be compiled.
 *
 * The default is free() if MPACK_STDLIB is enabled.
 */
#if defined(MPACK_STDLIB) && !defined(MPACK_FREE)
#define MPACK_FREE free
#endif


/*
 * Debugging options
 */

/**
 * \def MPACK_DEBUG
 *
 * Enables debug features. You may want to wrap this around your
 * own debug preprocs. By default, they are enabled if DEBUG or _DEBUG
 * are defined.
 *
 * Note that MPACK_DEBUG cannot be defined differently for different
 * source files because it affects layout of structs defined in header
 * files. Your entire project must be compiled with the same value of
 * MPACK_DEBUG. (This is why NDEBUG is not used.)
 */
#if !defined(MPACK_DEBUG) && (defined(DEBUG) || defined(_DEBUG))
#define MPACK_DEBUG 1
#endif

/**
 * Set this to 1 to implement a custom mpack_assert_fail() function. This
 * function must not return, and must have the following signature:
 *
 *     void mpack_assert_fail(const char* message)
 *
 * Asserts are only used when MPACK_DEBUG is enabled, and can be triggered
 * by bugs in mpack or bugs due to incorrect usage of mpack.
 */
#ifndef MPACK_CUSTOM_ASSERT
#define MPACK_CUSTOM_ASSERT 0
#endif

/**
 * \def MPACK_READ_TRACKING
 *
 * Enables compound type size tracking for readers. This ensures that the
 * correct number of elements or bytes are read from a compound type.
 *
 * This is enabled by default in debug builds (provided a malloc() is
 * available.)
 */
#if !defined(MPACK_READ_TRACKING) && \
        defined(MPACK_DEBUG) && MPACK_DEBUG && \
        defined(MPACK_READER) && MPACK_READER && \
        defined(MPACK_MALLOC)
#define MPACK_READ_TRACKING 1
#endif

/**
 * \def MPACK_WRITE_TRACKING
 *
 * Enables compound type size tracking for writers. This ensures that the
 * correct number of elements or bytes are written in a compound type.
 *
 * Note that without write tracking enabled, it is possible for buggy code
 * to emit invalid MessagePack without flagging an error by writing the wrong
 * number of elements or bytes in a compound type. With tracking enabled,
 * MPACK will catch such errors and break on the offending line of code.
 *
 * This is enabled by default in debug builds (provided a malloc() is
 * available.)
 */
#if !defined(MPACK_WRITE_TRACKING) && \
        defined(MPACK_DEBUG) && MPACK_DEBUG && \
        defined(MPACK_WRITER) && MPACK_WRITER && \
        defined(MPACK_MALLOC)
#define MPACK_WRITE_TRACKING 1
#endif


/*
 * Miscellaneous
 */

/**
 * Whether to optimize for size or speed. Optimizing for size causes
 * very few functions to be declared inline, and can save a couple
 * kilobytes of space in the resulting executable.
 *
 * This automatically detects -Os with GCC/Clang. Unfortunately there
 * doesn't seem to be a macro defined for /Os under MSVC.
 *
 * This feature is currently experimental and may be removed in a
 * future release.
 */
#ifndef MPACK_OPTIMIZE_FOR_SIZE
#ifdef __OPTIMIZE_SIZE__
#define MPACK_OPTIMIZE_FOR_SIZE 1
#else
#define MPACK_OPTIMIZE_FOR_SIZE 0
#endif
#endif

/**
 * Stack space to use when initializing a reader or writer with a
 * stack-allocated buffer.
 */
#ifndef MPACK_STACK_SIZE
#define MPACK_STACK_SIZE 4096
#endif

/**
 * Buffer size to use for allocated buffers (such as for a file writer.)
 */
#ifndef MPACK_BUFFER_SIZE
#define MPACK_BUFFER_SIZE 1024

/**
 * Number of nodes in each allocated node page.
 *
 * Nodes are 16 bytes when compiled for a 32-bit architecture and
 * 24 bytes when compiled for a 64-bit architecture.
 *
 * Using as many nodes fit in one memory page seems to provide the
 * best performance, and has very little waste when parsing small
 * messages.
 */
#ifndef MPACK_NODE_PAGE_SIZE
#define MPACK_NODE_PAGE_SIZE (4096 / sizeof(mpack_node_t))
#endif

/**
 * The initial depth for the node parser. When MPACK_MALLOC is available,
 * the node parser has no practical depth limit, and it is not recursive
 * so there is no risk of overflowing the call stack.
 */
#ifndef MPACK_NODE_INITIAL_DEPTH
#define MPACK_NODE_INITIAL_DEPTH 8
#endif

/**
 * The maximum depth for the node parser if MPACK_MALLOC is not available.
 * The parsing stack is placed on the call stack.
 */
#ifndef MPACK_NODE_MAX_DEPTH_WITHOUT_MALLOC
#define MPACK_NODE_MAX_DEPTH_WITHOUT_MALLOC 32
#endif


#endif

