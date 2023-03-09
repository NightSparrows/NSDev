#pragma once

#include "Packet.h"

namespace ns {

	class PacketSequence {
	public:
		virtual void addPacket(Packet &packet) = 0;

		///////////////////////////////////////////////////
		/// \brief Get the packet of the sequence
		/// Returning true means successfully read
		/// False means nothings to read
		///////////////////////////////////////////////////
		virtual bool getPacket(Packet &packet) = 0;

		Ref<PacketSequence> Create(size_t size);

	};

}
