set(LIBRARY_NAME PlayerBehaviourHuman)
set(SOURCES behaviour-human.cpp)

# Add sources to lib
add_library(${LIBRARY_NAME} STATIC ${SOURCES})

# Include "public" header files to lib
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${LIBRARY_NAME} PUBLIC IPlayerBehaviour)