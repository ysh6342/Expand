#pragma once

#include <boost/preprocessor.hpp>
#include <tuple>

#define BIND_MEM_FUNC_TIE(func_name, ...)\
auto func_name () const\
{\
	return std::tie(__VA_ARGS__);\
}\
auto func_name ()\
{\
	return std::tie(__VA_ARGS__);\
}

#define BIND_MEM_FUNC_TUPLE(func_name, ...)\
auto func_name ()\
{\
	return std::make_tuple(__VA_ARGS__);\
}

#define BIND_MEM_FUNC_FORWARD_AS_TUPLE(func_name, ...)\
auto func_name ()\
{\
	return std::forward_as_tuple(__VA_ARGS__);\
}


#define SEQ_TO_STRINGIZE(r, data, i, element)	BOOST_PP_COMMA_IF(i)	BOOST_PP_TSTRINGIZE(element)

#define VALIADIC_TO_STRINGIZE(...)	BOOST_PP_SEQ_FOR_EACH_I(SEQ_TO_STRINGIZE, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define BIND_MEM_FUNC_GET_MEMBERNAMES(...)\
static auto get_membernames() const ->decltype(std::make_tuple(VALIADIC_TO_STRINGIZE(__VA_ARGS__)))\
{\
	static constexpr auto value = std::make_tuple(VALIADIC_TO_STRINGIZE(__VA_ARGS__));\
	return value;\
}