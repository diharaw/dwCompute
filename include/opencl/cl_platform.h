#pragma once

#if defined(__APPLE__)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include <platform.h>

namespace dw
{
	namespace cmp
	{
		class CLPlatform : public Platform
		{
		public:
			static void get_all_platforms(std::vector<CLPlatform>& platforms);

			CLPlatform(cl::Platform platform);
			~CLPlatform();
			std::string type() override;
			std::string name() override;
			std::string vendor() override;
			std::string version() override;
			int		    device_count() override;
			std::vector<Device*> get_all_devices() override;

		private:
			cl::Platform		 m_platform;
			std::string			 m_name;
			std::string			 m_vendor;
			std::string			 m_version;
			int					 m_device_count;
			std::vector<Device*> m_devices;
		};
	}
}