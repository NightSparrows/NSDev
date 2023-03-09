#pragma once

#include "MasterAudio.h"

namespace ns {

	
	// 原版名字太長，改短
	class NS_API Audio {
	public:

		static void setPosition(float x, float y, float z) {
			MasterAudio::SetPosition(x, y, z);
		}

	};


}
