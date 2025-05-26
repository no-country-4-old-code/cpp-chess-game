set(LIBRARY_NAME PieceTypes)
set(SOURCES army.cpp pieces/king.cpp pieces/rock.cpp pieces/bishop.cpp pieces/queen.cpp pieces/piece.cpp)

# Add sources to lib
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

# Include "public" header files to lib
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/utils")
target_link_libraries(${LIBRARY_NAME} PUBLIC Board)