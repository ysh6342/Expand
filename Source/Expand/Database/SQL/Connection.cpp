#include "Connection.h"
//======================================================
#include "Driver.h"
#include "Statement.h"
//======================================================
#include "../../Error.hpp"
//======================================================
using namespace Expand::Database::SQL;
//======================================================
Connection::Connection(std::shared_ptr<Driver>& driver)
	: m_driver(driver)
	, m_hDbc(SQL_NULL_HDBC)
{
	
}

Connection::~Connection()
{
	OnRelease();
}

bool Connection::OnCreate(const TCHAR* pszConnection)
{
	auto driver = m_driver.lock();
	if (driver == nullptr)
	{
		Error::OnError(_T("[%s] Error : Invalid Driver "), __FUNCTIONT__);
		return false;
	}

	SQLRETURN result = SQLAllocConnect(driver->GetEnv(), &m_hDbc);
	if (!SQL_SUCCEEDED(result))
	{
		Error::OnError(result, *this);
		return false;
	}

	result = SQLDriverConnect(m_hDbc, nullptr, (SQLTCHAR*)pszConnection, SQL_NTS, nullptr, 0, nullptr, SQL_DRIVER_NOPROMPT);
	if (!SQL_SUCCEEDED(result))
	{
		Error::OnError(result, *this);
		return false;
	}

	return true;
}

void Connection::OnRelease()
{
	if (m_hDbc != SQL_NULL_HDBC)
	{
		SQLDisconnect(m_hDbc);
		SQLFreeConnect(m_hDbc);
		m_hDbc = SQL_NULL_HDBC;
	}
}

bool Connection::CheckConnection()
{
	if (m_hDbc == SQL_NULL_HDBC)
		return false;

	SQLUINTEGER	uIntVal;
	SQLRETURN retcode = SQLGetConnectAttr(m_hDbc
		, SQL_ATTR_CONNECTION_DEAD
		, (SQLPOINTER)&uIntVal
		, (SQLINTEGER) sizeof(uIntVal)
		, nullptr);

	return uIntVal != SQL_CD_TRUE;
}

std::shared_ptr<Statement> Connection::CreateStatement()
{
	auto pStatement = std::make_shared<Statement>(shared_from_this());
	if (pStatement)
	{
		if (pStatement->OnCreate())
			return std::move(pStatement);
	}
	
	return nullptr;
}