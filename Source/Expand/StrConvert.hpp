#pragma once

#include "TTypes.hpp"
#include "TypeTraitsEx.hpp"
#include "Error.hpp"
#include <codecvt>

namespace Expand
{
	class StrConvert
	{
	public:
		template <class... Args>
		static TString<TCHAR> Format(const TCHAR* pszFormat, Args&&... args)
		{
			TString<TCHAR> str;
			Format(str, pszFormat, ToSafeType(std::forward<Args>(args))...);

			return std::move(str);
		}
	private:
		static void Format(TString<TCHAR>& result, const TCHAR* pszFormat, ...)
		{
			va_list args;
			va_start(args, pszFormat);

			__try
			{
				int len = _vsctprintf(pszFormat, args) + 1;

				result.resize(len);

				int returnCode = _vstprintf_s(const_cast<TCHAR*>(result.data()), len, pszFormat, args);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				//Error::OnError(_T("[%s] "), __FUNCTIONT__);
			}
			va_end(args);
		}
	public:
#pragma warning(disable : 4996)

		static TString<wchar_t> ConvertT2Wide(const TCHAR* psz)
		{
#ifdef UNICODE
			return psz;
#else
			std::wstring_convert<
				std::codecvt_utf8_utf16< std::wstring::value_type >,
				std::wstring::value_type
			> utf16conv;

			return utf16conv.from_bytes(psz);
#endif
		}

		static TString<char> ConvertT2Multi(const TCHAR* psz)
		{
#ifdef UNICODE
			std::wstring_convert<
				std::codecvt_utf8_utf16< std::wstring::value_type >,
				std::wstring::value_type
			> utf16conv;

			return utf16conv.to_bytes(psz);
#else
			return psz;
#endif
		}

		static TString<TCHAR> ConvertWide2T(const wchar_t* psz)
		{
#ifdef UNICODE
			return psz;
#else
			std::wstring_convert<
				std::codecvt_utf8_utf16< std::wstring::value_type >,
				std::wstring::value_type
			> utf16conv;

			return utf16conv.to_bytes(psz);
#endif
		}

		static TString<TCHAR> ConvertMulti2T(const char* psz)
		{
#ifdef UNICODE
			std::wstring_convert<
				std::codecvt_utf8_utf16< std::wstring::value_type >,
				std::wstring::value_type
			> utf16conv;

			return utf16conv.from_bytes(psz);
#else
			return psz;
#endif
		}

#pragma warning(default : 4996)

		static TString<wchar_t> ConvertT2Wide(const TString<TCHAR>& str)
		{
			return ConvertT2Wide(str.c_str());
		}
		static TString<char> ConvertT2Multi(const TString<TCHAR>& str)
		{
			return ConvertT2Multi(str.c_str());
		}

		static TString<TCHAR> ConvertWide2T(const TString<wchar_t>& str)
		{
			return ConvertWide2T(str.c_str());
		}

		static TString<TCHAR> ConvertMulti2T(const TString<char>& str)
		{
			return ConvertMulti2T(str.c_str());
		}

	private:
		template <class T>
		static std::enable_if_t<std::is_arithmetic_v<std::remove_reference_t<T>>, T> ToSafeType(T&& arg)
		{
			return std::forward<T>(arg);
		}

		template <class T>
		static std::enable_if_t<std::is_enum_v<std::remove_reference_t<T>>, T> ToSafeType(T&& arg)
		{
			return std::forward<T>(arg);
		}

		template <class T>
		static std::enable_if_t<is_array_type<T>::value, const TCHAR*> ToSafeType(T&& arg)
		{
			return ToSafeType(std::forward<T>(arg));
		}

		template <class T>
		static std::enable_if_t<std::is_same_v<T, const TCHAR*>, T> ToSafeType(T arg)
		{
			return arg;
		}
	};
}