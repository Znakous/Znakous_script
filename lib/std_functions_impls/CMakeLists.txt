file(GLOB HEADERS CONFIGURE_DEPENDS
    "*.h"
    EXCLUDE "std_func_spec.h"
    EXCLUDE "std_func_all.h"
)  # Get all .h files except std_func_spec.h and std_func_all.h

# Generate the header content
set(HEADER_CONTENT "// Don't edit this file, it is auto-generated\n#pragma once\n")
foreach(HEADER ${HEADERS})
    get_filename_component(HEADER_NAME ${HEADER} NAME)
    string(APPEND HEADER_CONTENT "#include <std_functions_impls/${HEADER_NAME}>\n")
endforeach()

# Write the content to the file
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/std_func_all.h "${HEADER_CONTENT}")

# Create a target that depends on the generated header
add_custom_target(std_func_all 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/std_func_all.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../build/std_func_all.h
    DEPENDS ${HEADERS}
)

