#pragma once

#include "Includes.hpp"

namespace Expand
{
	namespace Net
	{
		class NetBase
			: public std::enable_shared_from_this<NetBase>
		{
		protected:
			boost::asio::io_context& m_io_context;

		public:
			NetBase(boost::asio::io_context& io_context);
			virtual ~NetBase() = 0 {}

			virtual bool Init() { return true; }

		public:
			boost::asio::io_context& get_io_context() { return m_io_context; }
		};
	}
}