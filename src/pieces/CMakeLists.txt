set(LIBRARY_NAME Pieces)
set(SOURCES pieces-color.cpp piece-update.cpp aggregator-positions.cpp army.cpp update-piece-attackable.cpp)

# Add sources to lib
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

# Include "public" header files to lib
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pieces")
target_link_libraries(${LIBRARY_NAME} PUBLIC Board)