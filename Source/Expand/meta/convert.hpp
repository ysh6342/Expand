#pragma once

#include "string.hpp"

namespace Expand
{
namespace meta
{
	template <class T>
	struct convert;

	template <template <char...> class T, char... Args>
	struct convert<T<Args...>>
	{
		using type = string<Args...>;
	};

	template <class T>
	using convert_t = typename convert<T>::type;
}
}