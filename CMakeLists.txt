project(bit_example)
cmake_minimum_required(VERSION 3.16)

if(WIN32)
    set(MSCL_DEFAULT_DIR "$ENV{LIB_PATH}")
else()
    set(MSCL_DEFAULT_DIR "/usr/share/c++-mscl/")
endif()

set(MSCL_DIR "${MSCL_DEFAULT_DIR}" CACHE STRING "Override the default location for MSCL")
find_library(MSCL_LIBRARY
    NAMES mscl
    HINTS "${MSCL_DIR}"
    REQUIRED
)

find_package(Threads REQUIRED)

add_executable(microstrain_bit src/microstrain_bit.cpp)
target_include_directories(microstrain_bit PUBLIC
    ${MSCL_DIR}/source
    ${MSCL_DIR}/Boost/include/
)
target_link_libraries(microstrain_bit ${MSCL_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS microstrain_bit)