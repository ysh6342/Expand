#pragma once

#include "../../meta.hpp"

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			template <class query_name_type, class params_type, class CALL = CALL_SQL>
			class Builder
			{
			private:
				template <class T>
				struct params
				{
					static constexpr auto value = for_each_param_unpack<T>::count<std::tuple_size<T>::value - 1>();
				};

				template <class T>
				struct for_each_param_unpack
				{
					template <int N>
					static constexpr auto count()
					{
						return for_each_impl::size<std::decay_t<std::tuple_element_t<N, T>>>() + count<N - 1>();
					}
					template <>
					static constexpr auto count<0>()
					{
						return for_each_impl::size<std::decay_t<std::tuple_element_t<0, T>>>();
					}
				};

				struct for_each_impl
				{
					template <class T>
					static constexpr std::enable_if_t<is_param_type<T>::value, int> size()
					{
						return 1;
					}

					template <class T>
					static constexpr std::enable_if_t<is_tuple_type<T>::value, int> size()
					{
						return for_each_param_unpack<T>::count<std::tuple_size<T>::value - 1>();
					}

					template <class T>
					static constexpr std::enable_if_t<has_mem_func_get_params<T>::value, int> size()
					{
						return for_each_param_unpack<decltype(std::declval<T>().get_params())>::count<std::tuple_size<decltype(std::declval<T>().get_params())>::value - 1>();
					}
				};

// 
// 				template <bool b = false>
// 				struct for_each_impl	// is_param_type
// 				{
// 					template <class T>
// 					static constexpr decltype(auto) size()
// 					{
// 						return 1;//T::size();
// 					}
// 				};
// 
// 				template <>
// 				struct for_each_impl<1>	// is_tuple_type
// 				{
// 					template <class T>
// 					static constexpr decltype(auto) size()
// 					{
// 						return for_each_param_unpack<T>::count<std::tuple_size<T>::value - 1>();
// 					}
// 				};

				template <int N>
				struct for_each
				{
					template <char... Args>
					static constexpr decltype(auto) type_func()
					{
						return for_each<N - 1>::type_func<Args..., ',', '?'>();
					}
				};

				template <>
				struct for_each<0>
				{
					template <char... Args>
					static constexpr decltype(auto) type_func()
					{
						return meta::string< Args..., ')', ' ', '}' >{};
					}
				};

				template <>
				struct for_each<-1>
				{
					template <char... Args>
					static constexpr decltype(auto) type_func()
					{
						return meta::string< ' ', '}' >{};
					}
				};

				using type1 = typename meta::convert_t<CALL>;
				using type2 = typename meta::convert_t<query_name_type>;
				using type3 = decltype(for_each<params<std::decay_t<params_type>>::value - 1>::type_func<'(', '?'>());
				using cat_type1 = typename meta::concat_t<type1, type2>;
				using cat_type2 = typename meta::concat_t<cat_type1, type3>;
			public:
				using type = typename cat_type2::type;
			};
		}
	}
}