add_executable(framework_test gameLoop.cpp)
target_link_libraries(framework_test framework logging ${GMOCK_BOTH_LIBRARIES} ${GTEST_LIBRARIES} ${SDL2_LIBRARY})

if(WIN32)
  COPY_DLLS_TO_EXE(TARGET framework_test DLLS ${GMOCK_DLLS} ${GTEST_DLLS} ${SDL2_DLLS})
endif(WIN32)

add_test(NAME framework COMMAND framework_test)
