#pragma once

#include "../Includes.hpp"
#include "ServerBase.h"
#include "../../Config/INILoader.h"

namespace Expand
{
	namespace Net
	{
		namespace TCP
		{
			template <auto Value, class T, size_t RecvBufferSize = 1024, size_t MaxPacketSize = 1024>
			class TServer
				: public ServerBase
			{	
				using SendBuffer = TBuffer<MaxPacketSize>;
				using RecvBuffer = TBuffer<RecvBufferSize>;
			public:
				using ServerBase::ServerBase;

			public:
				bool Start(Expand::Config::INILoader& loader)
				{
					// config load
					loader.SetSection(EnumConverter::ToStr(Value));

					PORT port = loader.GetValue<PORT>(_T("PORT"));
					if (port == 0)
					{
						Error::OnError(_T("[%s] load failed config - PORT"), __FUNCTIONT__);
						return false;
					}

					m_acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(m_io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
					if (!m_acceptor)
						return false;

					short maxSock = loader.GetValue<short>(_T("MAX_SOCK"));
					if (maxSock == 0)
					{
						Error::OnError(_T("[%s] load failed config - MAX_SOCK"), __FUNCTIONT__);
						return false;
					}

					for (auto i = 0; i < maxSock; ++i)
					{
						Prepare();
					}

					return true;
				}

				bool Prepare()
				{
					m_acceptor->async_accept([this, weak = weak_from_this()](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
					{
						if (ec)
						{
							// log 

							return;
						}

						auto pSelf = weak.lock();
						if (!pSelf)
						{
							// log

							return;
						}
						

						auto pSession = std::make_shared<T>(std::move(socket));
						if (!pSession)
						{
							// log

							return;
						}

						pSession->PrepareRecv(std::shared_ptr<BufferBase>(new RecvBuffer()));

						pSession->OnLink();
					});

					return true;
				}
			};
		}
	}
}