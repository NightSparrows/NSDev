
#include <NS/core/audio/MasterAudio.h>
#include <SFML/Audio.hpp>


namespace ns {

	void MasterAudio::Init() {
		
		// initialize
		SetPosition(0, 0, 0);

	}

	void MasterAudio::SetPosition(float x, float y, float z) {
		sf::Listener::setPosition(x, y, z);
	}

}
