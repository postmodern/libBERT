cmake_minimum_required(VERSION 2.6)
project(BERT C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(install_paths)

set(LIBRARY_VERSION "0.1.0")
set(LIBRARY_SOVERSION "0")
set(
	BERT_FILES
	src/errno.c src/util.c src/list.c src/dict.c
	src/private/regex.c src/private/data.c src/data.c
	src/private/decode.c src/private/decoder.c src/decoder.c
	src/private/encode.c src/private/encoder.c src/encoder.c
	src/bert.c
)
option(BERT_PCRE "Enable the use of PCRE Options in bert/regex.h")
option(BERT_DEBUG "Enable debugging information in libBERT")

configure_file(config.h.cmake ${BERT_SOURCE_DIR}/include/bert/config.h)

if(UNIX)
	add_definitions("-Wall")

	if(BERT_DEBUG)
		add_definitions("-ggdb3")
	endif(BERT_DEBUG)
endif(UNIX)

include_directories(${BERT_SOURCE_DIR}/include)

add_library(BERT SHARED ${BERT_FILES})
set_target_properties(
	BERT PROPERTIES
	VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
)

add_library(BERT-static STATIC ${BERT_FILES})
set_target_properties(
	BERT-static PROPERTIES
	VERSION ${LIBRARY_VERSION}
	SOVERSION ${LIBRARY_SOVERSION}
)

add_executable(bert_dump src/programs/dump.c)
target_link_libraries(bert_dump BERT)

if(UNIX)
	configure_file(libBERT.pc.cmake ${BERT_SOURCE_DIR}/libBERT.pc)
endif(UNIX)

install(DIRECTORY include/bert DESTINATION ${INCLUDE_INSTALL_DIR})
install(
	TARGETS BERT
	LIBRARY DESTINATION ${LIB_INSTALL_DIR}
)
install(
	TARGETS BERT-static
	ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
)
install(
	TARGETS bert_dump
	RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)

if(UNIX)
	install(FILES libBERT.pc DESTINATION ${PKGCONFIG_INSTALL_DIR})
endif(UNIX)

add_subdirectory(test)
