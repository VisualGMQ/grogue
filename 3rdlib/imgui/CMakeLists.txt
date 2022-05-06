aux_source_directory(. IMGUI_SRC)
file(GLOB IMGUI_HEADER_FILES "*.h" "*.hpp")

add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_HEADER_FILES})
target_link_libraries(imgui PUBLIC SDL2::SDL2)
target_compile_features(imgui PUBLIC cxx_std_17)
target_include_directories(imgui PUBLIC .)
