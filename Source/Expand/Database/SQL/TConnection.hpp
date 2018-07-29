#pragma once

#include "Config.h"
#include "Driver.h"
#include "Connection.h"
#include "Statement.h"
#include "TConfig.hpp"

#include "../../meta.hpp"

#include "../../Config/INILoader.h"

#include <atomic>
#include <future>

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			class async_restore
			{
			public:
				template <class T>
				static auto restore(T& connection)
				{
					return std::async(std::launch::async, [&connection]()
					{
						while (connection.IsRun())
						{
							if (connection.Restore())
								break;

							std::this_thread::sleep_for(std::chrono::seconds(5));
						}
					});
				}
			};

			template <class DatabaseEnum, DatabaseEnum Num, class restore = async_restore>
			class TConnection
			{
			public:
				using SQLDatabase = DatabaseEnum;
				static constexpr SQLDatabase DatabaseNum = Num;

			public:
				enum eState { eSTATE_NONE, eSTATE_CONNECTED, eSTATE_RESTORING };

			private:
				static Config s_conf;

			private:
				const Config& m_conf;
				std::weak_ptr<Driver> m_driver;
				std::shared_ptr<Connection> m_connection;
				std::shared_ptr<Statement> m_statement;

				eState m_state;
				std::shared_future<void> m_restore;
				bool m_run;

			public:
				TConnection(std::shared_ptr<Driver>& driver)
					: m_conf(s_conf)
					, m_driver(driver)
					, m_state(eSTATE_NONE)
					, m_run(true)
				{
				}
				TConnection(const Config& conf, std::shared_ptr<Driver>& driver)
					: m_conf(conf)
					, m_driver(driver)
					, m_state(eSTATE_NONE)
					, m_run(true)
				{
				}
				~TConnection()
				{
					m_run = false;
					if (m_restore.valid()) m_restore.wait();
				}

				bool IsRun() const { return m_run; }
				void ChangeState(eState state) { m_state = state; }

				static bool LoadConf(const TCHAR* pszFileName)
				{
					using ConfStr = Expand::Config::INILoader::ConfigStr;
					Expand::Config::INILoader conf(pszFileName);

					conf.SetSection(EnumConverter::ToStr(Num));

					s_conf.m_driver = conf.GetValue<ConfStr>(_T("DRIVER"));
					s_conf.m_database = conf.GetValue<ConfStr>(_T("DATABASE"));
					s_conf.m_ip = conf.GetValue<ConfStr>(_T("IP"));
					s_conf.m_id = conf.GetValue<ConfStr>(_T("ID"));
					s_conf.m_pass = conf.GetValue<ConfStr>(_T("PASS"));

					if (s_conf.m_driver.compare(_T("*")) == 0 ||
						s_conf.m_database.compare(_T("*")) == 0 ||
						s_conf.m_ip.compare(_T("*")) == 0 ||
						s_conf.m_id.compare(_T("*")) == 0 ||
						s_conf.m_pass.compare(_T("*")) == 0
						)
					{
						return false;
					}

					return true;
				}

				bool StartConnection()
				{
					auto driver = m_driver.lock();
					if (!driver)
						return false;

					auto strCon = m_conf.ToConnectionStr();
					m_connection = driver->Connect(strCon.c_str());
					if (!m_connection)
					{
						OnRestore();
						return false;
					}

					m_statement = m_connection->CreateStatement();

					m_state = eSTATE_CONNECTED;

					return true;
				}

				void OnRestore()
				{
					if (m_state == eSTATE_RESTORING)
						return;

					if (m_connection && m_connection->CheckConnection())
						return;

 					m_state = eSTATE_RESTORING;

					m_restore = restore::restore(*this);
				}

				bool Restore()
				{
					if (!Restart())
						return false;						
					
					if (m_statement)
					{
						m_statement->OnRelease();
						if (!m_statement->OnCreate())
						{
							m_statement.reset();
						}
					}

					m_state = eSTATE_CONNECTED;

					return true;
				}

				template <class... Args>
				bool ExecuteDirect(const TCHAR* query, Args&... args)
				{
					if (!IsConnected())
						return false;

					if (!m_statement)
					{
						m_statement = m_connection->CreateStatement();
						if (!m_statement)
							return false;
					}

					DirectExecutor executor(m_statement);
					if (!executor.Execute(query, args...))
					{
						OnRestore();
						return false;
					}
					return true;
				}

				template <class T>
				bool Execute(T& query)
				{
					if (!IsConnected())
						return false;
					
					if (!m_statement)
					{
						m_statement = m_connection->CreateStatement();
						if (!m_statement)
							return false;
					}

					if (!query.OnExecute(m_statement))
					{
						OnRestore();
						return false;
					}
					return true;
				}

			private:
				bool IsConnected()
				{
					return m_state == eSTATE_CONNECTED;
				}

				bool Restart()
				{
					if (m_connection)
					{
						m_connection->OnRelease();

						auto strCon = m_conf.ToConnectionStr();
						return m_connection->OnCreate(strCon.c_str());
					}

					return StartConnection();
				}
			};

			template <class DatabaseEnum, DatabaseEnum Num, class restore>
			typename Config TConnection<DatabaseEnum, Num, restore>::s_conf;
		}
	}
}
