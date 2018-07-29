#pragma once

namespace Expand
{
	namespace impl
	{
		template <class T>
		constexpr decltype(auto) to_tuple(T&& var)
		{
			return to_tuple_impl<Expand::is_tuple_type<T>::value>::convert(std::forward<T>(var));
		}

		template <bool b = false>
		struct to_tuple_impl
		{
			template <class T>
			static constexpr decltype(auto) convert(T&& var)
			{
				return std::forward_as_tuple(std::forward<T>(var));
			}
		};

		template <>
		struct to_tuple_impl<true>
		{
			template <class T>
			static constexpr decltype(auto) convert(T&& var)
			{
				return std::forward<T>(var);
			}
		};
	}

	template <class... Args>
	constexpr decltype(auto) to_tuple(Args&&... args)
	{
		return std::tuple_cat(impl::to_tuple(std::forward<Args>(args))...);
	}
}


