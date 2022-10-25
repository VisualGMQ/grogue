macro(CopyDLL target_name)
    if (WIN32)
        add_custom_command(
            TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_ROOT}/lib/x64/SDL2.dll $<TARGET_FILE_DIR:${target_name}>
            COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_TTF_ROOT}/lib/x64/SDL2_ttf.dll $<TARGET_FILE_DIR:${target_name}>
            COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_MIXER_ROOT}/lib/x64/SDL2_mixer.dll $<TARGET_FILE_DIR:${target_name}>
            COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_IMAGE_ROOT}/lib/x64/SDL2_image.dll $<TARGET_FILE_DIR:${target_name}>)
    endif()
endmacro(CopyDLL)