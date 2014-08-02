add_library(framework STATIC event.cpp eventQueue.cpp gameLoop.cpp gameState.cpp subsystem.cpp subsystemThread.cpp task.cpp taskQueue.cpp threadPool.cpp)
target_link_libraries(framework ${SDL2_LIBRARIES})
