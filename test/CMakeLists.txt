macro(ADD_CORE_TEST NAME)
    add_executable(${NAME} ${NAME}.cpp)
    add_test(NAME ${NAME} COMMAND ./${NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/build)
    target_link_libraries(${NAME} PRIVATE ${GROGUE_CORE_NAME})
    target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rdlib)

    set_target_properties(${NAME} PROPERTIES FOLDER "unittest")
    target_precompile_headers(${NAME} REUSE_FROM ${GROGUE_CORE_NAME})
endmacro()

macro(ADD_PHYSICAL_TEST NAME)
    add_executable(${NAME} ${NAME}.cpp)
    add_test(NAME ${NAME} COMMAND ./${NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/build)
    target_link_libraries(${NAME} PRIVATE ${GROGUE_PHYSICAL_NAME})
    target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/3rdlib)

    set_target_properties(${NAME} PROPERTIES FOLDER "unittest")
    target_precompile_headers(${NAME} REUSE_FROM ${GROGUE_PHYSICAL_NAME})
endmacro()

ADD_CORE_TEST(mathf)
ADD_CORE_TEST(image)
ADD_CORE_TEST(utf8string)
ADD_CORE_TEST(animation)

ADD_PHYSICAL_TEST(physics)
