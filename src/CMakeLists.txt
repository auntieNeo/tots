add_executable(tots main.cpp log.cpp)
target_link_libraries(tots framework graphics ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${SDL2_LIBRARY})

add_subdirectory("./framework")
add_subdirectory("./graphics")
