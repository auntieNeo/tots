add_executable(framework_test main.cpp)
target_link_libraries(framework_test framework logging ${GMOCK_BOTH_LIBRARIES} ${GTEST_LIBRARIES} ${SDL2_LIBRARY})

add_test(NAME framework COMMAND framework_test)
