cmake_minimum_required(VERSION 3.20)

# Define variables for build type
option(LIBMEMPP_BUILD_STATIC "Build libmempp static" OFF)
option(LIBMEMPP_BUILD_SHARED "Build libmempp shared" ON)

# CMakeScripts includes
include(cmake/cpp_flags.cmake)

# Set the minimum standard
set(CMAKE_CXX_STANDARD 20)

# Set to "ON" to debug makefile commands
set(CMAKE_VERBOSE_MAKEFILE ON
        CACHE BOOL "Show makefiles commands on build.")

set(LIBRARY_TARGET_NAME "mempp"
        CACHE STRING "Library target name")

project(mempp)

if (LIBMEMPP_BUILD_STATIC AND NOT LIBMEMPP_BUILD_SHARED)
    add_library(${LIBRARY_TARGET_NAME} STATIC)
elseif (LIBMEMPP_BUILD_SHARED AND NOT LIBMEMPP_BUILD_STATIC)
    add_library(${LIBRARY_TARGET_NAME} SHARED)
    target_compile_options(${LIBRARY_TARGET_NAME}
        PRIVATE
            -fPIC
    )
else ()
    message(FATAL_ERROR "Libmempp build type not set or invalid!\nEither LIBMEMPP_BUILD_STATIC OR LIBMEMPP_BUILD_SHARED to ON!")
endif ()

SetMyDefaultFlags(${LIBRARY_TARGET_NAME})

# Custom flags
target_compile_options(${LIBRARY_TARGET_NAME}
    PRIVATE
        -Wno-format # Complains about converting hex string to long unsigned int into a long int
    INTERFACE
        -fpermissive # Needed for some casts that may lose precision (requires close attention for now)
)

add_subdirectory(libmempp)

add_subdirectory(tests)

include(GNUInstallDirs)

install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT "${LIBRARY_TARGET_NAME}Config"
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT "${LIBRARY_TARGET_NAME}Config"
        FILE "${LIBRARY_TARGET_NAME}Config.cmake"
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake
)
