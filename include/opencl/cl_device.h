#pragma once

#include <device.h>

#if defined(__APPLE__)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

namespace dw
{
	namespace cmp
	{
		class CLDevice : public Device
		{
		public:
			CLDevice(cl::Device);
			std::string name() override;

		private:
			std::string m_name;
			cl::Device  m_device;
		};
	}
}