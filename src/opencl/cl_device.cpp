#include <opencl/cl_device.h>

namespace dw
{
	namespace cmp
	{
		CLDevice::CLDevice(cl::Device device) : m_device(device)
		{
			m_name = m_device.getInfo<CL_DEVICE_NAME>();
		}

		std::string CLDevice::name()
		{
			return m_name;
		}
	}
}