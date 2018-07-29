#pragma once

#include "../Includes.hpp"
#include "../Protocol/TProtocolHandler.hpp"
#include "../Protocol/TProtocolDispatcher.hpp"
#include "../Packet.h"
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
				using Dispatcher = Expand::Net::Protocol::TProtocolDispatcher<Handler>;

			protected:
				using SessionBase::SessionBase;

			public:
				virtual void OnMsg(Expand::Net::Protocol::IPacket& packet) override
				{
					auto handler = Expand::TSingleton_Static<Dispatcher>::GetInstance().GetHandler(packet.GetType());
					if (handler)
					{
						if (!handler->OnExecute(packet, static_cast<T&>(*this)))
						{
							Error::OnError(_T("[%s] failed handler execute : %d"), __FUNCTIONT__, packet.GetType());
						}
					}
					else
					{
						Error::OnError(_T("[%s] not found handler : %d"), __FUNCTIONT__, packet.GetType());
					}
				}
			};
		}
	}
}