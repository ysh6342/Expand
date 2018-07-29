#pragma once

#include "Includes.hpp"
#include <list>
#include <vector>
#include <mutex>
#include "Packet.h"

namespace Expand
{
	namespace Net
	{
		class BufferBase
			: public std::enable_shared_from_this<BufferBase>
		{
			std::size_t m_head;
			std::size_t m_tail;
			std::size_t m_max;
		public:
			explicit BufferBase()
				: m_head(0)
				, m_tail(0)
				, m_max(0)
			{

			}

		protected:
			explicit BufferBase(std::size_t BufferSize)
				: m_head(0)
				, m_tail(0)
				, m_max(BufferSize)
			{
			}

		public:
			virtual ~BufferBase() = 0 {}

			bool try_parse_header(Packet& packet)
			{
				Serializer s(get_data() + m_tail, m_head - m_tail);

				short size = 0;
				if (!s.TDeserialize(size))
					return false;

				if (size > m_head - m_tail)
					return false;

				short type = 0;
				if (!s.TDeserialize(type))
					return false;

				packet.Set(size - 4, type, get_data() + m_tail + 4);
				pop(size);

				return true;
			}

			boost::asio::const_buffer to_sendbuf() const
			{
				return boost::asio::buffer(get_data() + m_tail, m_head - m_tail);
			}

			boost::asio::mutable_buffer to_recvbuf()
			{
				return boost::asio::buffer(get_data() + m_head, m_max - m_head);
			}

			void push(std::size_t bytesTransferred) { m_head += bytesTransferred; }
			void pop(std::size_t bytesTransferred) { m_tail += bytesTransferred; }

			void Ready()
			{
				if (m_head == m_tail)
				{
					m_head = m_tail = 0;
				}
				else
				{
					memmove(get_data(), get_data() + m_tail, m_head - m_tail);
					m_head = m_head - m_tail;
					m_tail = 0;
				}
			}

			std::size_t get_maxsize() const { return m_max; }
			virtual char* get_data() = 0;
			virtual const char* get_data() const = 0;
		};

		template <std::size_t BufferSize>
		class TBuffer
			: public BufferBase
		{
			char m_data[BufferSize];
			
		public:
			TBuffer()
				: BufferBase(BufferSize)
				, m_data{ 0, }
			{
			}

			virtual char* get_data() override { return m_data; }
			virtual const char* get_data() const override { return m_data; }
		};

		class Buffer
			: public BufferBase
		{
			std::unique_ptr<char[]> m_data;
			std::size_t m_max;
			std::size_t m_head;
			std::size_t m_tail;
		public:
			Buffer(std::size_t bufferSize)
				: BufferBase(bufferSize)
				, m_data(std::make_unique<char[]>(bufferSize))
			{
			}


			virtual char* get_data() override { return m_data.get(); }
			virtual const char* get_data() const override { return m_data.get(); }
		};
	}
}
