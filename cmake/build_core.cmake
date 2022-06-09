aux_source_directory(src/core CORE_SRC)

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_library(${GROGUE_CORE_NAME} SHARED ${CORE_SRC} ${CORE_HEADER_FILES})
else()
    add_library(${GROGUE_CORE_NAME} STATIC ${CORE_SRC} ${CORE_HEADER_FILES})
endif()
target_link_libraries(
  ${GROGUE_CORE_NAME}
  PUBLIC SDL2::SDL2 SDL2::SDL2_image SDL2::SDL2_ttf Lua imgui imgui-filedialog
)
target_include_directories(
  ${GROGUE_CORE_NAME}
  PUBLIC include 3rdlib/toml++
)
target_compile_features(${GROGUE_CORE_NAME} PUBLIC cxx_std_17)
target_precompile_headers(${GROGUE_CORE_NAME} PUBLIC include/core/pch.hpp)
target_compile_options(${GROGUE_CORE_NAME} PRIVATE -Wall -Wformat -Wextra -pedantic)
