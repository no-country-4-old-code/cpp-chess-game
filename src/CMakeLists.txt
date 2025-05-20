add_subdirectory(demo)
add_subdirectory(engine)

# link libraries to demo app
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ChaosLib)

