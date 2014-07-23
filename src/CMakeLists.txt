add_executable(tots main.cpp log.cpp)
target_link_libraries(tots framework graphics physics ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})

add_subdirectory("./framework")
add_subdirectory("./graphics")
add_subdirectory("./physics")
