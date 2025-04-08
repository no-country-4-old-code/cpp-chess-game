##
# Creates a custom target to generate code coverage report using gcovr.
##
function(create_target_unittest_coverage)
    set(_OUTPUT_DIR ${PROJECT_REPORT_PATH}/coverage)
    set(_OUTPUT_FILE ${_OUTPUT_DIR}/coverage_report.html)
    set(_EXCLUDE_PATHS --exclude ${TEST_FOLDER_PATH} --exclude ${BUILD_FOLDER_PATH}/CMakeFiles/)

    add_custom_target(
        unittest-coverage
        # remove folder for a clean start (no artfacts of other builds mixing in)
        COMMAND rm -rf ${BUILD_FOLDER_PATH}/src ${BUILD_FOLDER_PATH}/test
        # build unittests
        COMMAND ${CMAKE_COMMAND} --build . --target unittests -j ${nproc}
        # run unittests
        COMMAND ctest --output-on-failure
        # create report
        COMMAND gcovr -r ${BUILD_FOLDER_PATH}/.. --print-summary ${_EXCLUDE_PATHS}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_OUTPUT_DIR}
        COMMAND gcovr -r ${BUILD_FOLDER_PATH}/.. --html --html-details -o ${_OUTPUT_FILE}
        COMMENT "Running tests and generating coverage report with gcovr")
endfunction()

create_target_unittest_coverage()
