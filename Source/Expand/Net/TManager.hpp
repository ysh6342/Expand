#pragma once

#include "../Config/INILoader.h"
#include <thread>

namespace Expand
{
	namespace Net
	{
		template <class... Args>
		class TManager
		{
			boost::asio::io_context m_io_context;
			std::tuple<std::shared_ptr<Args>...> m_nets;

			std::vector<std::thread> m_vec_thread;
			bool m_bRun = true;
		public:
			TManager() = default;
			~TManager() 
			{ 
				m_bRun = false;  
				for (auto& thread : m_vec_thread)
				{
					if (thread.joinable())
						thread.join();
				}
			}

			bool Start(const TCHAR* pszFileName)
			{
				Config::INILoader loader(pszFileName);

				loader.SetSection(_T("BASIC_INFO"));

				int threadCnt = loader.GetValue<int>(_T("THREAD_CNT"));
				if (threadCnt <= 0)
				{
					Error::OnError(_T("[%s] THREAD_CNT = %d"), __FUNCTIONT__, threadCnt);
					return false;
				}

				m_vec_thread.reserve(threadCnt);
				for (int i = 0; i < threadCnt; ++i)
				{
					m_vec_thread.emplace_back([this]()
					{
						Run();
					});
				}

				return ForEach(m_nets, [this, &loader](auto& var)
				{
					var.reset(new typename std::decay_t<decltype(var)>::element_type(m_io_context));

					if (var->Init())
						return var->Start(loader);

					return false;
				}, true);
			}

			template <class T>
			std::shared_ptr<T> GetNet() { return std::get<T>(m_nets); }

		private:
			void Run()
			{
				while (m_bRun)
				{
					try
					{
						m_io_context.run();
					}
					catch (const std::exception& e)
					{
						Error::OnError(_T("[%s] Error : %s"), __FUNCTIONT__, StrConvert::ConvertMulti2T(e.what()).c_str());
					}

					std::this_thread::sleep_for(std::chrono::microseconds(1));
				}
			}
		};
	}
}
