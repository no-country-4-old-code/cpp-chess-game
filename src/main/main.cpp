#include <iostream>
#include "build-info.h"
#include "chaos.h"

/**
 * @file main.cpp
 * @brief Main entry point for the program, prints the project information.
 *
 * This function outputs the project name, version, and commit hash to the
 * console. It serves as the starting point for executing the program.
 *
 */
int main() {
    namespace info = build::info;
    std::cout << "Start " << info::projectname << " v" << info::version
              << " from #" << info::hash << "\n";
    run_game();
}