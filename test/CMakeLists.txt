macro(AddTest name files)
    add_executable(${name}_test ${files})    
    target_include_directories(${name}_test PRIVATE ./)
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}_test>)
endmacro()

# add your test here:
AddTest(example example.cpp)