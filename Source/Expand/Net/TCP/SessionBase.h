#pragma once

#include "../Includes.hpp"
#include "../Buffer.h"
#include "../Protocol/IPacket.h"

namespace Expand
{
	namespace Net
	{
		class NetBase;
		namespace TCP
		{
			class SessionBase
				: public std::enable_shared_from_this<SessionBase>
			{
			protected:
				NetBase& m_owner;
				boost::asio::ip::tcp::socket m_socket;
				DWORD64 m_LastRecvedTick;
			public:
				explicit SessionBase(NetBase& owner, boost::asio::ip::tcp::socket&& socket);
				virtual ~SessionBase() = 0 {}

				virtual void OnLinked() {}
				virtual void OnDisconnected() {}
				virtual void OnMsg(Expand::Net::Protocol::IPacket&) = 0;

				void PrepareRecv(std::shared_ptr<class BufferBase>& pBuffer);
				void SendBuffer(std::shared_ptr<class BufferBase>& pBuffer);
				void Disconnect();

				void UpdateLastRecvedTick() { m_LastRecvedTick = GetTickCount64(); }
				DWORD64 GetLastRecvedTick() const { return m_LastRecvedTick; }

			protected:
				void OnRecv(std::shared_ptr<class BufferBase>& buffer, std::size_t length);

				virtual void OnRecvFinish() {}
			};
		}
	}
}