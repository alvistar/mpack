add_library(mpack mpack-common.c mpack-reader.c mpack-platform.c mpack-expect.c mpack-node.c mpack-writer.c)
target_include_directories(mpack PUBLIC ..)