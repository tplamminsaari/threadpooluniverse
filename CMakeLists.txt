cmake_minimum_required(VERSION 3.15)
project(threadpooluniverselib)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Get gTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
)
FetchContent_MakeAvailable(googletest)

# Define compiler requirements and flags.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Helper function to group source code files in IDE.
function(auto_source_group sources base_dir)
    foreach(_source IN LISTS ${sources})
        get_filename_component(_abs_source "${_source}" ABSOLUTE)
        file(RELATIVE_PATH _rel_path "${base_dir}" "${_abs_source}")
        get_filename_component(_rel_dir "${_rel_path}" PATH)
        if(_rel_dir AND NOT _rel_dir STREQUAL ".")
            string(REPLACE "/" "\\" _group_path "${_rel_dir}")
            source_group("${_group_path}" FILES "${_source}")
        endif()
    endforeach()
endfunction()

# Compile the library.
file(GLOB_RECURSE PROJECT_SOURCE_FILES "include/*h" "src/*.h" "src/*.cpp")
auto_source_group(PROJECT_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(threadpooluniverselib STATIC ${PROJECT_SOURCE_FILES})

# Gather all the test files and build the tests.
file(GLOB_RECURSE TEST_FILES "src_test/*.h" "src_test/*.cpp")
auto_source_group(TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(threadpooluniverselib_test ${TEST_FILES})

target_include_directories(threadpooluniverselib_test PUBLIC
    src
    src_test
    include
)
target_link_libraries(threadpooluniverselib_test threadpooluniverselib gtest)

include(GoogleTest)
gtest_discover_tests(threadpooluniverselib_test)

get_property(all_targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
foreach(target ${all_targets})
    set_property(TARGET ${target} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endforeach()