file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_library(GLCore STATIC ${SOURCES} ${HEADERS} "glad.c")

target_link_libraries(GLCore glfw)

create_target_directory_groups(GLCore)