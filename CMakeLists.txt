cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    Cataclysm-OnlyOne
    VERSION 0.1.0
    LANGUAGES CXX C
)

# build game

set(GAME_NAME ${PROJECT_NAME})

add_subdirectory(3rdlibs/tinyengine)

aux_source_directory(src/scenes SCENES_SRC)
aux_source_directory(src/scripts SCRIPTS_SRC)
aux_source_directory(src/others OTHERS_SRC)
aux_source_directory(src/components COMPONENTS_SRC)

add_executable(${GAME_NAME})
target_sources(${GAME_NAME} PRIVATE
                ${SCRIPTS_SRC}
                ${SCENES_SRC}
                ${OTHERS_SRC}
                ${COMPONENTS_SRC}
                src/main.cpp
)
target_link_libraries(${GAME_NAME} PRIVATE tinyengine)
target_precompile_headers(${GAME_NAME} REUSE_FROM tinyengine)
target_include_directories(${GAME_NAME} PRIVATE src)
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