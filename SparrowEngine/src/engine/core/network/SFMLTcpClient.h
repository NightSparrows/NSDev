#pragma once

#include <thread>

#include <SFML/Network.hpp>
#include <NS/core/network/TcpClient.h>

#include <NS/core/network/PacketSequence.h>

namespace ns {

	class NS_API SFMLTcpClient : public TcpClient {
	public:
		SFMLTcpClient();
		~SFMLTcpClient();

		bool connect(const char *ip, unsigned int port, float timeout) override;

		void disconnect() override;

		void send(Packet& packet) override;

		bool recive(Packet& packet) override;

		bool isConnected() override;

	protected:
		void threadFunction();

	private:
		sf::TcpSocket m_socket;
		sf::Mutex m_mutex;

		std::thread *m_thread;

		bool connected;
		Ref<PacketSequence> sequence;

	};

}
