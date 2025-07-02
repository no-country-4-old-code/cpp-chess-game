# Chess Game with support of custom boards and pieces (C++)
A little chess game engine as hobby project.
Aiming for a good balance between performance and abstraction.

## Current Status
Runnable, but 
- The "En passant"-move is currently not supported.
- It is currently not possible to end up with DRAW

## TODO
- Add benchmark tests
- Increase overall performance (e.g. by using templates for underlying move functions)
- Improve algorithm - maybe add some earlier pruning.
- Support Multithreading
- Support "En passant"
- Support "DRAW"
- Support of UCI (Universal Chess Interface)

## Folder
- /src/demo      : Runnable demo - feel free to adapt main.cpp
- /src/engine    : The Chess engine itself separted in "ai", "board" and "pieces"
- /test/_helper  : Just helper function for tests
- /test/engine   : Tests for the engine. Look at under subfolder ai/chess-riddle-tests.cpp to check if the engine can solve your checkmate-in-3-moves-riddle.
- /tools         : Cmake-Scripts, Dockerfile for toolchain, etc.

## Architectural decisions so far
I choose a piece-centric approach.
Pros:
- The board can be represented by bitmaps. This makes checks e.g "if piece A can attack piece B" very cheap.
- Iterating over pieces faster then over the (empty) board squares.
- Decoupling is easier (my oppinion). This is needed to support custom board sizes and pieces efficiently.

Cons:
- A list of pieces must be maintained. Given that heap allocation is ~ 200x slower then stack, I still use a static array with "dead" pieces inside.
- Knowledge about the game status is now distributed among multiple pieces. More complex to debug. I tackle this with unittests.

This results in a domain-separated, quasi-layered architecture, where 
- the dependency direction is strict (the "board" knows nothing, "pieces" know the "board", and the "AI" knows both "board" and "pieces") and
- the "pieces" act as an "open layer". That means "AI" can access the "board" directly

(Planned): Consider using templates to leverage compile-time optimization for low-level board movement logic.


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

