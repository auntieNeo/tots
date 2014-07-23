add_library(framework STATIC aggregateQueue.cpp event.cpp eventQueue.cpp gameLoop.cpp gameState.cpp hoggedThread.cpp subsystem.cpp subsystemThread.cpp threadPool.cpp workerThread.cpp)
target_link_libraries(framework ${SDL2_LIBRARIES})
