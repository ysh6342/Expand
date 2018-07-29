#pragma once

#include <type_traits>

#include "string.hpp"

namespace Expand
{
namespace meta
{
	//===================================================
	// to_string
	template <char N>
	struct to_wchar
	{
		static constexpr wchar_t value{ N };
	};
	template <wchar_t N>
	struct to_char
	{
		static constexpr char value{ N };
	};

	template <class T>
	struct to_wstring;
	template <template<char...> class T, char... Args>
	struct to_wstring<T<Args...>>
	{
		using type = wstring<to_wchar<Args>::value...>;
	};
	template <template<wchar_t...> class T, wchar_t... Args>
	struct to_wstring<T<Args...>>
	{
		using type = wstring<Args...>;
	};

	template <class T>
	struct to_string;
	template <template<wchar_t...> class T, wchar_t... Args>
	struct to_string<T<Args...>>
	{
		using type = string<to_char<Args>::value...>;
	};
	template <template<char...> class T, char... Args>
	struct to_string<T<Args...>>
	{
		using type = string<Args...>;
	};

#ifdef UNICODE
	template <class T>
	using to_tstring_t = typename to_wstring<T>::type;
#else
	template <class T>
	using to_tstring_t = typename to_string<T>::type;
#endif

	//===================================================
};
};