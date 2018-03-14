#pragma once

#if defined(DW_CUDA_BACKEND)

// Declaration

#include <cuda.h>
#include <nvrtc.h>

#include <kernel_macros.h>
#include <vector>
#include <string>
#include <iostream>

#define BEGIN_DW_NAMESPACE namespace dw {
#define END_DW_NAMESPACE }

#define BEGIN_CMP_NAMESPACE namespace cmp {
#define END_CMP_NAMESPACE }

BEGIN_DW_NAMESPACE
BEGIN_CMP_NAMESPACE

#define NVRTC_SAFE_CALL(Name, x)                                             \
  do {                                                                       \
    nvrtcResult result = x;                                                  \
    if (result != NVRTC_SUCCESS) {                                           \
      std::cerr << "\nerror: " << Name << " failed with error " <<           \
                                               nvrtcGetErrorString(result);  \
      exit(1);                                                               \
    }                                                                        \
  } while(0)

typedef struct
{
	char const *error_string;
	int  error_id;
} s_CudaErrorStr;

static s_CudaErrorStr sCudaDrvErrorString[] =
{
{ "CUDA_SUCCESS", 0 },
{ "CUDA_ERROR_INVALID_VALUE", 1 },
{ "CUDA_ERROR_OUT_OF_MEMORY", 2 },
{ "CUDA_ERROR_NOT_INITIALIZED", 3 },
{ "CUDA_ERROR_DEINITIALIZED", 4 },
{ "CUDA_ERROR_PROFILER_DISABLED", 5 },
{ "CUDA_ERROR_PROFILER_NOT_INITIALIZED", 6 },
{ "CUDA_ERROR_PROFILER_ALREADY_STARTED", 7 },
{ "CUDA_ERROR_PROFILER_ALREADY_STOPPED", 8 },
{ "CUDA_ERROR_NO_DEVICE (no CUDA-capable devices were detected)", 100 },
{ "CUDA_ERROR_INVALID_DEVICE (device specified is not a valid CUDA device)", 101 },
{ "CUDA_ERROR_INVALID_IMAGE", 200 },
{ "CUDA_ERROR_INVALID_CONTEXT", 201 },
{ "CUDA_ERROR_CONTEXT_ALREADY_CURRENT", 202 },
{ "CUDA_ERROR_MAP_FAILED", 205 },
{ "CUDA_ERROR_UNMAP_FAILED", 206 },
{ "CUDA_ERROR_ARRAY_IS_MAPPED", 207 },
{ "CUDA_ERROR_ALREADY_MAPPED", 208 },
{ "CUDA_ERROR_NO_BINARY_FOR_GPU", 209 },
{ "CUDA_ERROR_ALREADY_ACQUIRED", 210 },
{ "CUDA_ERROR_NOT_MAPPED", 211 },
{ "CUDA_ERROR_NOT_MAPPED_AS_ARRAY", 212 },
{ "CUDA_ERROR_NOT_MAPPED_AS_POINTER", 213 },
{ "CUDA_ERROR_ECC_UNCORRECTABLE", 214 },
{ "CUDA_ERROR_UNSUPPORTED_LIMIT", 215 },
{ "CUDA_ERROR_CONTEXT_ALREADY_IN_USE", 216 },
{ "CUDA_ERROR_PEER_ACCESS_UNSUPPORTED", 217 },
{ "CUDA_ERROR_INVALID_PTX", 218 },
{ "CUDA_ERROR_INVALID_GRAPHICS_CONTEXT", 219 },
{ "CUDA_ERROR_NVLINK_UNCORRECTABLE", 220 },
{ "CUDA_ERROR_JIT_COMPILER_NOT_FOUND", 221 },
{ "CUDA_ERROR_INVALID_SOURCE", 300 },
{ "CUDA_ERROR_FILE_NOT_FOUND", 301 },
{ "CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND", 302 },
{ "CUDA_ERROR_SHARED_OBJECT_INIT_FAILED", 303 },
{ "CUDA_ERROR_OPERATING_SYSTEM", 304 },
{ "CUDA_ERROR_INVALID_HANDLE", 400 },
{ "CUDA_ERROR_NOT_FOUND", 500 },
{ "CUDA_ERROR_NOT_READY", 600 },
{ "CUDA_ERROR_ILLEGAL_ADDRESS", 700 },
{ "CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES", 701 },
{ "CUDA_ERROR_LAUNCH_TIMEOUT", 702 },
{ "CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING", 703 },
{ "CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED", 704 },
{ "CUDA_ERROR_PEER_ACCESS_NOT_ENABLED", 705 },
{ "CUDA_ERROR_PRIMARY_CONTEXT_ACTIVE", 708 },
{ "CUDA_ERROR_CONTEXT_IS_DESTROYED", 709 },
{ "CUDA_ERROR_ASSERT", 710 },
{ "CUDA_ERROR_TOO_MANY_PEERS", 711 },
{ "CUDA_ERROR_HOST_MEMORY_ALREADY_REGISTERED", 712 },
{ "CUDA_ERROR_HOST_MEMORY_NOT_REGISTERED", 713 },
{ "CUDA_ERROR_HARDWARE_STACK_ERROR", 714 },
{ "CUDA_ERROR_ILLEGAL_INSTRUCTION", 715 },
{ "CUDA_ERROR_MISALIGNED_ADDRESS", 716 },
{ "CUDA_ERROR_INVALID_ADDRESS_SPACE", 717 },
{ "CUDA_ERROR_INVALID_PC", 718 },
{ "CUDA_ERROR_LAUNCH_FAILED", 719 },
{ "CUDA_ERROR_COOPERATIVE_LAUNCH_TOO_LARGE", 720 },
{ "CUDA_ERROR_NOT_PERMITTED", 800 },
{ "CUDA_ERROR_NOT_SUPPORTED", 801 },
{ "CUDA_ERROR_UNKNOWN", 999 },
{ NULL, -1 }
};

inline const char *getCudaDrvErrorString(CUresult error_id)
{
	int index = 0;

	while (sCudaDrvErrorString[index].error_id != error_id &&
		sCudaDrvErrorString[index].error_id != -1)
	{
		index++;
	}

	if (sCudaDrvErrorString[index].error_id == error_id)
		return (const char *)sCudaDrvErrorString[index].error_string;
	else
		return (const char *)"CUDA_ERROR not found!";
}

inline void check_error(CUresult error_id, const char* file, int line)
{
	if (error_id != CUDA_SUCCESS)
	{
		std::string error = getCudaDrvErrorString(error_id);
		std::cout << "ERROR::CUDA:: FILE:" << file << ", LINE:" << line << ": " << error << std::endl;
	}
}

#define ENABLE_CUDA_CHECK_ERROR

#if defined(ENABLE_CUDA_CHECK_ERROR)
#	define CUDA_CHECK_ERROR(x) check_error(x, __FILE__, __LINE__)
#else
#	define CUDA_CHECK_ERROR(x) x
#endif

struct dim3
{
	float x, y, z;

	dim3() : x(0), y(0), z(0) {}
	dim3(float _v) : x(_v), y(_v), z(_v) {}
	dim3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

class Device;
class Queue;

class Platform
{
public:
	static inline std::vector<Platform> get_all_platforms();

	Platform();
	Platform(uint32_t id);
	~Platform();
	inline std::string type();
	inline std::string name();
	inline std::string vendor();
	inline std::string version();
	inline size_t device_count();
	inline std::vector<Device> get_all_devices();

public:
	std::string	 m_name;
	std::string	 m_vendor;
	std::string	 m_version;
	size_t	     m_device_count;
};

class Device
{
public:
	Device(const uint32_t& index);
	Device(const Platform& platform, uint32_t id);
	inline std::string name();

public:
	std::string	 m_name;
	int			 m_index;
	CUdevice	 m_device;
};

class Context
{
public:
	Context(const Device& device);

public:
	CUcontext m_context;
};

enum BufferUsage
{
	CMP_BUFFER_USAGE_READ_ONLY = 0,
	CMP_BUFFER_USAGE_WRITE_ONLY = 1,
	CMP_BUFFER_USAGE_READ_WRITE = 2
};

class Buffer
{
public:
	Buffer(const Context& context, const BufferUsage& usage, const size_t& size, void* data);
	~Buffer();
	inline void read(const Queue& queue, const size_t& offset, const size_t& size, void* host);

public:
	CUdeviceptr m_buffer;
};

class Program
{
public:
	Program(const Context& context, const std::string& source);
	Program(const Context& context, const void* binary, const size_t& size);
	bool build(const Device& device);

public:
	CUmodule m_program;
	std::string m_source;
};

class Kernel
{
public:
	Kernel(const Program& program, const std::string& name);
	inline void set_argument(const uint32_t& index, const Buffer& buffer);

public:
	static const uint32_t kMAX_ARGUMENTS = 16;
	void*	   m_arguments[kMAX_ARGUMENTS];
	CUfunction m_kernel;
};

class Event
{
public:
	Event(const Context& context);
	~Event();
	inline bool complete();
	inline void wait();

public:
	CUevent m_event;
};

class Queue
{
public:
	Queue(const Context& context, const Device& device);
	~Queue();
	inline void execute_kernel(const Kernel& kernel, const dim3& global, const dim3& local);
	inline void execute_kernel(const Kernel& kernel, const dim3& global, const dim3& local, const Event& event);
	inline void finish();

public:
	CUstream m_queue;
};

END_CMP_NAMESPACE
END_DW_NAMESPACE

// Definition

BEGIN_DW_NAMESPACE
BEGIN_CMP_NAMESPACE

// -------------------------------------------------------------------------------------
// Platform ----------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

std::vector<Platform> Platform::get_all_platforms()
{
	std::vector<Platform> platforms;
	CUresult err = cuInit(0);

	CUDA_CHECK_ERROR(err);

	if (err == CUDA_SUCCESS)
		platforms.push_back(0);
	
	return platforms;
}

Platform::Platform()
{
	/*m_name = m_platform.getInfo<CL_PLATFORM_NAME>();
	m_vendor = m_platform.getInfo<CL_PLATFORM_VENDOR>();
	m_version = m_platform.getInfo<CL_PLATFORM_VERSION>();*/

	int device_count = 0;
	cuDeviceGetCount(&device_count);
	m_device_count = device_count;
}

Platform::Platform(uint32_t id)
{
	CUDA_CHECK_ERROR(cuInit(0));

	/*m_name = m_platform.getInfo<CL_PLATFORM_NAME>();
	m_vendor = m_platform.getInfo<CL_PLATFORM_VENDOR>();
	m_version = m_platform.getInfo<CL_PLATFORM_VERSION>();*/

	int device_count = 0;
	cuDeviceGetCount(&device_count);
	m_device_count = device_count;
}

Platform::~Platform()
{

}

std::vector<Device> Platform::get_all_devices()
{
	std::vector<Device> devices;

	for (int i = 0; i < m_device_count; i++)
		devices.push_back(Device(i));
		
	return devices;
}

std::string Platform::type()
{
	return "CUDA";
}

std::string Platform::name()
{
	return m_name;
}

std::string Platform::vendor()
{
	return m_vendor;
}

std::string Platform::version()
{
	return m_version;
}

size_t Platform::device_count()
{
	return m_device_count;
}

// -------------------------------------------------------------------------------------
// Device ------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Device::Device(const uint32_t& index)
{
	CUDA_CHECK_ERROR(cuDeviceGet(&m_device, index));
	char name[100];
	CUDA_CHECK_ERROR(cuDeviceGetName(name, 100, m_device));

	m_name = name;
}

Device::Device(const Platform& platform, uint32_t id)
{
	CUDA_CHECK_ERROR(cuDeviceGet(&m_device, id));
	char name[100];
	CUDA_CHECK_ERROR(cuDeviceGetName(name, 100, m_device));
}

std::string Device::name()
{
	return m_name;
}

// -------------------------------------------------------------------------------------
// Context -----------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Context::Context(const Device& device)
{
	CUDA_CHECK_ERROR(cuCtxCreate(&m_context, 0, device.m_device));
}

// -------------------------------------------------------------------------------------
// Buffer ------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Buffer::Buffer(const Context& context, const BufferUsage& usage, const size_t& size, void* data)
{
	CUDA_CHECK_ERROR(cuMemAlloc(&m_buffer, size));

	if (data)
	{
		CUDA_CHECK_ERROR(cuMemcpyHtoD(m_buffer, data, size));
	}
}

Buffer::~Buffer()
{
	CUDA_CHECK_ERROR(cuMemFree(m_buffer));
}

void Buffer::read(const Queue& queue, const size_t& offset, const size_t& size, void* host)
{
	CUDA_CHECK_ERROR(cuMemcpyDtoH(host, m_buffer, size));
}

// -------------------------------------------------------------------------------------
// Program -----------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Program::Program(const Context& context, const std::string& source)
{
	m_source = kKERNEL_MACROS;
	m_source = m_source + "\n\n";
	m_source = m_source + source;

}

Program::Program(const Context& context, const void* binary, const size_t& size)
{

}

bool Program::build(const Device& device)
{
	nvrtcProgram prog;
	int numCompileOptions = 0;

	NVRTC_SAFE_CALL("nvrtcCreateProgram", nvrtcCreateProgram(&prog, m_source.c_str(), NULL, 0, NULL, NULL));

	nvrtcResult res = nvrtcCompileProgram(prog, numCompileOptions, NULL);

	// dump log
	size_t logSize;
	NVRTC_SAFE_CALL("nvrtcGetProgramLogSize", nvrtcGetProgramLogSize(prog, &logSize));
	char *log = (char *)malloc(sizeof(char) * logSize + 1);
	NVRTC_SAFE_CALL("nvrtcGetProgramLog", nvrtcGetProgramLog(prog, log));
	log[logSize] = '\x0';


	if (strlen(log) >= 2)
	{
		std::cerr << "\n compilation log ---\n";
		std::cerr << log;
		std::cerr << "\n end log ---\n";
	}

	free(log);

	NVRTC_SAFE_CALL("nvrtcCompileProgram", res);
	// fetch PTX
	size_t ptxSize;
	NVRTC_SAFE_CALL("nvrtcGetPTXSize", nvrtcGetPTXSize(prog, &ptxSize));
	char *ptx = (char *)malloc(sizeof(char) * ptxSize);
	NVRTC_SAFE_CALL("nvrtcGetPTX", nvrtcGetPTX(prog, ptx));
	NVRTC_SAFE_CALL("nvrtcDestroyProgram", nvrtcDestroyProgram(&prog));

	CUresult result = cuModuleLoadDataEx(&m_program, ptx, 0, 0, 0);
	free(ptx);

	CUDA_CHECK_ERROR(result);

	return result == CUDA_SUCCESS;
}

// -------------------------------------------------------------------------------------
// Kernel ------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Kernel::Kernel(const Program& program, const std::string& name)
{
	CUDA_CHECK_ERROR(cuModuleGetFunction(&m_kernel, program.m_program, name.c_str()));
}

void Kernel::set_argument(const uint32_t& index, const Buffer& buffer)
{
	m_arguments[index] = (void*)&buffer.m_buffer;
}

// -------------------------------------------------------------------------------------
// Event -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Event::Event(const Context& context)
{
	CUDA_CHECK_ERROR(cuEventCreate(&m_event, CU_EVENT_BLOCKING_SYNC));
}

Event::~Event()
{
	CUDA_CHECK_ERROR(cuEventDestroy(m_event));
}

bool Event::complete()
{
	return cuEventQuery(m_event) == CUDA_SUCCESS;
}

void Event::wait()
{
	CUDA_CHECK_ERROR(cuEventSynchronize(m_event));
}

// -------------------------------------------------------------------------------------
// Queue -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Queue::Queue(const Context& context, const Device& device)
{
	CUDA_CHECK_ERROR(cuStreamCreate(&m_queue, CU_STREAM_DEFAULT));
}

Queue::~Queue()
{
	CUDA_CHECK_ERROR(cuStreamDestroy(m_queue));
}

void Queue::execute_kernel(const Kernel& kernel, const dim3& global, const dim3& local)
{
	CUDA_CHECK_ERROR(cuLaunchKernel(kernel.m_kernel, global.x, global.y, global.z, local.x, local.y, local.z, 0, m_queue, (void**)&kernel.m_arguments[0], 0));
}

void Queue::execute_kernel(const Kernel& kernel, const dim3& global, const dim3& local, const Event& event)
{
	CUDA_CHECK_ERROR(cuLaunchKernel(kernel.m_kernel, global.x, global.y, global.z, local.x, local.y, local.z, 0, m_queue, (void**)&kernel.m_arguments[0], 0));
	CUDA_CHECK_ERROR(cuEventRecord(event.m_event, m_queue));
}

void Queue::finish()
{
	CUDA_CHECK_ERROR(cuStreamSynchronize(m_queue));
}

END_CMP_NAMESPACE
END_DW_NAMESPACE

#endif
