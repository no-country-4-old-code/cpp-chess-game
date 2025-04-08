include(find-tool)

##
# Creates a custom target to format CMake-files using cmake-format.
##
function(create_target_cmake_format)
    # Use function to not pollute shared namespace with custom variables
    set(_TOOL_NAME cmake-format)
    find_tool(_TOOL_EXECUTABLE ${_TOOL_NAME})

    add_custom_target(
        ${_TOOL_NAME}
        COMMAND
            find ${CMAKE_SOURCE_DIR}/ -iname 'CMakeLists.txt' -o -iname '*.cmake' | grep -v
            './build/' | xargs -I {} ${_TOOL_EXECUTABLE} {} --config
            ${TOOLS_FOLDER_PATH}/.cmake-format.cfg --in-place
        COMMENT "Runs ${_TOOL_NAME} on CMakeLists.txt and *.cmake files")
endfunction()

create_target_cmake_format()
