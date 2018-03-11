#pragma once

#if !defined(DW_CUDA_BACKEND)

// Declaration

#if defined(__APPLE__)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

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
	inline int device_count();
	inline std::vector<Device> get_all_devices();

public:
	std::string	 m_name;
	std::string	 m_vendor;
	std::string	 m_version;
	int			 m_device_count;
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
	inline void read(const Queue& queue, const bool& blocking, const size_t& size, void* host);
};

class Program
{
public:
	Program(const Context& context, const std::string& source);
	Program(const Context& context, const void* binary, const size_t& size);
};

class Kernel
{
public:
	Kernel(const Program& program, const std::string& name);
	inline void set_argument(const uint32_t& index, const Buffer& buffer);
};

class Queue
{
public:
	Queue(const Device& device, const Context& context);
	inline void execute_kernel(const Kernel& kernel, const dim3& global, const dim3& local);
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

int Platform::device_count()
{
	return 1;
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

}

// -------------------------------------------------------------------------------------
// Buffer ------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Buffer::Buffer(const Context& context, const BufferUsage& usage, const size_t& size, void* data)
{

}

void Buffer::read(const Queue& queue, const bool& blocking, const size_t& size, void* host)
{

}

// -------------------------------------------------------------------------------------
// Program -----------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Program::Program(const Context& context, const std::string& source)
{

}

Program::Program(const Context& context, const void* binary, const size_t& size)
{

}

// -------------------------------------------------------------------------------------
// Kernel ------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Kernel::Kernel(const Program& program, const std::string& name)
{

}

void Kernel::set_argument(const uint32_t& index, const Buffer& buffer)
{

}

// -------------------------------------------------------------------------------------
// Queue -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Queue::Queue(const Device& device, const Context& context)
{

}

void Queue::execute_kernel(const Kernel& kernel, const dim3& global, const dim3& local)
{

}

END_CMP_NAMESPACE
END_DW_NAMESPACE

#endif