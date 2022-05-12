aux_source_directory(src/game GAME_SRC)

add_executable(${GROGUE_GAME_NAME} ${GAME_SRC} ${GAME_HEADER_FILES})
target_link_libraries(${GROGUE_GAME_NAME} PUBLIC ${GROGUE_CORE_LIB_NAME} ${GROGUE_GUI_LIB_NAME})
target_precompile_headers(${GROGUE_GAME_NAME} REUSE_FROM ${GROGUE_CORE_LIB_NAME})
