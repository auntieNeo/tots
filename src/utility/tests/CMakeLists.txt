add_definitions(-DBOOST_TEST_DYN_LINK) 
add_executable(utility_test main.cpp)
target_link_libraries(utility_test ${Boost_LIBRARIES})

add_test(NAME utility COMMAND utility_test)
