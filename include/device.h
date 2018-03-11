#pragma once

#include <string>

namespace dw
{
	namespace cmp
	{
		class Device
		{
		public:
			Device() {}
			virtual std::string name() = 0;


		protected:
			std::string m_name;
		};
	}
}