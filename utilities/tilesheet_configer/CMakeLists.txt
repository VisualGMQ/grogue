aux_source_directory(./ TILESHEET_CONFIG_SRC)
add_executable(tilesheet_configer ${TILESHEET_CONFIG_SRC})
target_link_libraries(tilesheet_configer PRIVATE ${GROGUE_LOWLEVELLIB_NAME} nuklear tinyfiledialogs)
CopyDLL(tilesheet_configer)