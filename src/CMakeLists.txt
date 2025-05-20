# add main app
add_subdirectory(main)

# add libraries
add_subdirectory(engine)
add_subdirectory(build-info)
add_subdirectory(player)
add_subdirectory(chaos)

# link libraries to main app
target_link_libraries(${EXECUTABLE_NAME} PUBLIC BuildInfo)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ChaosLib)

