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
			Platform() {}
			virtual ~Platform() {}
			virtual std::string type() = 0;
			virtual std::string name() = 0;
			virtual std::string vendor() = 0;
			virtual std::string version() = 0;
			virtual int			device_count() = 0;
			virtual std::vector<Device*> get_all_devices() = 0;
		};
	}
}