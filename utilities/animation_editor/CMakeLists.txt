aux_source_directory(./ ANIMATION_EDITOR_SRC)
add_executable(animation_editor ${ANIMATION_EDITOR_SRC})
target_link_libraries(animation_editor PRIVATE ${LOWLEVEL_NAME} nuklear tinyfiledialogs)
CopyDLL(animation_editor)