aux_source_directory(./src TREESITTER_VENDOR_CPP )
add_library(treesitter_vendor_cpp SHARED ${TREESITTER_VENDOR_CPP})
target_include_directories(treesitter_vendor_cpp PRIVATE ./src)