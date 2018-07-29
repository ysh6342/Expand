#pragma once

namespace Expand
{
//========================================================
namespace meta
{
	//====================================================
	// c_str
	template <class T>
	struct c_str;
	template <template <char...> class T, char... Args>
	struct c_str<T<Args...>>
	{
		static constexpr char value[]{ Args..., '\0' };
	};

	template <class T>
	struct c_wstr;
	template <template <wchar_t...> class T, wchar_t... Args>
	struct c_wstr <T<Args...>>
	{
		static constexpr wchar_t value[]{ Args..., L'\0' };
	};

#ifdef UNICODE
	template <class T>
	using c_tstr = c_wstr<T>;
#else
	template <class T>
	using c_tstr = c_str<T>;
#endif
	//====================================================


	//===================================================
	// string
	template <char... Args>
	struct string
	{
		using type = string<Args...>;
	};
	template <wchar_t... Args>
	struct wstring
	{
		using type = wstring<Args...>;
	};
#ifdef UNICODE
	template <wchar_t... Args>
	using tstring = wstring<Args...>;
#else
	template <char... Args>
	using tstring = string<Args...>;
#endif
	//====================================================
};
};