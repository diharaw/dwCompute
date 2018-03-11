#include <opencl/cl_platform.h>
#include <opencl/cl_device.h>

namespace dw
{
	namespace cmp
	{
		void CLPlatform::get_all_platforms(std::vector<CLPlatform>& platforms)
		{
			std::vector<cl::Platform> cl_platforms;
			cl::Platform::get(&cl_platforms);

			for (int i = 0; i < cl_platforms.size(); i++)
			{
				platforms.push_back(CLPlatform(cl_platforms[i]));
			}
		}

		CLPlatform::CLPlatform(cl::Platform platform) : m_platform(platform)
		{
			m_name = m_platform.getInfo<CL_PLATFORM_NAME>();
			m_vendor = m_platform.getInfo<CL_PLATFORM_VENDOR>();
			m_version = m_platform.getInfo<CL_PLATFORM_VERSION>();

			std::vector<cl::Device> devices;
			m_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

			m_device_count = devices.size();

			for (auto device : devices)
				m_devices.push_back(new CLDevice(device));
		}

		CLPlatform::~CLPlatform()
		{

		}	

		std::string CLPlatform::type()
		{
			return "OpenCL";
		}

		std::string CLPlatform::name()
		{
			return m_name;
		}

		std::string CLPlatform::vendor()
		{
			return m_vendor;
		}

		std::string CLPlatform::version()
		{
			return m_version;
		}

		int CLPlatform::device_count()
		{
			return m_device_count;
		}

		std::vector<Device*> CLPlatform::get_all_devices()
		{
			return m_devices;
		}
	}
}