#pragma once

#include <tchar.h>
#include <string>
#include <forward_list>
#include <list>
#include <vector>
#include <deque>
#include <queue>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <boost/multi_index_container.hpp>
#pragma warning (disable : 4819)
#include <boost/format.hpp>
#pragma warning (default : 4819)

#include <concurrent_vector.h>
#include <concurrent_queue.h>
#include <concurrent_unordered_set.h>
#include <concurrent_unordered_map.h>
#include <concurrent_priority_queue.h>

namespace Expand
{
	template <class T> using TFormat = typename boost::basic_format < T, std::char_traits<T>/* , Memory::TAllocator<T> */ >;

	template <class T> using TString = typename std::basic_string<T, std::char_traits<T>/* , Memory::TAllocator<T> */>;

	template <class T> using TForwardList = typename std::forward_list<T/* , Memory::TAllocator<T> */>;

	template <class T> using TList = typename std::list<T/* , Memory::TAllocator<T> */>;

	template <class T> using TVector = typename std::vector<T/* , Memory::TAllocator<T> */>;

	template <class T> using TDeque = typename std::deque<T/* , Memory::TAllocator<T> */>;

	template <class T> using TQueue = typename std::queue<T/* , Memory::TAllocator<T> */>;

	template <class T> using TSet = typename std::set<T, std::less<T>/* , Memory::TAllocator<T> */>;

	template <class T> using TMultiSet = typename std::multiset<T, std::less<T>/* , Memory::TAllocator<T> */>;

	template <class T> using TUnorderedSet = typename std::unordered_set<T, std::hash<T>, std::equal_to<T>/* , Memory::TAllocator<T> */>;

	template <class T> using TUnorderedMultiSet = typename std::unordered_multiset<T, std::hash<T>, std::equal_to<T>/* , Memory::TAllocator<T> */>;

	template <class T, class U> using TMap = typename std::map<T, U, std::less<T>/*, Memory::TAllocator<std::pair<T, U>>*/>;

	template <class T, class U> using TMultiMap = typename std::multimap<T, U, std::less<T>/*, Memory::TAllocator<std::pair<T, U>>*/>;

	template <class T, class U> using TUnorderedMap = typename std::unordered_map<T, U, std::hash<T>, std::equal_to<T>/*, Memory::TAllocator<std::pair<const T, U>>*/>;

	template <class T, class U> using TUnorderedMultiMap = typename std::unordered_multimap<T, U, std::hash<T>, std::equal_to<T>/*, Memory::TAllocator<std::pair<const T, U>>*/>;

	template <class T, class Indexes> using TMultiIndexContainer = typename boost::multi_index::multi_index_container<T, Indexes/* , Memory::TAllocator<T> */>;
}