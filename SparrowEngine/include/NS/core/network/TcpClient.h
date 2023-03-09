#pragma once

#include "Packet.h"

namespace ns {

	class NS_API TcpClient {
	public:
		////////////////////////////////////////////////////////////
		/// \brief Connect to the server
		/// \param timeout
		///		Maxinum time to connect, in second, Zero mean no timeout
		/// \return
		///		True mean success otherwise fail.
		////////////////////////////////////////////////////////////
		virtual bool connect(const char *ip, unsigned int port, float timeout = 0) = 0;

		////////////////////////////////////////////
		/// \brief Disconnect
		/// 不管有沒有連接都會變成沒有連線
		////////////////////////////////////////////
		virtual void disconnect() = 0;

		//////////////////////////////////////////////////
		/// \brief Send data by SFML packet.
		/// 因為我懶得寫所以這個函式如果傳太大的封包會Lag
		//////////////////////////////////////////////////
		virtual void send(Packet& packet) = 0;

		////////////////////////////////////////////
		/// \brief Get data from sequence
		/// return false mean nothing to read
		////////////////////////////////////////////
		virtual bool recive(Packet& packet) = 0;

		virtual bool isConnected() = 0;

		static Ref<TcpClient> Create();

	};

}
