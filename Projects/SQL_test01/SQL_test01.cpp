// SQL_test01.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "ConnectionManager_test.h"

#include "Query_test.h"

#include <iostream>

#include "Expand/typetraitsex.hpp"
// 
// template <class T>
// constexpr auto to_tuple(T&& var)
// {
// 	return to_tuple_impl<Expand::is_tuple_type<T>::value>::convert(std::forward<T>(var));
// }
// 
// template <bool b = false>
// struct to_tuple_impl
// {
// 	template <class T>
// 	static constexpr auto convert(T&& var)
// 	{
// 		return std::forward_as_tuple(std::forward<T>(var));
// 	}
// };
// 
// template <>
// struct to_tuple_impl<true>
// {
// 	template <class T>
// 	static constexpr auto convert(T&& var)
// 	{
// 		return std::forward<T>(var);
// 	}
// };
// 
// template <class... Args>
// auto to_tuple_s(Args&&... args)
// {
// 	return std::tuple_cat(to_tuple(std::forward<Args>(args))...);
// }

int main()
{
// 	int asdf = 4;
// 
// 	auto func = [v2 = to_tuple_s(1, 2,3,4, std::forward_as_tuple(1,2,3))]()
// 	{
// 
// 	};
// 
// 	func();
// 
// 	int n = 0;

// 	using build = SQL::Builder < BOOST_METAPARSE_STRING("sp_test"), std::tuple<
// 		SQL::TParameter<Expand::Database::SQL::eSQL_PARAM_INPUT
// 		, int
// 		, int
// 		, int
// 		, int
// 		, int
// 		, int>, 
// 		SQL::TParameter<Expand::Database::SQL::eSQL_PARAM_OUTPUT
// 		, int
// 		, int
// 		, int
// 		, int
// 		, int
// 		, int>>, SQL::CALL_SQL>::type;
// 
// 	std::wcout << ::Expand::meta::c_str<build>::value << std::endl;
	
	ConnectionManager_test con;

	const TCHAR* pszConfFile = _T("conf.ini");
	if (con.Create(SQL::TConfFile<Connection_ServerInfo>{pszConfFile}
	, SQL::TConfFile<Connection_Game>{pszConfFile}))
	{
		bool exit = false;
		std::thread th([&con, &exit]()
		{
			if (con.StartConnections())
			{
				while (!exit)
				{
					GameQueryTest query(0,1);
					if (con.Execute(query))
					{
						auto result = query.get_results();
					}

					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
			}
		});

		if (th.joinable())
			th.join();
	}
	
    return 0;
}


