project(bit_example)
cmake_minimum_required(VERSION 3.16)

add_executable(bit_example bit_example.cpp)
target_include_directories(bit_example PUBLIC
    /usr/share/c++-mscl/source
    /usr/share/c++-mscl/Boost/include/
)
target_link_libraries(bit_example /usr/share/c++-mscl/libmscl.so pthread)