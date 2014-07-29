add_library(logging STATIC logging.cpp)
target_link_libraries(logging ${SDL2_LIBRARIES})
