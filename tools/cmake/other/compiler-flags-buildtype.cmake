include(compiler-flags-coverage)

##
# Lookup compile flags depending on the build-type for the MSCV compiler
##
function(_get_compile_flags_MSCV _OUTPUT_VAR)

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(_FLAGS "/O2 /DNDEBUG")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(_FLAGS "/Zi /Od")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(_FLAGS "/O2 /Zi")
    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set(_FLAGS "/O1")
    else()
        set(_FLAGS "")
        message(FATAL_ERROR "Unknown build type - no compiler flags set !")
    endif()

    set(${_OUTPUT_VAR} ${_FLAGS} PARENT_SCOPE)

endfunction()

##
# Lookup compile flags depending on the build-type for the GCC or CLANG compiler
##
function(_get_compile_flags_GCC_CLANG _OUTPUT_VAR)

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(_FLAGS "-O3 -DNDEBUG")

    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(_FLAGS "-g -O0")

    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(_FLAGS "-O2 -g")

    elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set(_FLAGS "-Os")
    else()
        set(_FLAGS "")
        message(FATAL_ERROR "Unknown build type - no compiler flags set !")

    endif()

    set(${_OUTPUT_VAR} ${_FLAGS} PARENT_SCOPE)

endfunction()

##
# Set compile flags depending on the build-type and code-coverage-option
##
function(set_compiler_buildtype_flags _IS_COVERAGE_ENABLED)

    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        _get_compile_flags_mscv(_FLAGS)
    else()
        _get_compile_flags_gcc_clang(_FLAGS)
    endif()

    if(_IS_COVERAGE_ENABLED)
        check_if_toolchain_supports_coverage()
        append_coverage_compiler_flags()
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_FLAGS}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_FLAGS}" PARENT_SCOPE)
    message(STATUS "Append compiler buildtype flags for ${CMAKE_BUILD_TYPE}: ${_FLAGS}")

endfunction()
