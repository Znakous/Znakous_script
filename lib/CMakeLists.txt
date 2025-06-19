file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
add_subdirectory(operators)

add_library(itmoscript
    ${SOURCES}
)
add_subdirectory(std_functions)

add_dependencies(itmoscript binary_ops unary_ops)

target_include_directories(itmoscript PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)