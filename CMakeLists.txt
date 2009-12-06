cmake_minimum_required(VERSION 2.6)
project(BERT C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(install_paths)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

configure_file(config.h.cmake ${BERT_SOURCE_DIR}/include/bert/config.h)

include_directories(${BERT_SOURCE_DIR}/include)

add_definitions("-Wall")
add_library(BERT SHARED src/read.c src/list.c src/dict.c src/data.c src/decoder.c src/bert.c)
