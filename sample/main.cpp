//#include <dwCompute.h>
#include <iostream>
//
//
//class TDevice;
//
//class TPlatform
//{
//public:
//	std::vector<TDevice> get_all();
//};
//
//class TDevice
//{
//public:
//	TDevice(const TPlatform& p, int a);
//};
//
//std::vector<TDevice> TPlatform::get_all()
//{
//	std::vector<TDevice> dev;
//
//	for (int i = 0; i < 10; i++)
//		dev.push_back(TDevice(*this, i));
//
//	return dev;
//}
//
//TDevice::TDevice(const TPlatform& p, int a)
//{
//
//}
//
//int main()
//{
//	TPlatform p;
//	std::vector<TDevice> d = p.get_all();
//
//	std::vector<cmp::CLPlatform> platforms;
//	std::vector<cmp::Device*> devices;
//
//	cmp::CLPlatform::get_all_platforms(platforms);
//
//	for (int i = 0; i < platforms.size(); i++)
//	{
//		std::cout << "Platform " << i << ": " << platforms[i].name() << std::endl;
//		std::cout << "Type: " << platforms[i].type() << std::endl;
//		std::cout << "Vendor: " << platforms[i].vendor() << std::endl;
//		std::cout << "Version: " << platforms[i].version() << std::endl;
//		
//		devices = platforms[i].get_all_devices();
//
//		for (int j = 0; j < devices.size(); j++)
//		{
//			std::cout << "Device: " << devices[j]->name() << std::endl;
//		}
//	}
//
//	std::cin.get();
//	return 0;
//}

#include <opencl/cl_backend.h>

using namespace dw;

int main()
{
	std::vector<cmp::Platform> platforms;
	
	platforms = cmp::Platform::get_all_platforms();

	for (int i = 0; i < platforms.size(); i++)
	{
		std::cout << "Platform " << i << ": " << platforms[i].name() << std::endl;
		std::cout << "Type: " << platforms[i].type() << std::endl;
		std::cout << "Vendor: " << platforms[i].vendor() << std::endl;
		std::cout << "Version: " << platforms[i].version() << std::endl;

		std::vector<cmp::Device> devices;
		devices = platforms[i].get_all_devices();

		for (int j = 0; j < devices.size(); j++)
		{
			std::cout << "Device: " << devices[j].name() << std::endl;
		}
	}

	std::cin.get();
	return 0;
}