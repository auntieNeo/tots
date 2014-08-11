add_library(framework STATIC event.cpp eventQueue.cpp gameLoop.cpp gameState.cpp message.cpp subsystem.cpp subsystemThread.cpp task.cpp taskQueue.cpp threadPool.cpp threadSignal.cpp)
target_link_libraries(framework utility ${SDL2_LIBRARIES})

add_subdirectory("./tests")
