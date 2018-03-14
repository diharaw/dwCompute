#pragma once

#if !defined(DW_CUDA_BACKEND)

// Declaration

#if defined(__APPLE__)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include <kernel_macros.h>
#include <vector>
#include <string>

#define BEGIN_DW_NAMESPACE namespace dw {
#define END_DW_NAMESPACE }

#define BEGIN_CMP_NAMESPACE namespace cmp {
#define END_CMP_NAMESPACE }

BEGIN_DW_NAMESPACE
BEGIN_CMP_NAMESPACE

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

	Platform(cl::Platform platform);
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
	cl::Platform m_platform;
};

class Device
{
public:
	Device(cl::Device device);
	Device(const Platform& platform, uint32_t id);
	inline std::string name();

public:
	std::string	 m_name;
	cl::Device	 m_device;
};

class Context
{
public:
	Context(const Device& device);

public:
	cl::Context m_context;
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
	inline void read(const Queue& queue, const size_t& offset, const size_t& size, void* host);

public:
	cl::Buffer m_buffer;
};

class Program
{
public:
	Program(const Context& context, const std::string& source);
	Program(const Context& context, const void* binary, const size_t& size);
	bool build(const Device& device);

public:
	cl::Program m_program;
};

class Kernel
{
public:
	Kernel(const Program& program, const std::string& name);
	inline void set_argument(const uint32_t& index, const Buffer& buffer);

public:
	cl::Kernel m_kernel;
};

class Event
{
public:
	Event(const Context& context);
	~Event();
	inline bool complete();
	inline void wait();

public:
	cl::Event m_event;
};

class Queue
{
public:
	Queue(const Context& context, const Device& device);
	inline void execute_kernel(const Kernel& kernel, const dim3& global, const dim3& local);
	inline void execute_kernel(const Kernel& kernel, const dim3& global, const dim3& local, const Event& event);
	inline void finish();

public:
	cl::CommandQueue m_queue;
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
	std::vector<cl::Platform> cl_platforms;

	cl::Platform::get(&cl_platforms);

	for (auto platform : cl_platforms)
		platforms.push_back(Platform(platform));

	return platforms;
}

Platform::Platform(cl::Platform platform) : m_platform(platform)
{
	m_name = m_platform.getInfo<CL_PLATFORM_NAME>();
	m_vendor = m_platform.getInfo<CL_PLATFORM_VENDOR>();
	m_version = m_platform.getInfo<CL_PLATFORM_VERSION>();

	std::vector<cl::Device> devices;
	m_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	m_device_count = devices.size();
}

Platform::Platform(uint32_t id)
{
	std::vector<cl::Platform> cl_platforms;
	cl::Platform::get(&cl_platforms);

	m_platform = cl_platforms[id];

	m_name = m_platform.getInfo<CL_PLATFORM_NAME>();
	m_vendor = m_platform.getInfo<CL_PLATFORM_VENDOR>();
	m_version = m_platform.getInfo<CL_PLATFORM_VERSION>();

	std::vector<cl::Device> devices;
	m_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	m_device_count = devices.size();

}

Platform::~Platform()
{

}

std::vector<Device> Platform::get_all_devices()
{
	std::vector<Device> devices;
	std::vector<cl::Device> cl_devices;

	m_platform.getDevices(CL_DEVICE_TYPE_ALL, &cl_devices);

	for (auto device : cl_devices)
		devices.push_back(Device(device));

	return devices;
}

std::string Platform::type()
{
	return "OpenCL";
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

Device::Device(cl::Device device) : m_device(device)
{
	m_name = m_device.getInfo<CL_DEVICE_NAME>();
}

Device::Device(const Platform& platform, uint32_t id)
{
	std::vector<cl::Device> cl_devices;
	platform.m_platform.getDevices(CL_DEVICE_TYPE_ALL, &cl_devices);

	m_device = cl_devices[id];
	m_name = m_device.getInfo<CL_DEVICE_NAME>();
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
	m_context = cl::Context(device.m_device);
}

// -------------------------------------------------------------------------------------
// Buffer ------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Buffer::Buffer(const Context& context, const BufferUsage& usage, const size_t& size, void* data)
{
	cl_uint flags;

	if (usage == CMP_BUFFER_USAGE_READ_ONLY)
		flags = CL_MEM_READ_ONLY;
	else if (usage == CMP_BUFFER_USAGE_WRITE_ONLY)
		flags = CL_MEM_WRITE_ONLY;
	else if (usage == CMP_BUFFER_USAGE_READ_WRITE)
		flags = CL_MEM_READ_WRITE;

	if (data)
		flags |= CL_MEM_COPY_HOST_PTR;

	m_buffer = cl::Buffer(context.m_context, flags, size, data);
}

void Buffer::read(const Queue& queue, const size_t& offset, const size_t& size, void* host)
{
	queue.m_queue.enqueueReadBuffer(m_buffer, CL_TRUE, offset, size, host);
}

// -------------------------------------------------------------------------------------
// Program -----------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Program::Program(const Context& context, const std::string& source)
{
	std::string program = kKERNEL_MACROS;
	program = program + "\n\n";
	program = program + source;

	m_program = cl::Program(context.m_context, program);
}

Program::Program(const Context& context, const void* binary, const size_t& size)
{

}

bool Program::build(const Device& device)
{
	cl_int result = m_program.build({ device.m_device });
	return result == CL_SUCCESS;
}

// -------------------------------------------------------------------------------------
// Kernel ------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Kernel::Kernel(const Program& program, const std::string& name)
{
	m_kernel = cl::Kernel(program.m_program, name.c_str());
}

void Kernel::set_argument(const uint32_t& index, const Buffer& buffer)
{
	m_kernel.setArg(index, buffer.m_buffer);
}

// -------------------------------------------------------------------------------------
// Event -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Event::Event(const Context& context)
{
	
}

Event::~Event()
{
	
}

bool Event::complete()
{
	cl_int status = 0;
	m_event.getInfo(CL_EVENT_COMMAND_EXECUTION_STATUS, &status);
	return status == CL_COMPLETE;
}

void Event::wait()
{
	m_event.wait();
}

// -------------------------------------------------------------------------------------
// Queue -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Queue::Queue(const Context& context, const Device& device)
{
	m_queue = cl::CommandQueue(context.m_context, device.m_device);
}

void Queue::execute_kernel(const Kernel& kernel, const dim3& global, const dim3& local)
{
	m_queue.enqueueNDRangeKernel(kernel.m_kernel,
								 cl::NDRange(0),
								 cl::NDRange(global.x, global.y, global.z),
								 cl::NDRange(local.x, local.y, local.z));
}

void Queue::execute_kernel(const Kernel& kernel, const dim3& global, const dim3& local, const Event& event)
{
	m_queue.enqueueNDRangeKernel(kernel.m_kernel,
								 cl::NDRange(0),
								 cl::NDRange(global.x, global.y, global.z),
								 cl::NDRange(local.x, local.y, local.z),
								 NULL,
								 (cl::Event*)&event.m_event);
}

void Queue::finish()
{
	m_queue.finish();
}

END_CMP_NAMESPACE
END_DW_NAMESPACE

#endif