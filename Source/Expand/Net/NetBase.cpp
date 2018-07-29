#include "NetBase.h"
//==============================
#include "Protocol/TProtocolToType.hpp"
using namespace Expand::Net;

NetBase::NetBase(boost::asio::io_context& io_context)
	: m_io_context(io_context)
{
}