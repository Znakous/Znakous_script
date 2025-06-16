file(GLOB HEADERS CONFIGURE_DEPENDS
    "*.h"
    EXCLUDE "std_func_spec.h"
    EXCLUDE "std_func_all.h"
) 

set(HEADER_CONTENT "// Don't edit this file, it is auto-generated\n#pragma once\n")
foreach(HEADER ${HEADERS})
    get_filename_component(HEADER_NAME ${HEADER} NAME)
    string(APPEND HEADER_CONTENT "#include \"${HEADER_NAME}\"\n")
endforeach()

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/std_func_all.h "${HEADER_CONTENT}")

add_custom_target(std_func_all 
    ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/std_func_all.h
)

