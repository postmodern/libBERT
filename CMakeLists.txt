cmake_minimum_required (VERSION 2.6)
project (BERT)

include_directories (${BERT_SOURCE_DIR}/include)
add_library (BERT src/bert.c)
