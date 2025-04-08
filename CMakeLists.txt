cmake_minimum_required(VERSION 3.28.0)
project(
    "Name-of-your-project"
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Add your project description here")

# Global Settings
set(BUILD_FOLDER_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(TOOLS_FOLDER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tools)
set(SRC_FOLDER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_FOLDER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(PROJECT_REPORT_PATH ${BUILD_FOLDER_PATH}/reports)
add_subdirectory(tools)

# Compiler Settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
# cmake-format: off
# cmake-lint: disable=C0301
option(ENABLE_TESTS               "Builds tests with every build"                                           OFF)
option(ENABLE_IPO                 "Enables interprocedural optimization for application"                    OFF)
option(ENABLE_CLANG_FORMAT        "Adds build-target 'clang-format' to format source code"                  OFF)
option(ENABLE_CLANG_TIDY          "Adds build-target 'clang-tidy' to run static code analysis "             OFF)
option(ENABLE_CPP_CHECK           "Adds build-target 'cppcheck' to run static code analysis"                OFF)
option(ENABLE_SANITIZERS          "Builds application with 'sanitizers for run runtime-analysis"            OFF)
option(ENABLE_UNITTEST_COVERAGE   "Adds build-target 'unittest-coverage' to create test coverage reports"   OFF)
option(ENABLE_CMAKE_FORMAT        "Adds build-target 'cmake-format' to format CMake files"                  OFF)
option(ENABLE_PYTHON_SCRIPTS      "Adds build-targets 'script-<name-of-script>' to run python scripts"      OFF)
option(ENABLE_CMAKE_LINT          "Adds build-target 'cmake-lint' to lint CMake files"                      OFF)
option(ENABLE_DOXYGEN             "Adds build-target 'code-doc' to create documentation via doxygen"        OFF)
# cmake-format: on

# Application
include(application)
set(EXECUTABLE_NAME "application")
create_target_application(${ENABLE_UNITTEST_COVERAGE})

# Optional build-targets
if(ENABLE_CLANG_FORMAT)
    include(clang-format)
endif()

if(ENABLE_CLANG_TIDY)
    include(clang-tidy)
endif()

if(ENABLE_CPP_CHECK)
    include(cppcheck)
endif()

if(ENABLE_SANITIZERS)
    include(sanitizers)
endif()

if(ENABLE_UNITTEST_COVERAGE)
    include(unittest-coverage)
endif()

if(ENABLE_CMAKE_FORMAT)
    include(cmake-format)
endif()

if(ENABLE_PYTHON_SCRIPTS)
    include(python-scripts)
endif()

if(ENABLE_CMAKE_LINT)
    include(cmake-lint)
endif()

if(ENABLE_DOXYGEN)
    include(code-docs)
endif()

if(ENABLE_TESTS)
    include(CTest)
    include(gtest)
    enable_testing()
    add_subdirectory(test)
endif()

if(ENABLE_IPO)
    include(interprocedural-optimization)
    target_enable_ipo(${EXECUTABLE_NAME})
endif()
