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

			private:
				boost::asio::ip::tcp::endpoint m_endpoint;
				short m_sendHeartbeatSec = 0;

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
					auto endpoints = resolver.resolve(StrConvert::ConvertT2Multi(ip), std::to_string(port));
					m_endpoint = *endpoints;

					short maxSock = loader.GetValue<short>(_T("MAX_SOCK"));
					if (maxSock == 0)
					{
						Error::OnError(_T("[%s] load failed config - MAX_SOCK"), __FUNCTIONT__);
						return false;
					}

					m_sendHeartbeatSec = loader.GetValue<short>(_T("SEND_HEARTBEAT_SEC"));

					for (auto i = 0; i < maxSock; ++i)
					{
						Connect(endpoints);
					}

					return true;
				}

				bool Connect(const boost::asio::ip::tcp::endpoint& endpoint)
				{
					auto socket(std::make_shared<boost::asio::ip::tcp::socket>( get_io_context()));

					socket->async_connect(endpoint, [this, weak = weak_from_this(), socket](boost::system::error_code ec)
					{
						if (ec)
						{
							Error::OnError(_T("[%s] Error : %s - %d"), __FUNCTIONT__, StrConvert::ConvertMulti2T(ec.message()).c_str(), ec.value());
							return;
						}

						auto pSelf = weak.lock();
						if (!pSelf)
						{
							Error::OnError(_T("[%s] weak self is empty"), __FUNCTIONT__);
							return;
						}

						auto pSession = std::make_shared<T>(*this, std::move(*socket));
						if (!pSession)
						{
							// Assert

							return;
						}					

						pSession->PrepareRecv(std::shared_ptr<BufferBase>(new RecvBuffer()));

						pSession->OnLinked();

						auto pTimer(std::make_shared<boost::asio::deadline_timer>(m_io_context));
						if (!pTimer)
						{
							// Assert

							return;
						}
						if (m_sendHeartbeatSec > 0)
							SendHeartbeatHandler(std::move(pTimer), pSession);
					});

					return true;
				}

				virtual void OnSessionDisconnected() override final
				{
					Connect(m_endpoint);
				}

			private:
				void SendHeartbeatHandler(std::shared_ptr<boost::asio::deadline_timer>&& pTimer, std::weak_ptr<T> weak)
				{
					pTimer->expires_from_now(boost::posix_time::seconds(m_sendHeartbeatSec));
					pTimer->async_wait([this, pTimer = std::move(pTimer), weak = std::move(weak)](boost::system::error_code ec) mutable
					{
						if (ec)
						{
							return;
						}

						auto pSession = weak.lock();
						if (!pSession)
							return;

						// send heartbeat msg
						//pSession->SendMsg();

						SendHeartbeatHandler(std::move(pTimer), std::move(weak));
					});
				}
			};
		}
	}
}