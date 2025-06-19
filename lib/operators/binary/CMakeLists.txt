file(GLOB HEADERS CONFIGURE_DEPENDS
    "**/*.h"
    "*.h"
    EXCLUDE "binary_opers_executor.h"
    EXCLUDE "binary_opers.h"
    EXCLUDE "operator_base.h"
)
message(STATUS ${HEADERS})


set(HEADER_CONTENT "// Don't edit this file, it is auto-generated\n#pragma once\n")
foreach(HEADER ${HEADERS})
    get_filename_component(HEADER_NAME ${HEADER} ABSOLUTE)
    string(APPEND HEADER_CONTENT "#include \"${HEADER_NAME}\"\n")
endforeach()

message(STATUS ${HEADER_CONTENT})

set(CONTENT "// Don't edit this file, it is auto-generated\n#pragma once\n")
string(APPEND CONTENT "#define binary_opers \\\n   ")
set(NAMES_LIST)

foreach(HEADER ${HEADERS})
    file(READ "${HEADER}" FILE_CONTENT)
    
    string(REPLACE "\n" ";" FILE_LINES "${FILE_CONTENT}")
    
    foreach(LINE IN LISTS FILE_LINES)
        string(LENGTH "${LINE}" LINE_LEN)
        if(LINE_LEN GREATER 8)
            string(SUBSTRING "${LINE}" 0 8 LINE_START)
            if(LINE_START STREQUAL "// add: ")
                string(SUBSTRING "${LINE}" 8 -1 NAME_TO_ADD)
                string(STRIP "${NAME_TO_ADD}" NAME_TO_ADD)
                list(APPEND NAMES_LIST "${NAME_TO_ADD}")
            endif()
        endif()
    endforeach()
endforeach()

if(NAMES_LIST)
    string(REPLACE ";" ", " NAMES_STRING "${NAMES_LIST}")
    string(APPEND CONTENT "${NAMES_STRING}")
else()
    set(CONTENT "") 
endif()


file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/binary_opers_all.h "${HEADER_CONTENT}")
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/binary_opers_names.h "${CONTENT}")

add_custom_target(binary_ops 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/binary_opers_all.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../build/binary_opers_all.h
    DEPENDS ${HEADERS}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_BINARY_DIR}/binary_opers_names.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../../build/binary_opers_names.h
    DEPENDS ${HEADERS}
)