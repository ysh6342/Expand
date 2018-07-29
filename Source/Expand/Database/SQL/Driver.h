#pragma once

#include "Includes.hpp"

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			class Connection;

			class Driver
				: public IBase
				, public std::enable_shared_from_this<Driver>
			{
				SQLHENV m_hEnv;

				Driver(const Driver&) = delete;
				Driver(Driver&&) = delete;
			public:
				Driver();
				~Driver();

				bool OnCreate();
				void OnRelease();

				SQLHENV GetEnv() const { return m_hEnv; }

				std::shared_ptr<Connection> Connect(const TCHAR* pszConnection);

				virtual SQLHANDLE GetHandle() const override { return m_hEnv; }
				virtual SQLUSMALLINT GetType() const override { return SQL_HANDLE_ENV; }
			};
		}
	}
}
