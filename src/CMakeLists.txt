add_executable(tots main.cpp log.cpp graphics/component.cpp graphics/graphics.cpp graphics/opengl.cpp graphics/triangle.cpp)
target_link_libraries(tots ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})
