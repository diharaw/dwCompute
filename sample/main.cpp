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

#define DW_CUDA_BACKEND
#include <dwCompute.h>

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
			std::cout << "Device " << j << ": " << devices[j].name() << std::endl;
		}
	}

	const int numElements = 10;
	float cpuArrayX[numElements] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
	float cpuArrayY[numElements] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f };
	float cpuOutput[numElements] = {};

	const char* source_string = R"(_KERNEL_ void parallel_add(_GLOBAL_ float* x, _GLOBAL_ float* y, _GLOBAL_ float* output) 
{
    const int tid = _GLOBAL_ID_X_;
    output[tid] = x[tid] + y[tid];
})";

	cmp::Platform platform = platforms[0];
	cmp::Device device = platform.get_all_devices()[0];
	cmp::Context context = cmp::Context(device);
	cmp::Queue queue = cmp::Queue(context, device);
	cmp::Program program = cmp::Program(context, source_string);
	cmp::Event event = cmp::Event(context);

	if (!program.build(device))
	{
		std::cout << "Failed to build Program!" << std::endl;
		return 1;
	}

	cmp::Kernel kernel = cmp::Kernel(program, "parallel_add");

	cmp::Buffer bufX = cmp::Buffer(context, cmp::CMP_BUFFER_USAGE_READ_ONLY, sizeof(float) * numElements, &cpuArrayX[0]);
	cmp::Buffer bufY = cmp::Buffer(context, cmp::CMP_BUFFER_USAGE_READ_ONLY, sizeof(float) * numElements, &cpuArrayY[0]);
	cmp::Buffer bufOut = cmp::Buffer(context, cmp::CMP_BUFFER_USAGE_WRITE_ONLY, sizeof(float) * numElements, NULL);

	kernel.set_argument(0, bufX);
	kernel.set_argument(1, bufY);
	kernel.set_argument(2, bufOut);

	cmp::dim3 global = cmp::dim3(numElements, 1, 1);
	cmp::dim3 local = cmp::dim3(1);

	queue.execute_kernel(kernel, global, local, event);

	if (!event.complete())
	{
		std::cout << "Kernel not yet complete" << std::endl;
	}

	event.wait();

	if (event.complete())
	{
		std::cout << "Kernel completed!" << std::endl;
	}
	
	bufOut.read(queue, 0, sizeof(float) * numElements, &cpuOutput[0]);

	for (int i = 0; i < numElements; i++)
		std::cout << cpuArrayX[i] << " + " << cpuArrayY[i] << " = " << cpuOutput[i] << std::endl;

	std::cin.get();
	return 0;
}