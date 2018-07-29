#pragma once

#include "../Includes.hpp"
#include "../../Config/INILoader.h"
#include "ClientBase.h"

namespace Expand
{
	namespace Net
	{
		namespace TCP
		{
			template <auto Value, class T, size_t RecvBufferSize = 1024, size_t MaxPacketSize = 1024>
			class TClient
				: public ClientBase
			{
				using SendBuffer = TBuffer<MaxPacketSize>;
				using RecvBuffer = TBuffer<RecvBufferSize>;
			public:
				using ClientBase::ClientBase;

			public:
				bool Start(Expand::Config::INILoader& loader)
				{
					loader.SetSection(EnumConverter::ToStr(Value));

					auto ip = loader.GetValueStr(_T("IP"));
					if (ip.compare(_T("*")) == 0)
					{
						Error::OnError(_T("[%s] load failed config - IP"), __FUNCTIONT__);
						return false;
					}
					auto port = loader.GetValue<short>(_T("PORT"));
					if (port == 0)
					{
						Error::OnError(_T("[%s] load failed config - PORT"), __FUNCTIONT__);
						return false;
					}

					boost::asio::ip::tcp::resolver resolver(get_io_context());
					auto endpoint = resolver.resolve(StrConvert::ConvertT2Multi(ip), std::to_string(port));

					short maxSock = loader.GetValue<short>(_T("MAX_SOCK"));
					if (maxSock == 0)
					{
						Error::OnError(_T("[%s] load failed config - MAX_SOCK"), __FUNCTIONT__);
						return false;
					}

					for (auto i = 0; i < maxSock; ++i)
					{
						Connect(endpoint);
					}

					return true;
				}

				bool Connect(const boost::asio::ip::tcp::resolver::results_type& endpoints)
				{
					auto socket(std::make_shared<boost::asio::ip::tcp::socket>( get_io_context()));

					socket->async_connect(*endpoints, [this, weak = weak_from_this(), socket](boost::system::error_code ec)
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

						auto pSession = std::make_shared<T>(std::move(*socket));
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