set(LIBRARY_NAME Pieces)
set(SOURCES pieces-color.cpp piece-update.cpp piece-type.cpp piece-embraced-squares-mask.cpp aggregator-positions.cpp army.cpp piece-api.cpp pieces/piece-king.cpp pieces/piece-rock.cpp pieces/piece-bishop.cpp)

# Add sources to lib
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

# Include "public" header files to lib
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pieces")
target_link_libraries(${LIBRARY_NAME} PUBLIC Board)