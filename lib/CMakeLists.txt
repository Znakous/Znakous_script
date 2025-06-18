file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(itmoscript
    ${SOURCES}
)
add_subdirectory(std_functions)
add_dependencies(itmoscript std_func_all)

target_include_directories(itmoscript PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)