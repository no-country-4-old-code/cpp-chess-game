include(find-tool)

##
# Creates a custom target to run static analysis on CMake-files using cmake-lint.
##
function(create_target_cmake_lint)
    # Use function to not pollute shared namespace with custom variables
    set(_TOOL_NAME cmake-lint)
    find_tool(_TOOL_EXECUTABLE ${_TOOL_NAME})

    add_custom_target(
        ${_TOOL_NAME}
        COMMAND
            find ${CMAKE_SOURCE_DIR}/ -iname 'CMakeLists.txt' -o -iname '*.cmake' | grep -v
            './build/' | xargs -I {} ${_TOOL_EXECUTABLE} {} --config
            ${TOOLS_FOLDER_PATH}/.cmake-format.cfg --suppress-decorations
        COMMENT "Runs ${_TOOL_NAME} on CMakeLists.txt and *.cmake files")
endfunction()

create_target_cmake_lint()
