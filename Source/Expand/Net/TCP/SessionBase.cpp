#include "SessionBase.h"
//====================================
#include "../Buffer.h"
#include "../../Error.hpp"
//====================================
using namespace Expand::Net;
using namespace Expand::Net::TCP;

SessionBase::SessionBase(boost::asio::ip::tcp::socket&& socket)
	: m_socket(std::move(socket))
{

}

void SessionBase::PrepareRecv(std::shared_ptr<BufferBase>& pBuffer)
{
	m_socket.async_read_some(pBuffer->to_recvbuf(), [this, self = shared_from_this(), pBuffer](boost::system::error_code ec, std::size_t length) mutable
	{
		if (ec)
		{
			Error::OnError(_T("[%s] Error : %s - %d"), __FUNCTIONT__, StrConvert::ConvertMulti2T(ec.message()).c_str(), ec.value());
		}

		if (length > 0)
		{
			OnRecv(pBuffer, length);
			OnRecvFinish();
			PrepareRecv(pBuffer);
		}
		else
		{
			// disconnect
			OnDisconnect();
		}
	});
}

void SessionBase::SendBuffer(std::shared_ptr<class BufferBase>& pBuffer)
{
	m_socket.async_write_some(pBuffer->to_sendbuf(), [](boost::system::error_code ec, size_t)
	{
		if (ec)
		{
			Error::OnError(_T("[%s] Error : %s - %d"), __FUNCTIONT__, StrConvert::ConvertMulti2T(ec.message()).c_str(), ec.value());
			return;
		}
	});
}

void SessionBase::Disconnect()
{
	m_socket.shutdown(boost::asio::socket_base::shutdown_both);
}

void SessionBase::OnRecv(std::shared_ptr<BufferBase>& buffer, std::size_t length)
{
	buffer->push(length);

	Packet packet;
	while (buffer->try_parse_header(packet))
	{
		OnMsg(packet);
	}
	buffer->Ready();
}