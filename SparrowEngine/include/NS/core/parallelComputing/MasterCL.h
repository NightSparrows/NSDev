#pragma once

#include "../Base.h"

namespace ns {

	class NS_API MasterCL {
	public:

		enum PlatformType {
			CPU,
			GPU
		};

		static void Init();

		static void CleanUp();

	};


}
