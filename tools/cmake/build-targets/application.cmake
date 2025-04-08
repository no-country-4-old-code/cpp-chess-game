include(compiler-flags-buildtype)
include(compiler-flags-warnings)

##
# Creates target to build main application.
##
function(create_target_application _IS_COVERAGE_ENABLED)
    set_compiler_buildtype_flags(${_IS_COVERAGE_ENABLED})
    set_compiler_warning_flags()

    add_subdirectory(src)

    # pass build flags to PARENT_SCOPE
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
endfunction()
