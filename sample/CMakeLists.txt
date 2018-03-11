cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(DW_COMPUTE_INCLUDE ${PROJECT_SOURCE_DIR}/include)

include_directories("${DW_COMPUTE_INCLUDE}")

add_executable(sample main.cpp)

target_link_libraries(sample dwCompute)