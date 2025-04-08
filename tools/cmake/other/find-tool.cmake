##
# Wrapper for "find_program" which sets a FATAL_ERROR message if program was not found.
##
function(find_tool _OUTPUT_VAR _TOOL_NAME)
    find_program(_TMP NO_CACHE ${_TOOL_NAME})

    if(NOT _TMP)
        message(
            FATAL_ERROR
                "'${_TOOL_NAME}' was not found. Install '${_TOOL_NAME}' or disable related option !"
        )
    endif()

    set(${_OUTPUT_VAR} ${_TMP} PARENT_SCOPE)
endfunction()
