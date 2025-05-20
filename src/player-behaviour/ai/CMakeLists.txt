set(LIBRARY_NAME PlayerBehaviourAI)
set(SOURCES behaviour-ai.cpp)

# Add sources to lib
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

# Include "public" header files to lib
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${LIBRARY_NAME} PUBLIC IPlayerBehaviour)
target_link_libraries(${LIBRARY_NAME} PUBLIC PieceTypes)
target_link_libraries(${LIBRARY_NAME} PUBLIC PieceActions)