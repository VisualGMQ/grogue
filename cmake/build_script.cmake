aux_source_directory(src/script SCRIPT_SRC)
file(GLOB SCRIPT_HEADER_FILE "include/script/*.h" "include/script/*.hpp")

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_library(${GROGUE_SCRIPT_NAME} SHARED ${SCRIPT_SRC} ${SCRIPT_HEADER_FILE})
else()
    add_library(${GROGUE_SCRIPT_NAME} STATIC ${SCRIPT_SRC} ${SCRIPT_HEADER_FILE})
endif()

target_link_libraries(${GROGUE_SCRIPT_NAME} PUBLIC ${GROGUE_CORE_NAME})
target_precompile_headers(${GROGUE_SCRIPT_NAME} REUSE_FROM ${GROGUE_CORE_NAME})
