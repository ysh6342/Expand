#pragma once

#include <type_traits>

namespace Expand
{
	template <class TupleType, class FuncType>
	void ForEach(TupleType&& tuple, FuncType&& func)
	{
		ForEach_Tuple<std::tuple_size<std::decay_t<TupleType>>::value>::Func(std::forward<FuncType>(func), std::forward<TupleType>(tuple));
	}

	template <class TupleType, class FuncType, class CheckType>
	auto ForEach(TupleType&& tuple, FuncType&& func, CheckType&& checker)
	{
		return ForEach_Tuple<std::tuple_size<std::decay_t<TupleType>>::value>::Func(std::forward<FuncType>(func), std::forward<TupleType>(tuple), std::forward<CheckType>(checker));
	}

	template <size_t N>
	struct ForEach_Tuple
	{
		template <class FuncType, class TupleType>
		static void Func(FuncType&& func, TupleType&& tuple)
		{
			func(std::get<std::tuple_size<std::decay_t<TupleType>>::value - N>(tuple));

			ForEach_Tuple<N - 1>::Func(std::forward<FuncType>(func), std::forward<TupleType>(tuple));
		}


		template <class FuncType, class TupleType, class CheckType>
		static auto Func(FuncType&& func, TupleType&& tuple, CheckType&& checker)
		{
			auto ret = func(std::get<std::tuple_size<std::decay_t<TupleType>>::value - N>(tuple));
			if (ret == checker)
			{
				return ForEach_Tuple<N - 1>::Func(std::forward<FuncType>(func), std::forward<TupleType>(tuple), std::forward<CheckType>(checker));
			}

			return ret;
		}
	};

	template <>
	struct ForEach_Tuple < 0 >
	{
		template <class FuncType, class TupleType>
		static void Func(FuncType&&, TupleType&&)
		{
		}

		template <class FuncType, class TupleType, class CheckType>
		static auto Func(FuncType&&, TupleType&&, CheckType&& checker)
		{
			return checker;
		}
	};
}
