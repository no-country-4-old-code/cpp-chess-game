set(LIBRARY_NAME ChessAI)
set(SOURCES chess-ai.cpp score.cpp)

# Add sources to lib
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

# Include "public" header files to lib
target_include_directories(${LIBRARY_NAME} PUBLIC ".")
target_link_libraries(${LIBRARY_NAME} PUBLIC PieceTypes)
target_link_libraries(${LIBRARY_NAME} PUBLIC PieceActions)