#pragma once

namespace Expand
{
	namespace Net
	{
		namespace Protocol
		{
			template <class T>
			class TProtocolRegistor
			{
			public:
				template <class Enum>
				static void Regist(Enum registNum)
				{
					static T sInstance((short)registNum);
				}
			protected:
				TProtocolRegistor() = default;

				short MakeProtocolType(short registNum, short protocolNum)
				{
					return (registNum << 8) | ((char)protocolNum);
				}
			};
		};
	}
};