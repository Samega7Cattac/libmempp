message(STATUS "Configuring for linux")

target_sources(${LIBRARY_TARGET_NAME}
    PRIVATE
        Process.cpp Process.hpp)
