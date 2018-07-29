#pragma once

namespace Expand
{
	namespace Net
	{
		namespace Protocol
		{
			template <class T>
			class TProtocolToType
			{
				const short m_type;

				TProtocolToType(short type)
					: m_type(type)
				{
				}

			public:
				static TProtocolToType& RegistProtocol(short type)
				{
					static TProtocolToType instance(type);

					return instance;
				}

				static short GetProtocolType()
				{
					// 등록이 되어 있지 않은 경우 -1 오류 코드 리턴
					return RegistProtocol(-1).m_type;
				}
			};
		};
	}
};