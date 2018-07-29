#pragma once

#include "../NetBase.h"

namespace Expand
{
	namespace Net
	{
		namespace TCP
		{
			class ServerBase
				: public NetBase
			{
			protected:
				std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
				
				using NetBase::NetBase;	
			};
		}
	}
}