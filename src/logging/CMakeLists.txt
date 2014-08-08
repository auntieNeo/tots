add_library(logging STATIC log.cpp logging.cpp)
target_link_libraries(logging ${SDL2_LIBRARIES})
