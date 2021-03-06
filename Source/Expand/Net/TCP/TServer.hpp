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
				static_assert(std::is_enum_v<decltype(Value)>);

				using SendBuffer = TBuffer<MaxPacketSize>;
				using RecvBuffer = TBuffer<RecvBufferSize>;
			public:
				using ServerBase::ServerBase;

			private:
				short m_checkHeartbeatSec = 0;

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

					auto maxSock = loader.GetValue<short>(_T("MAX_SOCK"));
					if (maxSock == 0)
					{
						Error::OnError(_T("[%s] load failed config - MAX_SOCK"), __FUNCTIONT__);
						return false;
					}

					m_checkHeartbeatSec = loader.GetValue<short>(_T("CHECK_HEARTBEAT_SEC"));
					for (auto i = 0; i < maxSock; ++i)
					{
						Prepare();
					}

					return true;
				}

				virtual void OnSessionDisconnected() override final
				{
					Prepare();
				}

			private:
				bool Prepare()
				{
					m_acceptor->async_accept([this, weak = weak_from_this()](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
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

						auto pSession = std::make_shared<T>(*this, std::move(socket));
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

						if (m_checkHeartbeatSec > 0)
							CheckHeartbeatHandler(std::move(pTimer), pSession);
					});

					return true;
				}

				void CheckHeartbeatHandler(std::shared_ptr<boost::asio::deadline_timer>&& pTimer, std::weak_ptr<T> weak)
				{
					pTimer->expires_from_now(boost::posix_time::seconds(m_checkHeartbeatSec));
					pTimer->async_wait([this, pTimer = std::move(pTimer), weak = std::move(weak)](boost::system::error_code ec) mutable
					{
						if (ec)
						{
							return;
						}
					
						auto pSession = weak.lock();
						if (!pSession)
							return;
					
						if (GetTickCount64() - pSession->GetLastRecvedTick() >= m_checkHeartbeatSec * 1000)
						{
							pSession->Disconnect();
							return;
						}

						CheckHeartbeatHandler(std::move(pTimer), std::move(weak));
					});
				}
			};
		}
	}
}