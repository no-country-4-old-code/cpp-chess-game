set(LIBRARY_NAME PieceTypes)
set(SOURCES pieces-color.cpp piece-type.cpp army.cpp piece-king.cpp piece-rock.cpp piece-bishop.cpp)

# Add sources to lib
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

# Include "public" header files to lib
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${LIBRARY_NAME} PUBLIC Board)