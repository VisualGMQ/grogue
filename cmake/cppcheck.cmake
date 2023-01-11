find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
    list(
        APPEND CMAKE_CXX_CPPCHECK 
            "--enable=all"
            "--inconclusive"
            "-I ./include"
            "--force" 
            "--std=c++17"
    )
endif()