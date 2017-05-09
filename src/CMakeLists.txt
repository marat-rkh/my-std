set(SRC 
    shared_ptr.cpp
)
add_library(my-std ${SRC})
target_include_directories (my-std PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})