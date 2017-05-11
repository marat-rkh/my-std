set(TEST_SRC shared_ptr_tests.cpp)
add_executable(shared_ptr.tests ${TEST_SRC})
target_link_libraries(shared_ptr.tests gtest_main my-std)