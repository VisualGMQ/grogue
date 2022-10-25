function(InitConfig2Inc)
    set(INC_FILENAME ${CMAKE_CURRENT_SOURCE_DIR}/init_config.inc)

    file(STRINGS ${CMAKE_SOURCE_DIR}/init.toml INIT_CONFIG_SOURCE NEWLINE_CONSUME)
    file(WRITE  ${INC_FILENAME} "std::string_view InitConfigContent = R\"(\n")
    file(APPEND ${INC_FILENAME} ${INIT_CONFIG_SOURCE})
    file(APPEND ${INC_FILENAME} ")\";\n")
endfunction()