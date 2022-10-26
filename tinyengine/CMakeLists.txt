include(cmake/FindSDL2.cmake)
include(cmake/FindSDL2_mixer.cmake)
include(cmake/FindSDL2_ttf.cmake) 
include(cmake/FindSDL2_image.cmake) 

add_subdirectory(3rdlibs/tomlplusplus)

include(cmake/init_config2inc.cmake)
include(cmake/copydll.cmake)

############# build engine ##############
set(ENGINE_NAME "tinyengine")

InitConfig2Inc()

aux_source_directory(src/core ENGINE_CORE_SRC)
aux_source_directory(src/renderer ENGINE_RENDERER_SRC)
aux_source_directory(src/input ENGINE_INPUT_SRC)
aux_source_directory(src/ecs ENGINE_ECS_SRC)
aux_source_directory(src/ui ENGINE_UI_SRC)
aux_source_directory(src/components ENGINE_COMPONENTS_SRC)
aux_source_directory(src/sound ENGINE_SOUND_SRC)

add_library(${ENGINE_NAME} STATIC)
target_sources(${ENGINE_NAME} PRIVATE
               ${ENGINE_CORE_SRC}
               ${ENGINE_RENDERER_SRC}
               ${ENGINE_INPUT_SRC}
               ${ENGINE_ECS_SRC}
               ${ENGINE_UI_SRC}
               ${ENGINE_COMPONENTS_SRC}
               ${ENGINE_SOUND_SRC}
               src/entry.cpp)
target_link_libraries(${ENGINE_NAME} PUBLIC SDL2 SDL2_ttf SDL2_mixer toml++ SDL2_image)
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_17)
target_include_directories(${ENGINE_NAME} PUBLIC ./)
target_precompile_headers(${ENGINE_NAME} PUBLIC "engine/core/pch.hpp")