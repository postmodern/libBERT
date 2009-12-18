if (UNIX)
  # Suffix for Linux
  set(LIB_SUFFIX
    CACHE STRING "Define suffix of directory name (32/64)"
  )

  set(EXEC_INSTALL_PREFIX
    "${CMAKE_INSTALL_PREFIX}"
    CACHE PATH  "Base directory for executables and libraries"
  )

  # The following are directories where stuff will be installed to
  set(BIN_INSTALL_DIR
    "${EXEC_INSTALL_PREFIX}/bin"
    CACHE PATH "The ${APPLICATION_NAME} binary install dir (default prefix/bin)"
  )
  set(LIB_INSTALL_DIR
    "${EXEC_INSTALL_PREFIX}/lib${LIB_SUFFIX}"
    CACHE PATH "The subdirectory relative to the install prefix where libraries will be installed (default is prefix/lib)"
  )
  set(INCLUDE_INSTALL_DIR
    "${CMAKE_INSTALL_PREFIX}/include"
    CACHE PATH "The subdirectory to the header prefix (default prefix/include)"
  )
  set(PKGCONFIG_INSTALL_DIR
    "${LIB_INSTALL_DIR}/pkgconfig"
    CACHE PATH "The subdirectory to the header prefix (default prefix/lib/pkgconfig)"
  )

endif (UNIX)

if (WIN32)
  # Same same
  set(BIN_INSTALL_DIR "." CACHE PATH "-")
  set(SBIN_INSTALL_DIR "." CACHE PATH "-")
  set(LIB_INSTALL_DIR "lib" CACHE PATH "-")
  set(INCLUDE_INSTALL_DIR "include" CACHE PATH "-")
endif (WIN32)
