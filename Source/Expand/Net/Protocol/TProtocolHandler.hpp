#pragma once

#include <functional>

namespace Expand
{
	namespace Net
	{
		namespace Protocol
		{
			class IPacket;

			template <class... Args>
			class TBaseProtocolHandler
			{
			public:
				TBaseProtocolHandler() {}
				virtual ~TBaseProtocolHandler() {}

				virtual bool OnExecute(IPacket& rPacket, Args... args) = 0;
//				virtual void OnComplete(Args... args) {}
			};

			template <class T, class... Args>
			class TProtocolHandler
				: public TBaseProtocolHandler<Args...>
			{
			public:
				TProtocolHandler() {}
				virtual ~TProtocolHandler() {}

				virtual bool OnExecute(IPacket& rPacket, Args... args) override sealed
				{
					Expand::Net::Protocol::TProtocol<T> protocol;
					if (!protocol.Deserialize(rPacket))
						return false;

					_OnComplete(protocol.Data, args...);
					return true;
				}
// 				virtual void OnComplete(Args... args) override sealed
// 				{
// 					_OnComplete(m_protocol.Data, args...);
// 				}

			protected:
				virtual void _OnComplete(const T& rMsg, Args... args);
			};
		};
	}
};