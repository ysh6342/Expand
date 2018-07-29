#pragma once

enum eSQLDatabase
{
	eSQLDATABASE_SERVERINFO,
	eSQLDATABASE_GAME,
};

BIND_ENUM_CONVERT(eSQLDatabase
	, eSQLDATABASE_SERVERINFO
	, eSQLDATABASE_GAME
);


namespace SQL = ::Expand::Database::SQL;

using Connection_ServerInfo = SQL::TConnection<eSQLDatabase, eSQLDATABASE_SERVERINFO>;
using Connection_Game = SQL::TMultiConnection<eSQLDatabase, eSQLDATABASE_GAME>;

class ConnectionManager_test
	: public SQL::TConnectionManager<eSQLDatabase
	, Connection_ServerInfo
	, Connection_Game>
{
public:
	ConnectionManager_test();
	~ConnectionManager_test();

private:
	virtual void OnErrorHandle(const SQL::Error& error);
};

