#pragma once

#include "MasterAudio.h"

namespace ns {

	
	// �쪩�W�r�Ӫ��A��u
	class NS_API Audio {
	public:

		static void setPosition(float x, float y, float z) {
			MasterAudio::SetPosition(x, y, z);
		}

	};


}
