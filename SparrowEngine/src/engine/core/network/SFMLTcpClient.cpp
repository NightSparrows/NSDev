
#include <NS/core/Log.h>
#include <SFML/System/Time.hpp>
#include "SFMLTcpClient.h"

namespace ns {

	SFMLTcpClient::SFMLTcpClient() {
		this->connected = false;
		this->m_thread = nullptr;
	}

	SFMLTcpClient::~SFMLTcpClient() {
		this->disconnect();

		if (this->m_thread) {
			// Wait for the thread end.
			if (this->m_thread->joinable()) {
				this->m_thread->join();
			}
			delete this->m_thread;
		}
	}

	bool SFMLTcpClient::connect(const char *ip, unsigned int port, float timeout) {
		if (this->m_socket.connect(ip, port, sf::seconds(timeout)) == sf::Socket::Done) {
			this->connected = true;

			this->m_socket.setBlocking(true);

			this->m_thread = new std::thread(&SFMLTcpClient::threadFunction, this);

			return true;
		}

		return false;
	}

	void SFMLTcpClient::disconnect() {
		this->m_socket.disconnect();
		this->connected = false;
	}

	void SFMLTcpClient::send(Packet &packet) {
		sf::Lock lock(this->m_mutex);
		sf::Packet toSend;
		toSend.append(packet.getData(), packet.getDataSize());
		this->m_socket.send(toSend);
	}

	bool SFMLTcpClient::recive(ns::Packet &packet) {
		return this->sequence->getPacket(packet);
	}

	void SFMLTcpClient::threadFunction() {
		sf::Packet tmpData;
		ns::Packet toAdd;

		while (this->connected) {
			sf::Packet receiveData;
			sf::Socket::Status stat = this->m_socket.receive(receiveData);

			tmpData << receiveData;

			switch (stat)
			{
			case sf::Socket::Done:

				toAdd.append(tmpData.getData(), tmpData.getDataSize());

				this->sequence->addPacket(toAdd);
				toAdd.clear();
				tmpData.clear();
				break;
			case sf::Socket::NotReady:
				break;
			case sf::Socket::Partial:
				break;
			case sf::Socket::Disconnected:
				this->disconnect();
				break;
			case sf::Socket::Error:
				this->disconnect();
				NS_CORE_ERROR("TCP socket error!");
				break;
			default:
				break;
			}
		}

#ifdef NS_DEBUG

		NS_INFO("Client thread closed.");

#endif // NS_DEBUG

	}

	bool SFMLTcpClient::isConnected() {
		return this->connected;
	}
}
