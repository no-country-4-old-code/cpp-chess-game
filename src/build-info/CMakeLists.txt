# Fetch Git hash of current head and save it in $GIT_HASH
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

# Fetch Git commit date and save it in $GIT_COMMIT_DATE
execute_process(
    COMMAND git show -s --format=%ci HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE)

# Check if Git repository is clean
execute_process(
    COMMAND git status --porcelain
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_STATUS
    OUTPUT_STRIP_TRAILING_WHITESPACE)

if("${GIT_STATUS}" STREQUAL "")
    set(GIT_HAS_UNCOMMITTED_CHANGES FALSE)
else()
    set(GIT_HAS_UNCOMMITTED_CHANGES TRUE)
endif()

set(PATH_GENERATED_BUILD_INFO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/_generated/build-info)

# Fill out templates (*.h.in) and generate real files
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/build-info.h.in"
               "${PATH_GENERATED_BUILD_INFO_FILE}/build-info.h" ESCAPE_QUOTES)

# Define header-only library
add_library(BuildInfo INTERFACE)
target_include_directories(BuildInfo INTERFACE "${PATH_GENERATED_BUILD_INFO_FILE}")

# Custom target to touch build-info.h.in to force its regeneration
add_custom_target(
    trigger-rebuild-of-build-info COMMAND ${CMAKE_COMMAND} -E touch
                                          "${CMAKE_CURRENT_SOURCE_DIR}/build-info.h.in"
    COMMENT "Helper to force re-generation of build-info.h to update git-hash etc.")

# Ensure the UpdateBuildInfoHeader target only triggers the rebuild of BuildInfo
add_dependencies(BuildInfo trigger-rebuild-of-build-info)
