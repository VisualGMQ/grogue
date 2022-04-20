macro(ADD_CORE_TEST NAME)
    add_executable(${NAME} ${NAME}.cpp)
    add_test(NAME ${NAME} COMMAND ./${NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/build)
    target_link_libraries(${NAME} PRIVATE ${GROGUE_CORE_LIB_NAME})
    target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rdlib)

    set_target_properties(${NAME} PROPERTIES FOLDER "unittest")
    target_precompile_headers(${NAME} REUSE_FROM ${PROJECT_NAME})
endmacro()

ADD_CORE_TEST(mathf)
ADD_CORE_TEST(texture)
ADD_CORE_TEST(utf8string)
