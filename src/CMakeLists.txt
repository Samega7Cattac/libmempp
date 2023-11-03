message(STATUS "Configuring mempp")

target_sources(${LIBRARY_TARGET_NAME}
    PRIVATE
        Process.hpp
        ProcessInterface.cpp ProcessInterface.hpp)

add_subdirectory(unix)

add_subdirectory(windows)
