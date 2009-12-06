cmake_minimum_required (VERSION 2.6)
project (BERT)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")

configure_file(config.h.cmake ${BERT_SOURCE_DIR}/include/bert/config.h)

include_directories (${BERT_SOURCE_DIR}/include)
add_library (BERT SHARED src/bert.c src/data.c src/decoder.c)
