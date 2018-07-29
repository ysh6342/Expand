#include "Driver.h"
//==========================================================
#include "Connection.h"
#include "../../Error.hpp"
//==========================================================
using namespace Expand::Database::SQL;
//==========================================================
Driver::Driver()
	: m_hEnv(SQL_NULL_HENV)
{
	
}

Driver::~Driver()
{
	OnRelease();
}

bool Driver::OnCreate()
{
	SQLRETURN result = SQLAllocEnv(&m_hEnv);
	if (!SQL_SUCCEEDED(result))
	{
		Error::OnError(_T("[%s] Error : %d "), __FUNCTIONT__, result);
		return false;
	}

	result = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), SQL_IS_INTEGER);
	if (!SQL_SUCCEEDED(result))
	{
		Error::OnError(_T("[%s] Error : %d "), __FUNCTIONT__, result);
		return false;
	}

	return true;
}

void Driver::OnRelease()
{
	if (m_hEnv != SQL_NULL_HENV)
	{
		SQLFreeEnv(m_hEnv);
		m_hEnv = SQL_NULL_HENV;
	}
}

std::shared_ptr<Connection> Driver::Connect(const TCHAR* pszConnection)
{
	auto pConnection = std::make_shared<Connection>(shared_from_this());
	if (pConnection)
	{
		if (pConnection->OnCreate(pszConnection))
			return std::move(pConnection);
	}

	return nullptr;
}