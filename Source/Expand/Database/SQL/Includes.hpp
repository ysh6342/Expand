#pragma once

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <string>
#include <memory>
#include <functional>

#pragma comment(lib, "odbccp32.lib")
#pragma comment(lib, "odbc32.lib")

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

#ifndef TSTRING
#define TSTRING
namespace std
{
#ifdef UNICODE
	typedef wstring tstring;
#else
	typedef string tstring;
#endif
}
#endif

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			class IBase
			{
			public:
				virtual ~IBase() = 0 {}

				virtual SQLHANDLE GetHandle() const = 0;
				virtual SQLUSMALLINT GetType() const = 0;
			};
		}
	}
}