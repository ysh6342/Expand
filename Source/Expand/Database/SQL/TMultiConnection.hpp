#pragma once

#include "TConnection.hpp"

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			template <class DatabaseEnum, DatabaseEnum Num>
			class TMultiConnection
			{
			public:
				using SQLConnection = TConnection<DatabaseEnum, Num>;
				using SQLMultiConfig = std::vector<Config>;

			protected:
				static SQLMultiConfig s_conf;

			private:
				std::vector<std::shared_ptr<SQLConnection>> m_vecConnection;

			public:
				TMultiConnection(std::shared_ptr<Driver>& driver)
				{
					m_vecConnection.reserve(s_conf.size());
					for (const auto& conf : s_conf)
					{
						m_vecConnection.push_back(std::make_shared<SQLConnection>(conf, driver));
					}
				}

				template <class DatabaseEnum, DatabaseEnum Num, class query_name_type>
				static bool LoadConf()
				{
					auto driver(std::make_shared<Driver>());
					if (!driver->OnCreate())
						return false;

					TConnection<DatabaseEnum, Num> db(driver);
					if (!db.StartConnection())
						return false;

					Expand::Database::TQueryLoadConf<DatabaseEnum, Num, query_name_type> query;
					if (!db.Execute(query))
						return false;

					for (auto& conf : query.m_confs)
					{
						s_conf.emplace_back(conf.m_driver.data()
							, conf.m_database.data()
							, conf.m_ip.data()
							, conf.m_id.data()
							, conf.m_pass.data());
					}
					return true;
				}

				static bool LoadConf(const TCHAR* pszFileName)
				{
					using ConfStr = Expand::Config::INILoader::ConfigStr;
					Expand::Config::INILoader conf(pszFileName);

					conf.SetSection(EnumConverter::ToStr(Num));

					auto count = conf.GetValue<int>(_T("DB_COUNT"));
					if (count == 0)
						return false;

					for (auto i = 0; i < count; ++i)
					{
						conf.SetSection(_T("%s_%d"), EnumConverter::ToStr(Num), i);

						Config config;

						config.m_driver = conf.GetValue<ConfStr>(_T("DRIVER"));
						config.m_database = conf.GetValue<ConfStr>(_T("DATABASE"));
						config.m_ip = conf.GetValue<ConfStr>(_T("IP"));
						config.m_id = conf.GetValue<ConfStr>(_T("ID"));
						config.m_pass = conf.GetValue<ConfStr>(_T("PASS"));

						if (config.m_driver.compare(_T("*")) == 0 ||
							config.m_database.compare(_T("*")) == 0 ||
							config.m_ip.compare(_T("*")) == 0 ||
							config.m_id.compare(_T("*")) == 0 ||
							config.m_pass.compare(_T("*")) == 0
							)
						{
							return false;
						}

						s_conf.push_back(config);
					}

					return true;
				}

				bool StartConnection()
				{
					if (s_conf.empty())
						return false;

					for (size_t i = 0; i < s_conf.size(); ++i)
					{
						if (!m_vecConnection[i]->StartConnection())
							return false;
					}

					return true;
				}

				template <class T>
				bool Execute(T& query)
				{
					auto connection = GetConnection(query.get_db_num());
					if (!connection)
						return false;

					return connection->Execute(query);
				}

			private:
				SQLConnection* GetConnection(unsigned short num)
				{
					if (num >= m_vecConnection.size())
						return nullptr;

					return m_vecConnection[Num].get();
				}
			};

			template <class DatabaseEnum, DatabaseEnum Num>
			typename TMultiConnection<DatabaseEnum, Num>::SQLMultiConfig TMultiConnection<DatabaseEnum, Num>::s_conf;
		}
	}
}