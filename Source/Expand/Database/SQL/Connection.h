#pragma once

#include "Includes.hpp"
#include <functional>

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			class Driver;
			class Statement;

			class Connection
				: public IBase
				, public std::enable_shared_from_this<Connection>
			{
				std::weak_ptr<Driver> m_driver;

				SQLHDBC m_hDbc;

				Connection(const Connection&) = delete;
				Connection(Connection&&) = delete;
			public:
				Connection(std::shared_ptr<Driver>& driver);
				~Connection();

				bool OnCreate(const TCHAR* pszConnection);
				void OnRelease();

				bool CheckConnection();

				SQLHDBC GetDbc() const { return m_hDbc; }

				std::shared_ptr<Statement> CreateStatement();

				virtual SQLHANDLE GetHandle() const override { return m_hDbc; }
				virtual SQLUSMALLINT GetType() const override { return SQL_HANDLE_DBC; }
			};
		}
	}
}
