add_executable(test_mempp main.cpp)

add_executable(target target.cpp)

target_link_libraries(test_mempp
    PRIVATE
        ${LIBRARY_TARGET_NAME}
)
