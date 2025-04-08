include(find-tool)

##
# Creates a custom target to generate code documentation using Doxygen.
##
function(create_target_code_doc)
    # Use function to not pollute shared namespace with custom variables
    set(_TOOL_NAME doxygen)
    find_tool(_TOOL_EXECUTABLE ${_TOOL_NAME})

    # Create Doxyfile from template to fillout CMAKE Variables
    configure_file(${TOOLS_FOLDER_PATH}/Doxyfile.in ${BUILD_FOLDER_PATH}/Doxyfile)

    add_custom_target(
        "code-docs"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_REPORT_PATH}"
        COMMAND ${_TOOL_EXECUTABLE}
        WORKING_DIRECTORY ${BUILD_FOLDER_PATH}/
        COMMENT "Create code documentation for source files")
endfunction()

create_target_code_doc()
