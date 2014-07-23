add_library(graphics STATIC component.cpp graphics.cpp opengl.cpp triangle.cpp)
target_link_libraries(graphics ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})
