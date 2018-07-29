#pragma once

#include "ConnectionManager_test.h"

template <class T, class query_name_type>
using GameQuery = SQL::TMQuery<T, eSQLDatabase, eSQLDATABASE_GAME, query_name_type>;

class GameQueryTest
	: public GameQuery<GameQueryTest, BOOST_METAPARSE_STRING("sp_test")>
{
	int charID;

	int result;
public:
	GameQueryTest(unsigned short db_num, int id)
		: base(db_num)
		, charID(id), result(0) {}

	BIND_MEM_FUNC_GET_PARAMS(make_in_param(charID));
	BIND_MEM_FUNC_GET_RESULTS(result);
};