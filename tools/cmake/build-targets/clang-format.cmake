include(find-tool)

##
# Creates a custom target to format code using clang-format
##
function(create_target_clang_format)
    # Use function to not pollute shared namespace with custom variables
    set(_TOOL_NAME clang-format)
    find_tool(_TOOL_EXECUTABLE ${_TOOL_NAME})

    add_custom_target(
        ${_TOOL_NAME}
        COMMAND
            find ${SRC_FOLDER_PATH}/ -iname '*.h' -o -iname '*.hpp' -o -iname '*.c' -o -iname
            '*.cpp' -o -iname '*.cc' | xargs -I {} ${_TOOL_EXECUTABLE} {} -i
            --style='file:${TOOLS_FOLDER_PATH}/.clang-format'
        COMMENT "Runs ${_TOOL_NAME} on source files (*.h, *.hpp, *.c, *.cpp, *.cc)")
endfunction()

create_target_clang_format()
