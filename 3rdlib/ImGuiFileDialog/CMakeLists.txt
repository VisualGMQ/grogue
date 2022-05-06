add_library(imgui-filedialog INTERFACE)
target_sources(
    imgui-filedialog
    INTERFACE
    ImGuiFileDialog.cpp ImGuiFileDialog.h ImGuiFileDialogConfig.h dirent/dirent.h stb/stb_image.h stb/stb_image_resize.h)
target_link_libraries(imgui-filedialog INTERFACE imgui)
target_include_directories(
    imgui-filedialog
    INTERFACE . ./stb ./dirent
)
