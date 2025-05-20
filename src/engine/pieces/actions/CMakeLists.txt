set(LIBRARY_NAME PieceActions)
set(SOURCES piece-update.cpp piece-embraced-squares-mask.cpp piece-api.cpp)

# Add sources to lib
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

# Include "public" header files to lib
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${LIBRARY_NAME} PUBLIC Board)
target_link_libraries(${LIBRARY_NAME} PUBLIC PieceTypes)
