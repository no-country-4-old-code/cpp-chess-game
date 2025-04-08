include(find-tool)

##
# Creates a custom target to run static code analysis with clang-tidy.
##
function(create_target_clang_tidy)
    # Use function to not pollute shared namespace with custom variables
    set(_TOOL_NAME clang-tidy)
    find_tool(_TOOL_EXECUTABLE ${_TOOL_NAME})

    add_custom_target(
        ${_TOOL_NAME}
        COMMAND
            find ${CMAKE_SOURCE_DIR}/src/ -iname '*.c' -o -iname '*.cpp' -o -iname '*.cc' | xargs -I
            {} ${_TOOL_EXECUTABLE} {} -p ${BUILD_FOLDER_PATH}
            --config-file=${TOOLS_FOLDER_PATH}/.clang-tidy
        COMMENT "Runs '${_TOOL_NAME}' on source files (.c, .cpp, .cc)")
endfunction()

create_target_clang_tidy()
