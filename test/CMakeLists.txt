IsMinGWBackend(is_mingw_backend)

macro(AddTest name files)
    add_executable(${name}_test ${files})    
    target_include_directories(${name}_test PRIVATE ./ ${CMAKE_BINARY_DIR})
    target_link_libraries(${name}_test PRIVATE ${GROGUE_LOWLEVELLIB_NAME})
    add_test(NAME ${name}
             COMMAND $<TARGET_FILE:${name}_test>)
endmacro()

# use this macro to add your test
macro(AddSimpleTest name files)
    AddTest(${name} ${files})
    set_property(TEST ${name} PROPERTY LABELS NoInteractive)
endmacro()

# use this macro to add your interactive test
macro(AddInteractiveTest name files)
    AddTest(${name} ${files})
    set_property(TEST ${name} PROPERTY LABELS Interactive)
endmacro()


set(TEST_RESOURCE_DIR ${CMAKE_SOURCE_DIR}/test/resources/)
configure_file(${CMAKE_SOURCE_DIR}/test/test_helper.inc
               ${CMAKE_BINARY_DIR}/test_helper.hpp)

# add your test here:
AddSimpleTest(example example.cpp)
AddSimpleTest(math math.cpp)
AddSimpleTest(handle handle.cpp)
AddSimpleTest(lua lua.cpp)
AddSimpleTest(memory_recycler memory_recycler.cpp)
AddSimpleTest(ecs ecs.cpp)
AddSimpleTest(signal signal.cpp)
AddInteractiveTest(input input.cpp)
AddInteractiveTest(tilesheet tilesheet.cpp)
AddInteractiveTest(animation animation.cpp)
AddInteractiveTest(shapes shapes.cpp)
AddInteractiveTest(hierarchy hierarchy.cpp)
AddInteractiveTest(ui ui.cpp)
AddInteractiveTest(physics physics.cpp)

# CopyDefScript(lua_binding_test)
CopyDLL(example_test)
