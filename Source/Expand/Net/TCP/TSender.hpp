#pragma once

#include "../Includes.hpp"
#include "../Buffer.h"
#include "../Protocol.hpp"

namespace Expand
{
	namespace Net
	{
		namespace TCP
		{
			//=========================================================================================
			template <class T>
			class TSender
				: virtual public SessionBase
			{
			protected:
				using SessionBase::SessionBase;

			public:
				std::shared_ptr<BufferBase> MakeSerializedBuffer(Protocol::IProtocol& protocol)
				{
					std::shared_ptr<BufferBase> buffer(new Buffer(protocol.GetSize()));
					Packet packet;
					packet.Set(buffer->get_maxsize(), protocol.GetType(), buffer->get_data());
					if (protocol.Serialize(packet))
					{
						buffer->push(packet.GetSize());

						return std::move(buffer);
					}

					return nullptr;
				}
				
				void SendMsg(Protocol::IProtocol& protocol)
				{
					auto buffer = MakeSerializedBuffer(protocol);
					if (buffer)
					{
						SendBuffer(buffer);
					}
				}
			};

			//=========================================================================================
			template <class T>
			class TSendGather
				: public TSender<T>
			{
				using ListBuffer = std::list<std::shared_ptr<BufferBase>>;
				ListBuffer m_listBuffer;
			protected:
				using TSender<T>::TSender;

			public:
				void SendBufferGather(std::shared_ptr<BufferBase>& buffer)
				{
					m_listBuffer.emplace_back(buffer);
				}

				void SendGatherMsg(Protocol::IProtocol& protocol)
				{
					auto buffer = MakeSerializedBuffer(protocol);
					if (buffer)
					{
						SendBufferGather(buffer);
					}
				}

			private:
				void SendFlush()
				{
					if (m_listBuffer.size() == 0)
						return;

					std::vector<boost::asio::const_buffer> buffers; buffers.reserve(m_listBuffer.size());
					for (const auto& var : m_listBuffer)
					{
						buffers.emplace_back(var->to_sendbuf());
					}
					m_socket.async_write_some(buffers, [listBuffer = std::move(m_listBuffer)](boost::system::error_code ec, std::size_t)
					{
						if (ec)
						{
							Error::OnError(_T("[%s] Error : %s - %d"), __FUNCTIONT__, StrConvert::ConvertMulti2T(ec.message()).c_str(), ec.value());
							return;
						}
					});
				}

				virtual void OnRecvFinish() { SendFlush(); }
			};
		}
	}
}