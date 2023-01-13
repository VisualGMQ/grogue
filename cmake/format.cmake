find_program(CLANG_FORMAT NAMES clang-format)
find_program(GIT_PROGRAM NAMES git)

if (CLANG_FORMAT AND GIT_PROGRAM)
    execute_process(COMMAND ${GIT_PROGRAM} "ls-files" "-m"
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    OUTPUT_VARIABLE modified_files)
    string(REPLACE "\n" ";" modified_files ${modified_files})
    foreach(file IN LISTS modified_files)
        if (NOT ${file} STREQUAL "")
            string(STRIP file ${file})
            string(REGEX MATCH ".*.[c|cxx|cpp|cc|h|hpp|hxx]$" file ${file})
            if (NOT ${file} STREQUAL "")
                execute_process(COMMAND ${CLANG_FORMAT} ${file} "-i"
                                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
                message("formatting ${file} ...")
            endif()
        endif()
    endforeach()
elseif (NOT CLANG_FORMAT)
    message(WARNING "you don't has clang-format so we can't auto format codes, we recommand you install it.")
endif()