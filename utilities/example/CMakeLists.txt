aux_source_directory(./ EXAMPLE_SRC)
add_executable(example ${EXAMPLE_SRC})
target_link_libraries(example PRIVATE ${GROGUELIB_NAME} nuklear)
CopyDLL(example)