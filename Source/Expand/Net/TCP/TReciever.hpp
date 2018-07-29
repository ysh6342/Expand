#pragma once

#include "../Includes.hpp"
#include "../Protocol/TProtocolHandler.hpp"
#include "../Protocol/TProtocolDispatcher.hpp"
#include "../Packet.h"
#include "../../TSingleton.hpp"
//#include "TServer.hpp"

namespace Expand
{
	namespace Net
	{
		namespace TCP
		{
			//=========================================================================================
			template <class T>
			class TReciever
				: virtual public SessionBase
			{
			public:
				using Handler = Expand::Net::Protocol::TBaseProtocolHandler<T&>;
				using DispatcherMgr = Expand::TSingleton_Static<Expand::Net::Protocol::TProtocolDispatcher<Handler>>;
			protected:
				using SessionBase::SessionBase;

			public:
				virtual void OnMsg(Expand::Net::Protocol::IPacket& packet) override
				{
					auto handler = DispatcherMgr::GetInstance().GetHandler(packet.GetType());
					if (handler)
					{
						if (!handler->OnExecute(packet, static_cast<T&>(*this)))
						{
							Error::OnError(_T("[%s] failed handler execute : %d"), __FUNCTIONT__, packet.GetType());
						}
					}
					else
					{
						if (packet.GetType() == 0)
						{
							UpdateLastRecvedTick();
						}
						else
						{
							Error::OnError(_T("[%s] not found handler : %d"), __FUNCTIONT__, packet.GetType());
						}
					}
				}
			};
		}
	}
}