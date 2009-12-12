prefix=${CMAKE_INSTALL_PREFIX}
libdir=${LIB_INSTALL_DIR}
includedir=${INCLUDE_INSTALL_DIR}
 
Name: libBERT
Description: BERT encoding/decoding C library
Version: ${LIBRARY_VERSION}
Libs: -L${LIB_INSTALL_DIR} -lBERT
Cflags: -I${INCLUDE_INSTALL_DIR} ${CFLAGS}
