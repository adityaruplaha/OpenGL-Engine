file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_library(GLCore STATIC ${SOURCES} ${HEADERS} "glad.c")

target_link_libraries(GLCore glfw dl)

create_target_directory_groups(GLCore)