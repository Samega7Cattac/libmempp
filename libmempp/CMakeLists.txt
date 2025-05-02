target_sources(${LIBRARY_TARGET_NAME}
    PRIVATE
        ProcessInterface.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}
        FILES
            Process.hpp
            ProcessInterface.hpp
)

if (UNIX)
    add_subdirectory(unix)
elseif (WIN32)
    add_subdirectory(windows)
endif ()
