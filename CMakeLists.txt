cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    grogue
    VERSION 0.1.0
    LANGUAGES CXX C
)

# build game

set(GAME_NAME ${PROJECT_NAME})

add_subdirectory(tinyengine)

aux_source_directory(src/scenes SCENES_SRC)
aux_source_directory(src/scripts SCRIPTS_SRC)
aux_source_directory(src/others OTHERS_SRC)
aux_source_directory(src/components COMPONENTS_SRC)
aux_source_directory(src/system SYSTEM_SRC)
aux_source_directory(src/map MAP_SRC)
aux_source_directory(src/ui UI_SRC)
aux_source_directory(src/collision COLLISION_SRC)

add_executable(${GAME_NAME})
target_sources(${GAME_NAME} PRIVATE
                ${SCRIPTS_SRC}
                ${SCENES_SRC}
                ${OTHERS_SRC}
                ${COMPONENTS_SRC}
                ${SYSTEM_SRC}
                ${MAP_SRC}
                ${UI_SRC}
                ${COLLISION_SRC}
                src/main.cpp
)
target_link_libraries(${GAME_NAME} PRIVATE tinyengine)
target_precompile_headers(${GAME_NAME} REUSE_FROM tinyengine)
target_include_directories(${GAME_NAME} PRIVATE src)
target_compile_features(${GAME_NAME} PRIVATE cxx_std_17)
CopyDLL(${GAME_NAME})

# install game
install(
    TARGETS ${GAME_NAME}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/install
)

install(
    DIRECTORY data
    DESTINATION ${CMAKE_SOURCE_DIR}/install
)

install(
    DIRECTORY resources
    DESTINATION ${CMAKE_SOURCE_DIR}/install
)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    install(
        FILES init.toml
        DESTINATION ${CMAKE_SOURCE_DIR}/install
    )
endif()