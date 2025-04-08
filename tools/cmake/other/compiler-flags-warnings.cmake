##
# Sets compiler flags for warnings according to compiler type (MSVC, GCC, CLANG)
##
function(set_compiler_warning_flags)

    # Define warnings for MSVC compiler
    set(_MSVC_WARNINGS
        /W4 # Reasonable baseline warnings
        /permissive- # Standards conformance mode
        /we4289 # Nonstandard extension used
        /w14242 # Conversion may result in loss of data
        /w14254 # Operator conversion may change field bits
        /w14263 # Function does not override base class virtual function
        /w14265 # Class with virtual functions lacks virtual destructor
        /w14287 # Unsigned/negative constant mismatch
        /w14296 # Expression always evaluates to a boolean value
        /w14311 # Pointer truncation may occur
        /w14640 # Thread unsafe static member initialization
        /w14826 # Sign-extended conversion between types
        /w14905 # Wide string literal cast to 'LPSTR'
        /w14906 # String literal cast to 'LPWSTR'
        /w14928 # Multiple implicitly user-defined conversions
        /w44062 # Enumerator in switch statement not handled
        /w44242 # Conversion between types may result in data loss
    )

    # Define warnings for Clang compiler
    set(_CLANG_WARNINGS
        -Wall # All basic warnings
        -Wcast-align # Potential performance problem with casts
        -Wconversion # Type conversions may lose data
        -Wdouble-promotion # Float implicitly promoted to double
        -Weffc++ # Effective C++ violations
        -Wextra # Standard extra warnings
        -Wformat=2 # Security issues with formatting functions
        -Wnon-virtual-dtor # Class with virtual functions lacks virtual destructor
        -Wnull-dereference # Null dereference detected
        -Wold-style-cast # C-style cast warnings
        -Woverloaded-virtual # Overload of virtual function
        -Wpedantic # Non-standard code warnings
        -Wshadow # Variable declaration shadows previous context
        -Wsign-conversion # Sign conversion issues
        -Wunused # Unused code warnings
    )

    # Define warnings for GCC compiler, extending Clang warnings
    set(_GCC_WARNINGS
        ${_CLANG_WARNINGS} -Wduplicated-branches # Duplicate code in if/else branches
        -Wduplicated-cond # Duplicate conditions in if/else chains
        -Wlogical-op # Logical operations where bitwise were intended
    )

    # Apply compiler-specific warnings based on the compiler ID
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(_FLAGS "${_MSVC_WARNINGS}")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(_FLAGS "${_CLANG_WARNINGS}")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(_FLAGS "${_GCC_WARNINGS}")
    endif()

    # Convert list to space-separated string
    string(REPLACE ";" " " _FLAGS_STR "${_FLAGS}")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_FLAGS_STR}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_FLAGS_STR}" PARENT_SCOPE)
    message(STATUS "Append compiler warning flags for ${CMAKE_BUILD_TYPE}: ${_FLAGS}")

endfunction()
