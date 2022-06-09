aux_source_directory(src/physical PHYSICAL_SRC)
file(GLOB PHYSICAL_HEADER_FILE "include/physical/*.h" "include/physical/*.hpp")

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_library(${GROGUE_PHYSICAL_NAME} SHARED ${PHYSICAL_SRC} ${PHYSICAL_HEADER_FILE})
else()
    add_library(${GROGUE_PHYSICAL_NAME} STATIC ${PHYSICAL_SRC} ${PHYSICAL_HEADER_FILE})
endif()

target_link_libraries(${GROGUE_PHYSICAL_NAME} PUBLIC ${GROGUE_CORE_NAME})
target_precompile_headers(${GROGUE_PHYSICAL_NAME} REUSE_FROM ${GROGUE_CORE_NAME})
