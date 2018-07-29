#pragma once

#define USE_STL_CONTAINER
#define USE_BOOST_MULTIINDEX_CONTAINER

#ifdef USE_STL_CONTAINER

#include <string>
#include <array>
#include <forward_list>
#include <list>
#include <vector>
#include <queue>
#include <deque>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#endif

#ifdef USE_BOOST_MULTIINDEX_CONTAINER

#include <boost/multi_index_container.hpp>

#endif

#include <type_traits>

namespace Expand
{
#ifdef USE_STL_CONTAINER

	//=============================================================
	template <class T>
	struct is_string_type : std::false_type {};

	template <class T, class... Args>
	struct is_string_type < std::basic_string<T, Args...> > : std::true_type {};

	//=============================================================
	template <class T>
	struct is_array_type : std::false_type {};

	template <class T, size_t N>
	struct is_array_type< std::array<T, N> > : std::true_type {};

	//=============================================================
	template <class  T>
	struct is_element_container : std::false_type {};

#define ADD_IS_ELEMENT_CONTAINER(Container)\
	template <class Value, class... Args>\
	struct is_element_container< Container<Value, Args...> > : std::true_type{}

	//=============================================================
	template <class T>
	struct is_pair_container : std::false_type {};

#define ADD_IS_PAIR_CONTAINER(Container)\
	template <class Key, class Value, class... Args>\
	struct is_pair_container< Container<Key, Value, Args...> > : std::true_type{}

	//=============================================================
	template <class T>
	struct is_shared_ptr : std::false_type {};

	template <template <class> class T, class Element>
	struct is_shared_ptr< T<Element> > : std::is_same < T<Element>, std::shared_ptr<Element> > {};

	//=============================================================
	template <class T>
	struct is_tuple_type : std::false_type {};

	template <template <class...> class T, class... Args>
	struct is_tuple_type<T<Args...>> : std::is_same<T<Args...>, std::tuple<Args...>> {};


	ADD_IS_ELEMENT_CONTAINER(std::forward_list);
	ADD_IS_ELEMENT_CONTAINER(std::list);
	ADD_IS_ELEMENT_CONTAINER(std::vector);
	ADD_IS_ELEMENT_CONTAINER(std::deque);
	ADD_IS_ELEMENT_CONTAINER(std::queue);
	ADD_IS_ELEMENT_CONTAINER(std::set);
	ADD_IS_ELEMENT_CONTAINER(std::multiset);
	ADD_IS_ELEMENT_CONTAINER(std::unordered_set);
	ADD_IS_ELEMENT_CONTAINER(std::unordered_multiset);

	ADD_IS_PAIR_CONTAINER(std::map);
	ADD_IS_PAIR_CONTAINER(std::multimap);
	ADD_IS_PAIR_CONTAINER(std::unordered_map);
	ADD_IS_PAIR_CONTAINER(std::unordered_multimap);

#endif
	//=============================================================

#ifdef USE_BOOST_MULTIINDEX_CONTAINER

 	template <class T>
 	struct is_multi_index_container : false_type {};

	template <class Value, class IndexSpecifierList, class Allocator>
	struct is_multi_index_container<boost::multi_index_container<Value, IndexSpecifierList, Allocator>> : std::true_type {};
//  	template <template <class, class, class> Container, class Value, class IndexSpecifierList, class Allocator>
//  	struct is_multi_index_container< Container<Value, IndexSpecifierList, Allocator> > : std::is_same<Container<Value, IndexSpecifierList, Allocator>, boost::multi_index_container<Value, IndexSpecifierList, Allocator> > {};

#endif

#ifndef ADD_HAS_
#define ADD_HAS_
#define ADD_HAS_MEMBER(MemberName)\
	template<typename T>\
	struct has_member_##MemberName\
	{\
	private:\
		typedef std::true_type yes;\
		typedef std::false_type no;\
		\
		template<typename U> static auto test(int) -> decltype(std::declval<U>().MemberName, yes());\
		\
		template<typename> static no test(...);\
		\
	public:\
		enum { value = std::is_same<decltype(test<T>(0)), yes>::value };\
	}

#define ADD_HAS_MEMBER_FUNCTION(FunctionName)\
	template<typename T, typename... Args>\
	struct has_mem_func_##FunctionName\
	{\
	private:\
		typedef std::true_type yes;\
		typedef std::false_type no;\
		\
		template<typename U> static auto test(int) -> decltype(std::declval<U>().FunctionName(std::declval<Args>()...), yes());\
		\
		template<typename> static no test(...);\
		\
	public:\
		enum { value = std::is_same<decltype(test<T>(0)), yes>::value };\
	}
#endif
}
