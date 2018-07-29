#pragma once

#include "../Includes.hpp"
#include "../Buffer.h"
#include "../Protocol/IPacket.h"

namespace Expand
{
	namespace Net
	{
		namespace TCP
		{
			class SessionBase
				: public std::enable_shared_from_this<SessionBase>
			{
			protected:
				boost::asio::ip::tcp::socket m_socket;
			public:
				SessionBase(boost::asio::ip::tcp::socket&& socket);
				virtual ~SessionBase() = 0 {}

				virtual void OnLink() {}
				virtual void OnDisconnect() {}
				virtual void OnMsg(Expand::Net::Protocol::IPacket&) = 0;

				void PrepareRecv(std::shared_ptr<class BufferBase>& pBuffer);
				void SendBuffer(std::shared_ptr<class BufferBase>& pBuffer);
				void Disconnect();

			protected:
				void OnRecv(std::shared_ptr<class BufferBase>& buffer, std::size_t length);

				virtual void OnRecvFinish() {}
			};
		}
	}
}