file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_executable(Engine ${SOURCES} ${HEADERS})

target_link_libraries(Engine Common GLCore)

create_target_directory_groups(Engine)