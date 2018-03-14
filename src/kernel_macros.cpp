#include <kernel_macros.h>

const char* kKERNEL_MACROS = R"(
#ifdef __CUDACC__
#	define _LOCAL_ 		   __shared__
#	define _GLOBAL_
#	define _FENCE_		   _threadfence()
#	define _BARRIER_ 	   __syncthreads()
#	define _KERNEL_ 	   extern "C" __global__
#	define _CONST_ 	   	   __constant__
#	define _GLOBAL_ID_X_   (threadIdx.x + blockDim.x * blockIdx.x)
#	define _GLOBAL_ID_Y_   (threadIdx.y + blockDim.y * blockIdx.y)
#	define _GLOBAL_ID_Z_   (threadIdx.z + blockDim.z * blockIdx.z)
#	define _LOCAL_ID_X_    (threadIdx.x)
#	define _LOCAL_ID_Y_    (threadIdx.y)
#	define _LOCAL_ID_Z_    (threadIdx.z)
#	define _GROUP_ID_X_    (blockIdx.x)
#	define _GROUP_ID_Y_    (blockIdx.y)
#	define _GROUP_ID_Z_    (blockIdx.z)
#else
#	define _LOCAL_ 		   __local
#	define _GLOBAL_ 	   __global
#	define _FENCE_		   mem_fence(CLK_LOCAL_MEM_FENCE)
#	define _BARRIER_ 	   barrier(CLK_LOCAL_MEM_FENCE)
#	define _KERNEL_ 	   __kernel
#	define _CONST_ 	   	   const
#	define _GLOBAL_ID_X_   get_global_id(0)
#	define _GLOBAL_ID_Y_   get_global_id(1)
#	define _GLOBAL_ID_Z_   get_global_id(2)
#	define _LOCAL_ID_X_    get_local_id(0)
#	define _LOCAL_ID_Y_    get_local_id(1)
#	define _LOCAL_ID_Z_    get_local_id(2)
#	define _GROUP_ID_X_    get_group_id(0)
#	define _GROUP_ID_Y_    get_group_id(1)
#	define _GROUP_ID_Z_    get_group_id(2)
#endif)";