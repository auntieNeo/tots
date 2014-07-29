add_library(framework STATIC aggregateQueue.cpp event.cpp eventQueue.cpp gameLoop.cpp gameState.cpp subsystem.cpp subsystemThread.cpp threadPool.cpp)
target_link_libraries(framework ${SDL2_LIBRARIES})
