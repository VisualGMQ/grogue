cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(
  grogue
  VERSION 0.1.0
  LANGUAGES CXX C
)

set(GROGUE_CORE_NAME core)
set(GROGUE_GAME_NAME grogue)
set(GROGUE_SCRIPT_NAME script)
set(GROGUE_PHYSICAL_NAME physical)

include(cmake/find_3rdparts.cmake)

include(cmake/build_core.cmake)
include(cmake/build_script.cmake)
include(cmake/build_physical.cmake)
include(cmake/build_game.cmake)
include(cmake/build_tool.cmake)
include(cmake/build_test.cmake)

include(cmake/ide_config.cmake)

include(cmake/install.cmake)
