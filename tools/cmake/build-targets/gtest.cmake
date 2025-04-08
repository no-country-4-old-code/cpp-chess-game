##
# Creates a executable which runs the given _TEST_FILE and links _LIBRARY_UNDER_TEST
##
function(add_gtest _TEST_FILE _LIBRARY_UNDER_TEST)
    set(_GTEST_LIBS GTest::gtest GTest::gtest_main)
    set(_GTEST_INCLUDES ${gtest_SOURCE_DIR}/include ${gtest_BINARY_DIR})

    find_package(GTest REQUIRED)

    if(NOT GTest_FOUND)
        message(FATAL_ERROR "GTest was not found. Install GTest or disable tests !")
    else()
        # Extract file name without extension for target name
        get_filename_component(_TEST_NAME ${_TEST_FILE} NAME_WE)
        add_executable(${_TEST_NAME} ${_TEST_FILE})
        message(
            STATUS
                "Created Test-Executable ${_TEST_NAME} for testing library: ${_LIBRARY_UNDER_TEST}")

        # link Google Test
        target_link_libraries(${_TEST_NAME} PRIVATE ${_GTEST_LIBS})
        target_include_directories(${_TEST_NAME} PRIVATE ${_GTEST_INCLUDES})

        # link library under test
        target_link_libraries(${_TEST_NAME} PRIVATE ${_LIBRARY_UNDER_TEST})

        # Register test with CTest
        gtest_discover_tests(${_TEST_NAME} EXTRA_ARGS --gtest_print_time --gtest_color=yes
                                                      --gtest_output=json:${_TEST_NAME}.json)

        # Connect with target unittests
        add_dependencies(unittests ${_TEST_NAME})
    endif()
endfunction()

# this target is used to re-build the unittests with coverage for target unittest-coverage
add_custom_target(unittests COMMENT "Build and run all unittests")
