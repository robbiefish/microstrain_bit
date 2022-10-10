project(bit_example)
cmake_minimum_required(VERSION 3.16)

option(BUILD_EXAMPLES "" OFF)
add_subdirectory(src/mip_sdk)

add_executable(microstrain_bit src/microstrain_bit.cpp)
target_include_directories(microstrain_bit PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/mip_sdk/src
)
target_link_libraries(microstrain_bit mip)

install(TARGETS microstrain_bit)