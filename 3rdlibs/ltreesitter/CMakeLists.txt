aux_source_directory(./csrc LTREESITTER_SRC)
add_library(ltreesitter SHARED ${LTREESITTER_SRC})
target_include_directories(ltreesitter PRIVATE ./include)
target_compile_features(ltreesitter PRIVATE c_std_99)
target_link_libraries(ltreesitter PRIVATE lua)
target_link_libraries(ltreesitter PRIVATE treesitter)

if (UNIX)
    target_link_libraries(ltreesitter PRIVATE dl)
endif()

add_subdirectory(./vendors/cpp)