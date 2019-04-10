file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_library(Common STATIC ${SOURCES} ${HEADERS} "${PROJECT_BINARY_DIR}/generated/cmake_vars.cpp")

create_target_directory_groups(Common)