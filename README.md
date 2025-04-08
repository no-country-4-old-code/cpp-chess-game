# Chess Game with support of custom boards and pieces (C++)

A little chess game engine as hobby project.
I try to find a good balance between performance and abstraction.

## Current Status
Not runnable !
Still in progress 

## Goals regarding performance
- Chess time control of 40/15 (40 moves in 15 minutes) #CCRL 
- ELO > 2000 on medium hardware (while holding 40/15)

## Goals regarding abstraction
- SW-Architecture should support adding custom boards or/and pieces without **modifiying** more then 10 lines of code in a maximum of 3 different files.
- SW-Architecture should support adding custom boards or/and pieces without **adding** much boiler-plate code.

Have fun!
**nc4oc**  


---

## Software Requirements

### Option 1: The Docker Way
You only need Docker installed to build and run the containerized environment.

### Option 2: Manual Installation
If you prefer a manual setup, install the following dependencies:
- **CMake 3.28+**
- **A compiler** (GCC, Clang, MSVC) and a **build system** (Make, Ninja). GCC and Make are recommended because "code coverage" currently works only with GCC & gcovr.

To use optional features like Google Test, you need to install several other tools. Check the `Dockerfile` under `tools/Dockerfile` for the installation process.

---

## Working with the Docker Container

### Running the Docker Container
Use either the precompiled Docker image:
```sh
docker run -di --mount type=bind,src=.,dst=/workspace nc4oc/toolchain-cpp:latest
```
or build your own image if you prefer:
```sh
docker build -t some-image-name -f tools/Dockerfile .
docker run -di --mount type=bind,src=.,dst=/workspace some-image-name
```
Run the command from the project's root directory. The container will mount this folder (`src=.`).

### Attaching to the Docker Container
Most IDEs allow attaching to a running Docker container (e.g., Visual Studio Code with the "Dev Containers" plugin).  
To do it via the console, first find the container ID:
```sh
docker ps
```
Then attach to it:
```sh
docker attach <ID-of-your-container>
```

---

## Building the Project

> **Note:** The following steps assume you are using a Linux shell. If you are on Windows/macOS, use equivalent commands.

CMake is executed from within the `/build` directory. If `/build` does not exist, create it first:
```sh
mkdir -p build
cd build
```

### Build and Run the Application
```sh
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target application
cd src/main
./application
```

### Build and Run Google Tests
```sh
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON ..
cmake --build .
ctest
```

### Build and Generate Coverage Report
```sh
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTS=ON -DENABLE_UNITTEST_COVERAGE=ON ..
cmake --build . --target unittest-coverage
cd reports/coverage
ls *.html
```

### Build and Run custom Python Scripts
```sh
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_PYTHON_SCRIPTS=ON ..
```
This creates build-targets for python-scripts which are listed in "tools/cmake/build-targets/python-script.cmake".
Modify that file if you want to add your own scripts.
All scripts get the path to /src as argument passed.

```sh
cmake --build . --target script-detect-dependencies.py
cmake --build . --target script-find-todos.py
```

### Build and Run... Whatever
Take a look in `CMakeLists.txt` to see the available options.
If you want to enable e.g. the option "ENABLE_CLANG_TIDY," just configure it with:
```sh
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_CLANG_TIDY=ON ..
```
This enables the build target "clang-tidy," which can be executed by:
```sh
cmake --build . --target clang-tidy
```

Happy coding! 🚀

