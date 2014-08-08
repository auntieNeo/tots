add_library(graphics STATIC component.cpp graphics.cpp logging.cpp opengl.cpp triangle.cpp)
target_link_libraries(graphics PUBLIC ${Boost_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES})
