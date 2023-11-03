cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20
    CACHE STRING "Cpp standard.")

project(mempp)

set(LIBRARY_TARGET_NAME "mempp"
    CACHE STRING "Library target name.")

set(STATIC_LIBRARY OFF
    CACHE BOOL "Build static library.")

# Set to "ON" to debug makefile commands
set(CMAKE_VERBOSE_MAKEFILE ON
     CACHE BOOL "Show makefiles commands on build.")

if(STATIC_LIBRARY EQUAL ON)
    message(STATUS "Build: Static")
    add_library(${LIBRARY_TARGET_NAME} STATIC)
else()
    message(STATUS "Build: Shared")
    add_library(${LIBRARY_TARGET_NAME} SHARED)
    target_compile_options(${LIBRARY_TARGET_NAME}
        PRIVATE
            -fPIC)
endif()

# Add extra flags to the compiler
add_compile_options(-Werror -Wall -Wextra)

# In case of building in debug it spreds that definition to all targets
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
     add_compile_definitions("DEBUG")
endif()

install(TARGETS ${LIBRARY_TARGET_NAME}
    EXPORT "${LIBRARY_TARGET_NAME}Config"
    DESTINATION lib
    INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

install(EXPORT "${LIBRARY_TARGET_NAME}Config"
    FILE "${LIBRARY_TARGET_NAME}Config.cmake"
    NAMESPACE mempp::
    DESTINATION cmake)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
    DESTINATION include/mempp
    FILES_MATCHING
    PATTERN "*.hpp")

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

add_executable(test_mempp main.cpp)

add_executable(target target.cpp)

add_subdirectory(src)

target_link_libraries(test_mempp ${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug/libmempp.so)
