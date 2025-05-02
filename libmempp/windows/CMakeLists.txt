target_sources(${LIBRARY_TARGET_NAME}
    PRIVATE
        Process.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}
        FILES
            Process.hpp
)
