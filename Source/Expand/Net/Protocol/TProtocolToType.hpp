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
					// ����� �Ǿ� ���� ���� ��� -1 ���� �ڵ� ����
					return RegistProtocol(-1).m_type;
				}
			};
		};
	}
};