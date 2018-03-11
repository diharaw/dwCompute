cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

option(INCLUDE_CUDA_BACKEND "Compile dwCompute with the CUDA backend" ON)

find_package(OpenCL REQUIRED)

if(INCLUDE_CUDA_BACKEND)
    find_package(CUDA REQUIRED)

    set(CUDA_LIB ${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/cuda.lib)
    set(NVRTC_LIB ${CUDA_TOOLKIT_ROOT_DIR}/lib/x64/nvrtc.lib)
endif()

set(DW_COMPUTE_HEADERS ${PROJECT_SOURCE_DIR}/include/buffer.h
                       ${PROJECT_SOURCE_DIR}/include/device.h
                       ${PROJECT_SOURCE_DIR}/include/kernel.h
                       ${PROJECT_SOURCE_DIR}/include/platform.h
                       ${PROJECT_SOURCE_DIR}/include/program.h
                       ${PROJECT_SOURCE_DIR}/include/queue.h
                       ${PROJECT_SOURCE_DIR}/include/context.h
                       ${PROJECT_SOURCE_DIR}/include/opencl/cl_buffer.h
                       ${PROJECT_SOURCE_DIR}/include/opencl/cl_device.h
                       ${PROJECT_SOURCE_DIR}/include/opencl/cl_kernel.h
                       ${PROJECT_SOURCE_DIR}/include/opencl/cl_platform.h
                       ${PROJECT_SOURCE_DIR}/include/opencl/cl_context.h
                       ${PROJECT_SOURCE_DIR}/include/opencl/cl_program.h)

set(DW_COMPUTE_SOURCE ${PROJECT_SOURCE_DIR}/src/platform.cpp
                      ${PROJECT_SOURCE_DIR}/src/opencl/cl_platform.cpp
                      ${PROJECT_SOURCE_DIR}/src/opencl/cl_device.cpp)

set(DW_COMPUTE_INCLUDE ${PROJECT_SOURCE_DIR}/include)

include_directories("${OpenCL_INCLUDE_DIRS}"
                    "${DW_COMPUTE_INCLUDE}")

message(${CUDA_TOOLKIT_ROOT_DIR})

if(INCLUDE_CUDA_BACKEND)
    cuda_add_library(dwCompute ${DW_COMPUTE_SOURCE} ${DW_COMPUTE_HEADERS})
    target_link_libraries(dwCompute ${CUDA_LIB})
    target_link_libraries(dwCompute ${NVRTC_LIB})
else()
    add_library(dwCompute ${DW_COMPUTE_SOURCE})
endif()
			
target_link_libraries(dwCompute ${OpenCL_LIBRARIES})