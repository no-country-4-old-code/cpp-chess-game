include(find-tool)

##
# Creates a custom target to run static code analysis with cppcheck.
##
function(create_target_cpp_check)
    # Use function to not pollute shared namespace with custom variables
    set(_TOOL_NAME "cppcheck")
    find_tool(_TOOL_EXECUTABLE ${_TOOL_NAME})

    add_custom_target(
        ${_TOOL_NAME}
        COMMAND
            ${_TOOL_EXECUTABLE} --enable=all --language=c++
            --project="${BUILD_FOLDER_PATH}/compile_commands.json" --suppress=missingIncludeSystem
        COMMENT "Runs '${_TOOL_NAME}' on source files (.c, .cpp, .cc)")
endfunction()

create_target_cpp_check()
