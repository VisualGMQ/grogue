function(AttachCppCheck)
    find_program(CPPCHECK NAMES cppcheck)
    if (CPPCHECK)
        set(CMAKE_CXX_CPPCHECK ${CPPCHECK})
        list(
            APPEND CMAKE_CXX_CPPCHECK 
                "--enable=all"
                "--inconclusive"
                "-I ./include"
                "--force" 
                "--std=c++17"
        )
    endif()
endfunction()