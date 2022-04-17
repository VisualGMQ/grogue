macro(ADD_CORE_TEST NAME)
    add_executable(${NAME} ${NAME}.cpp)
    target_link_libraries(${NAME} PRIVATE ${GROGUE_CORE_LIB_NAME})
endmacro()

ADD_CORE_TEST(mathf)
