#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <windows.h>
#include <memory>
#include <list>
#include <vector>
#include <atomic>
#pragma warning(disable : 4996)
#include <boost/asio.hpp>
#pragma warning(default : 4996)

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#ifndef __FUNCTIONT__
#if UNICODE
#define __FUNCTIONT__	__FUNCTIONW__
#else
#define __FUNCTIONT__	__FUNCTION__
#endif
#endif

#ifndef __FILET__
#if UNICODE
#define __FILET__	__FILEW__
#else
#define __FILET__	__FILE__
#endif
#endif

namespace Expand
{
	namespace Net
	{
		typedef short PORT;

		enum ErrorCode
		{
			ErrorCode_
		};

		struct HEART_BEAT
		{
		};
	}
}