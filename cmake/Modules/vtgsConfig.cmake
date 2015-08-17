INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_VTGS vtgs)

FIND_PATH(
    VTGS_INCLUDE_DIRS
    NAMES vtgs/api.h
    HINTS $ENV{VTGS_DIR}/include
        ${PC_VTGS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    VTGS_LIBRARIES
    NAMES gnuradio-vtgs
    HINTS $ENV{VTGS_DIR}/lib
        ${PC_VTGS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(VTGS DEFAULT_MSG VTGS_LIBRARIES VTGS_INCLUDE_DIRS)
MARK_AS_ADVANCED(VTGS_LIBRARIES VTGS_INCLUDE_DIRS)

