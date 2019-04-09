cmake_minimum_required(VERSION 3.8)
project("OpenGL Engine")

function(create_target_directory_groups target_name)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    get_target_property(target_sources "${target_name}" SOURCES)

    foreach(file_name IN LISTS target_sources)
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include_directories("include-ext")
include_directories("${PROJECT_BINARY_DIR}/generated")

link_directories("lib-ext")
link_directories(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

# Get git branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Branch Name: ${GIT_BRANCH}")

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Commit Hash: ${GIT_COMMIT}")

# Set project root directory
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})
message(STATUS "Project Root: ${PROJECT_ROOT_DIR}")

configure_file("src/Common/cmake_vars.cpp.in" "${PROJECT_BINARY_DIR}/generated/cmake_vars.cpp")

add_subdirectory("src")

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Engine)