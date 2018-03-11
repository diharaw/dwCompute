#include <dwCompute.h>
#include <iostream>

using namespace dw;

int main()
{
	std::vector<cmp::Platform*> platforms;
	std::vector<cmp::Device*> devices;

	cmp::Platform::get_all_platforms(platforms);

	for (int i = 0; i < platforms.size(); i++)
	{
		std::cout << "Platform " << i << ": " << platforms[i]->name() << std::endl;
		std::cout << "Type: " << platforms[i]->type() << std::endl;
		std::cout << "Vendor: " << platforms[i]->vendor() << std::endl;
		std::cout << "Version: " << platforms[i]->version() << std::endl;
		
		platforms[i]->get_all_devices(devices);

		for (int j = 0; j < devices.size(); j++)
		{
			std::cout << "Device: " << devices[i]->name() << std::endl;
		}
	}

	std::cin.get();
	return 0;
}