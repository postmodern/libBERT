cmake_minimum_required(VERSION 2.6)
project(BERT C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(install_paths)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

configure_file(config.h.cmake ${BERT_SOURCE_DIR}/include/bert/config.h)

include_directories(${BERT_SOURCE_DIR}/include)

add_definitions("-Wall")
add_library(BERT SHARED src/errno.c src/util.c src/buffer.c src/list.c src/dict.c src/data.c src/decoder_private.c src/decoder.c src/bert.c)

add_executable(bert_dump src/utils/dump.c)
target_link_libraries(bert_dump BERT m)

install(TARGETS BERT
	LIBRARY DESTINATION ${LIB_INSTALL_DIR})
install(DIRECTORY include/bert DESTINATION ${INCLUDE_INSTALL_DIR})
