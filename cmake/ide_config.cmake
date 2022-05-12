# core files
file(GLOB_RECURSE CORE_HEADER_FILES "include/core/*.hpp" "include/core/*.h")

source_group("Header Files" FILES ${CORE_HEADER_FILES})
source_group("Source Files" FILES ${CORE_SRC})

# game files
source_group("Header Files" FILES ${GAME_HEADER_FILES})
source_group("Source Files" FILES ${GAME_SRC})
file(GLOB_RECURSE GAME_HEADER_FILES "include/game/*.hpp" "include/game/*.h")
set_target_properties(${PROJECT_NAME} ${GROGUE_CORE_LIB_NAME} ${GROGUE_GUI_LIB_NAME}
    PROPERTIES 
        FOLDER "Game"
)

# assets files
file(GLOB_RECURSE GAME_ASSETS "${CMAKE_SOURCE_DIR}/assets/*")
add_library(assets INTERFACE ${GAME_ASSETS})

# cmake files 
file(GLOB_RECURSE CMAKE_FILES "cmake/*.cmake")
add_library(cmake INTERFACE ${CMAKE_FILES})

# 3rdlib files
file(GLOB_RECURSE 3RDLIB_FILES "3rdlib/*.hpp" "3rdlib/*.h++")
add_library(3rdlib INTERFACE ${3RDLIB_FILES})

# other files 
file(GLOB GAME_OTHER_ASSETS "*.md")
add_library(others INTERFACE ${GAME_OTHER_ASSETS})
