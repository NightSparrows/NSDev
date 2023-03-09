
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.hpp>

#include <NS/core/Log.h>

#include <NS/core/parallelComputing/MasterCL.h>

namespace ns {

	void MasterCL::Init() {
#ifdef NS_DEBUG
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);

		NS_CORE_INFO("This computer has {0} platforms", platforms.size());
		for (auto platform : platforms) {
			std::vector<cl::Device> devices;
			platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

			for (auto device : devices) {
				auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
				auto version = device.getInfo<CL_DEVICE_VERSION>();
				NS_CORE_INFO("Vender:{0} version:{1}", vendor, version);
			}
		}
#endif // NS_DEBUG

	}

	void MasterCL::CleanUp() {

	}

}
