cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# if (WIN32)
#     set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# endif()

project(
    grogue
    VERSION 0.1.0
    LANGUAGES CXX C
    HOMEPAGE_URL https://visualgmq.github.io/grogue/
)

# fetch SDL under Windows
# for Appveyor CI/CD
if (WIN32 AND NOT EMSCRIPTEN)
    include(cmake/fetch_sdl.cmake)
    option(FETCH_SDL "fetch SDL2 SDL2_image SDL2_ttf SDL2_mixer from web" FALSE)
    if (FETCH_SDL)
        FetchSDL()
    endif()
endif()

include(cmake/utility.cmake)

# find SDL
if (NOT EMSCRIPTEN)
    include(cmake/FindSDL2_image.cmake)
    include(cmake/FindSDL2_mixer.cmake)
    include(cmake/FindSDL2_ttf.cmake)
    include(cmake/FindSDL2.cmake)
endif()

# compile 3rdlibs
add_subdirectory(3rdlibs/lua)
add_subdirectory(3rdlibs/sol)
add_subdirectory(3rdlibs/expected)

# find python to execute luabind parser
find_program(PYTHON python3)
if (${PYTHON} MATCHES PYTHON-NOTFOUND)
    find_program(PYTHON python)
    if (${PYTHON} MATCHES PYTHON-NOTFOUND)
        message(FATAL_ERROR "you must install python 3.0 and pip 3.0")
    endif()
endif()

message("fount python3")

find_program(PIP pip3)
if (${PIP} MATCHES PIP-NOTFOUND)
    find_program(PIP pip)
    if (${PIP} MATCHES PIP-NOTFOUND)
        message(FATAL_ERROR "you must install python 3.0 and pip 3.0")
    endif()
endif()

message("fount pip3")

# run python parser to generate luabind codes
file(GLOB_RECURSE CPP_INCLUDE_FILES ./include/*.hpp)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/luabind/luabind.hpp 
    COMMAND ${PIP} install -r utilities/luabind_parser/requirements.txt
    COMMAND ${PYTHON} ${CMAKE_SOURCE_DIR}/utilities/luabind_parser/parser.py "${CMAKE_BINARY_DIR}"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "run parser to parse C++ header file, generate lua bind codes"
    DEPENDS ./utilities/luabind_parser/parser.py ${CPP_INCLUDE_FILES}
    VERBATIM
)


# some useful utiilities
include(cmake/copydll.cmake)
include(cmake/cppcheck.cmake)
AttachCppCheck()


# build game

set(GAME_NAME ${PROJECT_NAME})
set(LOWLEVEL_NAME lowlevel)
set(LUABIND_NAME luabind)
set(GAMELIB_NAME gamelib)
set(GROGUE_LOWLEVELLIB_NAME grogue_lowlevel)
set(GROGUELIB_NAME grogue_game)

aux_source_directory(src/core LOWLEVEL_SRC)
aux_source_directory(src/app  LOWLEVEL_SRC)

add_library(${LOWLEVEL_NAME} STATIC ${LOWLEVEL_SRC} ${CPP_INCLUDE_FILES})

target_include_directories(${LOWLEVEL_NAME} PUBLIC include)
target_compile_features(${LOWLEVEL_NAME} PUBLIC cxx_std_17)
if (MSVC)
    target_compile_options(${LOWLEVEL_NAME} PUBLIC /bigobj)
endif()

if (NOT ${is_mingw_backend})
    target_precompile_headers(${LOWLEVEL_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/core/pch.hpp)
endif()

# check platform
if (UNIX)
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_UNIX)
elseif (WIN32)
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_WIN32)
    target_link_libraries(${LOWLEVEL_NAME} PUBLIC DbgHelp.Lib)
elseif (APPLE)
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_APPLE)
elseif (ANDROID)
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_ANDROID)
elseif (EMSCRIPTEN)
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_EMSCRIPTEN)
else()
    target_compile_definitions(${LOWLEVEL_NAME} PUBLIC GROGUE_PLATFORM_UNKNOWN)
endif()

# compile game layer library
aux_source_directory(src/game GAMELIB_SRC)
add_library(${GAMELIB_NAME} STATIC ${GAMELIB_SRC})
target_link_libraries(${GAMELIB_NAME} PUBLIC ${LOWLEVEL_NAME})

# compile lua bind code
aux_source_directory(src/script SCRIPT_SRC)
add_library(${LUABIND_NAME} STATIC ${CMAKE_BINARY_DIR}/luabind/luabind.hpp ${CMAKE_BINARY_DIR}/luabind/luabind.cpp ${SCRIPT_SRC})
target_link_libraries(${LUABIND_NAME} PUBLIC ${LOWLEVEL_NAME} ${GAMELIB_NAME})
target_include_directories(${LUABIND_NAME} PRIVATE ${CMAKE_BINARY_DIR}/luabind)

# gather all library together
add_library(${GROGUE_LOWLEVELLIB_NAME} INTERFACE)
target_link_libraries(${GROGUE_LOWLEVELLIB_NAME} INTERFACE ${LOWLEVEL_NAME} ${LUABIND_NAME})

add_library(${GROGUELIB_NAME} INTERFACE)
target_link_libraries(${GROGUELIB_NAME} INTERFACE ${GROGUE_LOWLEVELLIB_NAME} ${GAMELIB_NAME})

# compile game
add_executable(${GAME_NAME} ./src/main.cpp)
target_link_libraries(${GAME_NAME} PRIVATE ${GROGUELIB_NAME})

if (NOT EMSCRIPTEN)
    target_link_libraries(${LOWLEVEL_NAME} PUBLIC SDL2 SDL2_image SDL2_ttf SDL2_mixer lua sol2 expected)
    CopyDefScript(${GAME_NAME})
    CopyDLL(${GAME_NAME})
else() # emscripten special link
    message("-- build for wasm")
    CopyResForWASM(${GAME_NAME})    # NOTE: em++ work under ${CMAKE_BINARY_DIR}, so we copy resources dir to there for resources path correctly
    set(USE_SDL "-sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -sUSE_SDL_MIXER=2 -sSDL2_IMAGE_FORMATS=\"[\"png\", \"jpg\", \"bmp\"]\"" )
    set(PRELOAD "--preload-file resources --use-preload-plugins")
    set(OTHER_FLAGS "-s WASM=1 -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_SDL}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_SDL}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_SDL} ${PRELOAD} ${OTHER_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
    target_link_libraries(${LOWLEVEL_NAME} PUBLIC lua sol2 expected)
endif()

# auto format
# option(AUTO_FORMAT "should auto-format before build using clang-format?" ON)
# mark_as_advanced(FORCE AUTO_FORMAT)
# if (AUTO_FORMAT)
#     message("enable auto format")
#     add_custom_command(TARGET ${GAME_NAME} PRE_BUILD
#                     COMMAND ${CMAKE_COMMAND} -P cmake/format.cmake
#                     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#                     COMMENT "start format")
# endif()


# doc generate
option(DOC_GENERATE "generate doc" ON)

if (DOC_GENERATE)
    find_program(DOXYGEN_PROGRAM doxygen)
    if (DOXYGEN_PROGRAM)
        message("found doxygen, enable build doc")
        execute_process(
            COMMAND doxygen
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_QUIET)
    endif()
endif()

# utilities
option(ENABLE_UTILITIES "enable build utilities" ON)
if (ENABLE_UTILITIES AND NOT EMSCRIPTEN)
    add_subdirectory(utilities)
endif()

# unittest
option(ENABLE_TEST "enable build unit test" ON)
if (ENABLE_TEST AND NOT EMSCRIPTEN)
    enable_testing()
    add_subdirectory(test)
endif()


# pack game
set(INSTALL_DIR ${CMAKE_BINARY_DIR}/install)
install(PROGRAMS $<TARGET_FILE:${GAME_NAME}> DESTINATION ${INSTALL_DIR})
if (NOT EMSCRIPTEN)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
    install(DIRECTORY resources DESTINATION ${INSTALL_DIR})
    include(InstallRequiredSystemLibraries)
    install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION ${INSTALL_DIR})
    if (WIN32) # it currently only works on Windows
        install(FILES $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE:SDL2::IMAGE> $<TARGET_FILE:SDL2::TTF> $<TARGET_FILE:SDL2::MIXER>
                DESTINATION ${INSTALL_DIR})
    endif()
else()
    install(FILES ${CMAKE_BINARY_DIR}/${GAME_NAME}.data ${CMAKE_BINARY_DIR}/${GAME_NAME}.wasm ${CMAKE_BINARY_DIR}/${GAME_NAME}.js
            DESTINATION ${INSTALL_DIR})
endif()

# set(CPACK_PACKAGE_NAME "${GAME_NAME}")
# set(CPACK_SET_DESTDIR OFF)
# set(CPACK_PACKAGE_VENDOR "VisualGMQ")
# set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Roguelite game")
# set(CPACK_VERBATIM_VARIABLES YES)
# set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/_packages")
# set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
# set(CPACK_PACKAGE_CONTACT "2142587070@qq.com")
# 
# # some options for NSIS(a Windows package tool), you need install [NSIS](https://nsis.sourceforge.io/)
# # set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/resources/install.bmp")
# set(CPACK_CREATE_DESKTOP_LINKS "${GAME_NAME} v${PROJECT_VERSION}")
# set(CPACK_PACKAGE_INSTALL_DIRECTORY "${GAME_NAME}")
# set(CPACK_PACKAGE_EXECUTABLES "${GAME_NAME}" "resourcecs/role.png")
# include(CPack)
