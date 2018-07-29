#pragma once

#include "Protocol.hpp"

namespace Expand
{
	namespace Net
	{
		class Packet
			: public Protocol::IPacket
		{
			char* m_pBuffer;
			size_t m_size;
			size_t m_type;
		public:
			Packet() 
				: m_pBuffer(nullptr)
				, m_size(0)
				, m_type(0)
			{
			}

			void Set(size_t size, size_t type, char* pBuffer)
			{
				m_size = size;
				m_type = type;
				m_pBuffer = pBuffer;
			}

			virtual char* GetPtr() override { return m_pBuffer; }
			virtual short GetSize() const override { return static_cast<short>(m_size); }
			virtual short GetType() const override { return static_cast<short>(m_type); }
		};
	}
}
