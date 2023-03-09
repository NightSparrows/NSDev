
#include "SFMLPacketSequence.h"

namespace ns {

	Ref<PacketSequence> PacketSequence::Create(size_t size) {
		return CreateRef<SFMLPacketSequence>(size);
	}

}
