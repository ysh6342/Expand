#pragma once

#include <tchar.h>
#include <string>
#include <map>
#include <algorithm>
#include <functional>
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/wstringize.hpp>

#include "../TTypes.hpp"


template <typename T, T... Args>
struct enum_sequence
{
	static_assert(std::is_enum<T>::value, "enum_sequence<T, I...> requires T to be an enum type.");

	using type = enum_sequence<T, Args...>;
	using value_type = T;

	static constexpr size_t size() noexcept
	{ // get length of parameter list
		return (sizeof...(Args));
	}
};

template<typename T>
class TEnum
{
public:
	typedef Expand::TString<TCHAR> EnumStr;
	typedef Expand::TUnorderedMap<T, EnumStr> MapEnumStr;
	typedef Expand::TUnorderedMap<EnumStr, T> MapEnum;

public:
	TEnum()
	{
		static Init<TEnum> s_init;
	}

	T GetEnum(const TCHAR* strValue)
	{
		MapEnum::iterator itFind = m_mapEnum.find(strValue);
		if (itFind == m_mapEnum.end())
			throw std::exception("Not found Enum Value");

		return itFind->second;
	}
	const TCHAR* GetStr(T enumValue)
	{
		MapEnumStr::iterator itFind = m_mapEnumStr.find(enumValue);
		if (itFind == m_mapEnumStr.end())
			throw std::exception("Not found Enum Str");

		return itFind->second.c_str();
	}

	static TEnum& GetInstance() { return s_instance; }

private:
	template <class T>
	friend struct Init;
	static TEnum s_instance;

	void _InsertMap(T enumValue, const TCHAR* strValue)
	{
		m_mapEnumStr.insert(std::make_pair(enumValue, strValue));
		m_mapEnum.insert(std::make_pair(strValue, enumValue));
	}

private:
	MapEnumStr m_mapEnumStr;
	MapEnum m_mapEnum;
};

template <class T>
typename TEnum<T>::TEnum TEnum<T>::s_instance;

#ifdef UNICODE
#define BOOST_PP_TSTRINGIZE(x)  BOOST_PP_WSTRINGIZE(x)
#else
#define BOOST_PP_TSTRINGIZE(x)  BOOST_PP_STRINGIZE(x)
#endif

template <class T>
struct Init {};

namespace EnumConverter
{
	template <class T>
	inline T ToEnum(const TCHAR* strValue)\
	{
		static_assert(0, "Not Found BIND_ENUM_CONVERT");
	}

	template <class T>
	struct enum_type { using type = T; };
}

#define MAKE_ENUM_CONVERT( Enum )\
namespace EnumConverter\
{\
 template <>\
 inline Enum ToEnum<Enum>(const TCHAR* strValue)\
 {\
  return Enum##_Convert ::GetInstance().GetEnum(strValue);\
 }\
 inline const TCHAR* ToStr(Enum enumValue)\
 {\
  return Enum##_Convert ::GetInstance().GetStr(enumValue);\
 }\
}

#define BIND_ENUM_VALUE(r, data, element) data##_Convert ::s_instance._InsertMap(element, BOOST_PP_TSTRINGIZE(element));
#define BIND_ENUM_CLASS_VALUE(r, data, element) data##_Convert ::s_instance._InsertMap(data::element, BOOST_PP_TSTRINGIZE(element));

#define SEQ_TO_ENUM_VALUE(r, data, i, element) BOOST_PP_COMMA_IF(i)  data::element
#define VALIADIC_TO_ENUM_VALUE(Enum, ...) BOOST_PP_SEQ_FOR_EACH_I(SEQ_TO_ENUM_VALUE, Enum, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define BIND_ENUM_CONVERT_M( Macro, Enum, ... )\
typedef TEnum<Enum> Enum##_Convert;\
template<>\
struct Init<TEnum<Enum>>\
{\
 Init() { BOOST_PP_SEQ_FOR_EACH(Macro, Enum, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)); }\
};\
namespace EnumConverter\
{\
 template <>\
 struct enum_type<Enum> { using type = enum_sequence<Enum, VALIADIC_TO_ENUM_VALUE(Enum, __VA_ARGS__)>; }; \
}\
MAKE_ENUM_CONVERT( Enum )

#define BIND_ENUM_CONVERT( Enum, ...)  BIND_ENUM_CONVERT_M( BIND_ENUM_VALUE, Enum, __VA_ARGS__)
#define BIND_ENUM_CLASS_CONVERT( Enum, ...) BIND_ENUM_CONVERT_M( BIND_ENUM_CLASS_VALUE, Enum, __VA_ARGS__)

/* example
enum class eTest
{
eTEST_1,
eTEST_2,
eTEST_3,
};


BIND_ENUM_CLASS_CONVERT(eTest,
eTEST_1,
eTEST_2,
eTEST_3
);

auto enumVar = EnumConverter::ToEnum<eTest>(_T("eTEST_1"));
auto strVar = EnumConverter::ToStr(eTest::eTEST_1);
*/
/*example
enum eTest
{
eTEST_1,
eTEST_2,
eTEST_3,
};


BIND_ENUM_CONVERT(eTest,
eTEST_1,
eTEST_2,
eTEST_3
);

auto enumVar = EnumConverter::ToEnum<eTest>(_T("eTEST_1"));
auto strVar = EnumConverter::ToStr(eTEST_1);
*/
