macro(AddTest name files)
    add_executable(${name}_test ${files})    
    target_include_directories(${name}_test PRIVATE ./)
    target_link_libraries(${name}_test PRIVATE ${LOWLEVEL_NAME})
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}_test>)
endmacro()

# add your test here:
AddTest(example example.cpp)
AddTest(math math.cpp)
AddTest(handle handle.cpp)
AddTest(lua lua.cpp)


set(TEST_RESOURCE_DIR ${CMAKE_SOURCE_DIR}/test/resources/)
configure_file(${CMAKE_SOURCE_DIR}/test/test_helper.inc
               ${CMAKE_SOURCE_DIR}/test/test_helper.hpp)