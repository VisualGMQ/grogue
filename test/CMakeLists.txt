macro(AddTest name files)
    add_executable(${name}_test ${files})    
    target_include_directories(${name}_test PRIVATE ./)
    target_link_libraries(${name}_test PRIVATE ${LOWLEVEL_NAME})
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}_test>)
endmacro()

# add your test here:
AddTest(example example.cpp)
AddTest(vector3 t_vector3.cpp)
