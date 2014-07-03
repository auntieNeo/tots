add_executable(tots main.cpp log.cpp opengl.cpp graphics/graphics.cpp)
target_link_libraries(tots ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})
