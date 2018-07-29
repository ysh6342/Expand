#include "Statement.h"
//===========================================================
#include "Connection.h"
#include "../../Error.hpp"
//===========================================================
using namespace Expand::Database::SQL;
//===========================================================
Statement::Statement(std::shared_ptr<Connection>& connection)
	: m_connection(connection)
	, m_hStmt(SQL_NULL_HSTMT)
{
	
}

Statement::~Statement()
{
	OnRelease();
}

bool Statement::OnCreate()
{
	auto connection = m_connection.lock();
	if (connection == nullptr)
	{
		Error::OnError(SQL_INVALID_HANDLE, *this);
		return false;
	}

	SQLRETURN result = SQLAllocStmt(connection->GetDbc(), &m_hStmt);
	if (!SQL_SUCCEEDED(result))
	{
		Error::OnError(result, *this);
		return false;
	}

	return true;
}

void Statement::OnRelease()
{
	if (m_hStmt != SQL_NULL_HSTMT)
	{
		SQLFreeStmt(m_hStmt, SQL_DROP);
//		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
		m_hStmt = SQL_NULL_HSTMT;
	}
}

bool Statement::ExecuteDirect(const TCHAR* query)
{
	SQLRETURN sqlRet = SQLExecDirect(m_hStmt, (SQLWCHAR*)query, SQL_NTS);
	if (sqlRet != SQL_SUCCESS)
	{
		Error::OnError(sqlRet, *this);
	}

	return SQL_SUCCEEDED(sqlRet);
}

bool Statement::Prepare(const TCHAR* query)
{
	SQLRETURN sqlRet = SQLPrepare(m_hStmt, (SQLTCHAR*)query, SQL_NTS);
	if (sqlRet != SQL_SUCCESS)
	{
		Error::OnError(sqlRet, *this);
	}

	return SQL_SUCCEEDED(sqlRet);
}

bool Statement::BindParameter(SQLUSMALLINT paramNum, eSQLParamType paramType, eSQLCType sqlCType, eSQLType sqlType, SQLUINTEGER cbColDef, SQLSMALLINT ibScale, SQLPOINTER pData, SQLINTEGER cbValueMax)
{
	//SQLLEN size = 0;
	SQLRETURN sqlRet = SQLBindParameter(m_hStmt, paramNum, (SQLSMALLINT)paramType, (SQLSMALLINT)sqlCType, (SQLSMALLINT)sqlType, cbColDef, ibScale, pData, cbValueMax, nullptr);
	if (sqlRet != SQL_SUCCESS)
	{
		Error::OnError(sqlRet, *this);
	}

	return SQL_SUCCEEDED(sqlRet);
}

bool Statement::Execute()
{
	SQLRETURN sqlRet = SQLExecute(m_hStmt);
	if (sqlRet != SQL_SUCCESS)
	{
		Error::OnError(sqlRet, *this);
	}

	return SQL_SUCCEEDED(sqlRet);
}

bool Statement::SetFetch()
{
	SQLRETURN sqlRet = SQLFetch(m_hStmt);
	if (sqlRet == SQL_NO_DATA)
		return false;

	if (sqlRet != SQL_SUCCESS)
	{
		Error::OnError(sqlRet, *this);
	}
		
	return SQL_SUCCEEDED(sqlRet);
}

bool Statement::GetData(int columnNumber, eSQLCType sqlCType, SQLPOINTER pData, SQLINTEGER bufferLength)
{
	//SQLLEN size = 0;
	SQLRETURN sqlRet = SQLGetData(m_hStmt, columnNumber, (SQLSMALLINT)sqlCType, pData, bufferLength, nullptr);
	if (sqlRet != SQL_SUCCESS)
	{
		Error::OnError(sqlRet, *this);
	}
// 	if (sqlRet == SQL_SUCCESS || sqlRet == SQL_SUCCESS_WITH_INFO)
// 	{
// 		if (bufferLength > 0)
// 		{
// 			if (size > bufferLength)
// 			{
// 				//DB_LOG(_T("[%s] returnSize(%d) > bufferLength(%d)"), __FUNCTION_T_, size, bufferLength);
// 			}
// 		}
// 	}

	return SQL_SUCCEEDED(sqlRet);
}

void Statement::CloseCursor()
{
	SQLCloseCursor(m_hStmt);
}

void Statement::MoreResults()
{
	while (SQLMoreResults(m_hStmt) == SQL_SUCCESS);
}