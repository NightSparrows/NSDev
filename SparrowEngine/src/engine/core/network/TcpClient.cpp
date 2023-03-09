
#include <NS/core/network/TcpClient.h>

#include "SFMLTcpClient.h"

namespace ns {

	Ref<TcpClient> TcpClient::Create() {
		return CreateRef<SFMLTcpClient>();
	}

}
