set(LIBRARY_NAME IPlayerBehaviour)
set(SOURCES interface-player-behaviour.cpp)

# Add sources to lib
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

# Include "public" header files to lib
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(ai)
add_subdirectory(human)
