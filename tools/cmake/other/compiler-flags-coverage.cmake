include(find-tool)

##
# Appends "--coverage" flag to C / C++ compiler flags
##
function(append_coverage_compiler_flags)
    set(_FLAGS "--coverage")
    # append flags to C / C++ compiler flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_FLAGS}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_FLAGS}" PARENT_SCOPE)
    message(STATUS "Appending code coverage compiler flags: ${_FLAGS}")
endfunction()

##
# Checks if toolchain fullfills requirments for code coverage.
# Currently this CMakeFile only support gcc & gcov.
##
function(check_if_toolchain_supports_coverage)
    ## Is compiler type supported for coverage ?
    if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(_IS_GCC TRUE)
    else()
        set(_IS_GCC FALSE)
    endif()

    if(NOT ${_IS_GCC})
        message(FATAL_ERROR "Coverage is only supported for gcc/g++ compiler ! Aborting...")
    endif()

    ## Are compiler version and gcov version matching ?
    # Get gcov version
    find_tool(TMP gcovr)
    find_tool(GCOV gcov)
    if(GCOV)
        execute_process(COMMAND ${GCOV} --version OUTPUT_VARIABLE GCOV_VERSION
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()

    # Get compiler version
    get_filename_component(COMPILER_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)
    find_program(GCC_EXECUTABLE NAMES gcc gcc-${CMAKE_CXX_COMPILER_VERSION} HINTS ${COMPILER_PATH})

    if(GCC_EXECUTABLE)
        execute_process(COMMAND ${GCC_EXECUTABLE} --version OUTPUT_VARIABLE COMPILER_VERSION
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()

    # Extract only the first line of the version output
    string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" GCOVR_VERSION_SHORT ${GCOV_VERSION})
    string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" COMPILER_VERSION_SHORT ${COMPILER_VERSION})

    # Compare versions
    if(GCOVR_VERSION_SHORT AND COMPILER_VERSION_SHORT AND NOT GCOVR_VERSION_SHORT STREQUAL
                                                          COMPILER_VERSION_SHORT)
        message(
            WARNING
                "Coverage generation might fail because version of gcovr (${GCOVR_VERSION_SHORT})
                 does not match version of gcc (${COMPILER_VERSION_SHORT}).")
    endif()

endfunction()
