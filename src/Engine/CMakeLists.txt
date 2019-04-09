file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_executable(Engine ${SOURCES} ${HEADERS})

file(GLOB_RECURSE EXT_LIBS "${PROJECT_ROOT_DIR}/lib-ext/*.lib")
target_link_libraries(Engine Common GLCore ${EXT_LIBS})

create_target_directory_groups(Engine)