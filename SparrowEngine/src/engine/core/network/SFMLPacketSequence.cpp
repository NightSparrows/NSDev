
#include <NS/core/Log.h>
#include "SFMLPacketSequence.h"

namespace ns {



	SFMLPacketSequence::SFMLPacketSequence(size_t size) {
		this->size = size;
		this->packets = new Packet*[size];

		// initialize
		for (size_t i = 0; i < this->size; i++) {
			this->packets[i] = nullptr;
		}
		this->toReadPtr = 0;
		this->toWritePtr = 0;
	}

	SFMLPacketSequence::~SFMLPacketSequence() {
		for (size_t i = 0; i < this->size; i++) {
			if (this->packets[i] != nullptr) {
				delete this->packets[i];
			}
		}
		delete this->packets;
	}

	void SFMLPacketSequence::addPacket(Packet &packet) {
		sf::Lock lock(this->mutex);
		Packet *toPack = new Packet(packet);
		this->packets[this->toWritePtr] = toPack;
		this->toWritePtr++;
		if (this->toWritePtr == this->size) {
			this->toWritePtr = 0;
		}
		// Check if overload
		if (this->toWritePtr == this->toReadPtr) {
			NS_CORE_WARN("Packet sequence will overflow! Is your buffer big enough?");
		}
	}

	bool SFMLPacketSequence::getPacket(Packet &packet) {
		sf::Lock lock(this->mutex);
		Packet *resultPacket = this->packets[this->toReadPtr];
		if (resultPacket == nullptr) {
			return false;
		}

		// 清空陣列指標
		this->packets[this->toReadPtr] = nullptr;
		this->toReadPtr++;

		packet = *resultPacket;
		
		delete resultPacket;
		return true;
	}
}
