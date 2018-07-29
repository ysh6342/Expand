#pragma once

namespace Expand
{
namespace meta
{
	//===================================================
	// concat
	template <class T1, class T2> struct concat;

	template<template<char...> class T1, template<char...> class T2, char... U, char... T>
	struct concat<T1<U...>, T2<T...>>
	{
		using type = T1<U..., T...>;
	};

	template<template<wchar_t...> class T1, template<wchar_t...> class T2, wchar_t... U, wchar_t... T>
	struct concat<T1<U...>, T2<T...>>
	{
		using type = T1<U..., T...>;
	};

	template <class T1, class T2> 
	using concat_t = typename concat<T1, T2>::type;
	//===================================================
};
}