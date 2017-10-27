project(getimage)
cmake_minimum_required(VERSION 2.8)

file(GLOB ALL_SRC_LIST
    "*.h"
    "*.cpp"
)

add_executable(${PROJECT_NAME} ${ALL_SRC_LIST})
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

