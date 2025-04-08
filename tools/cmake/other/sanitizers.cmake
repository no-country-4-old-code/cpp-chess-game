##
# Enables sanitizers on build.
# For more sanitize options see https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
##
function(enable_sanitzers)
    if(${CMAKE_CXX_COMPILER} MATCHES ".*msys64.*|.*msys32.*")
        set(_RUN_IN_MYSYS_ENV True)
    else()
        set(_RUN_IN_MYSYS_ENV False)
    endif()

    if(_RUN_IN_MYSYS_ENV OR CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(FATAL_ERROR "Sanitizers are not directly supported within MSVC enviroment.")

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer")
        set(_FLAGS_LINKER "-fsanitize=address,undefined")

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_FLAGS}" PARENT_SCOPE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_FLAGS}" PARENT_SCOPE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${_FLAGS_LINKER}" PARENT_SCOPE)

    else()
        message(
            FATAL_ERROR
                "Sanitizers are not directly supported for this compiler: ${CMAKE_CXX_COMPILER}")
    endif()
endfunction()

enable_sanitzers()
