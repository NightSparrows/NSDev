#pragma once

#include <SFML/Network.hpp>

#include <NS/core/network/PacketSequence.h>

namespace ns {

	class NS_API SFMLPacketSequence : public PacketSequence {
	public:
		SFMLPacketSequence(size_t size);
		~SFMLPacketSequence();

		void addPacket(Packet &packet) override;

		bool getPacket(Packet &packet) override;

	private:
		Packet** packets;

		size_t toReadPtr;
		size_t toWritePtr;
		size_t size;

		sf::Mutex mutex;

	};

}
