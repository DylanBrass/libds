cmake_minimum_required(VERSION 4.1)
project(DataStructures C)

set(CMAKE_C_STANDARD 23)

add_library(ds STATIC
        src/dynamic_array.c)

add_library(DataStructures::ds ALIAS ds)

target_include_directories(ds PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_compile_options(ds PRIVATE -Wall -Wextra -Wshadow -Wconversion)

if (PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_executable(test_dynamic_array tests/test_dynamic_array.c)
    target_link_libraries(test_dynamic_array PRIVATE ds)
    target_compile_options(test_dynamic_array PRIVATE -fsanitize=address,undefined -g)
    target_link_options(test_dynamic_array PRIVATE -fsanitize=address,undefined)
    add_test(NAME vector COMMAND test_dynamic_array)
endif ()