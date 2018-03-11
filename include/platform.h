#pragma once

#include <vector>
#include <string>
#include <stdint.h>

namespace dw
{
	namespace cmp
	{
		class Device;

		class Platform
		{
		public:
			static void get_all_platforms(std::vector<Platform*>& platforms);

			Platform() {}
			virtual ~Platform() {}
			virtual std::string type() = 0;
			virtual std::string name() = 0;
			virtual std::string vendor() = 0;
			virtual std::string version() = 0;
			virtual int			device_count() = 0;
			virtual void		get_all_devices(std::vector<Device*>& devices) = 0;
		};
	}
}