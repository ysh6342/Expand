#pragma once

#include "Enums.hpp"

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			class Connection;

			class Statement
				: public IBase
				, public std::enable_shared_from_this<Statement>
			{
				std::weak_ptr<Connection> m_connection;

				SQLHSTMT m_hStmt;

				Statement(const Statement&) = delete;
				Statement(Statement&&) = delete;
			public:
				Statement(std::shared_ptr<Connection>& connection);
				~Statement();

				bool OnCreate();
				void OnRelease();

				bool ExecuteDirect(const TCHAR* query);

				bool Prepare(const TCHAR* query);
				bool BindParameter(SQLUSMALLINT paramNum, eSQLParamType paramType, eSQLCType sqlCType, eSQLType sqlType, SQLUINTEGER cbColDef, SQLSMALLINT ibScale, SQLPOINTER pData, SQLINTEGER cbValueMax);
				bool Execute();

				bool SetFetch();
				bool GetData(int columnNumber, eSQLCType sqlCType, SQLPOINTER pData, SQLINTEGER bufferLength = 0);

				void CloseCursor();
				void MoreResults();

				SQLHSTMT GetStmt() const { return m_hStmt; }

				virtual SQLHANDLE GetHandle() const override { return m_hStmt; }
				virtual SQLUSMALLINT GetType() const override { return SQL_HANDLE_STMT; }
			};
		}
	}
}
