#pragma once

#include <tuple>
#include <boost/thread/tss.hpp>

#include "Error.h"
#include "TConnection.hpp"
#include "TMultiConnection.hpp"

#include "../../Automation/TForEach.hpp"

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			template <class DatabaseEnum, class... SQLConnectionArgs>
			class TConnectionManager
			{
			public:
				using SQLConnections = std::tuple<boost::thread_specific_ptr<SQLConnectionArgs>...>;

			private:
				std::shared_ptr<Driver> m_driver;
				SQLConnections m_connections;

			public:
				TConnectionManager()
					: m_driver(std::make_shared<Driver>())
				{
				}
				~TConnectionManager() = default;

			public:
				template <class... TConfArgs>		// 
				bool Create(TConfArgs&&... args)
				{
					static_assert(sizeof...(TConfArgs) == sizeof...(SQLConnectionArgs), "missmatch config count");

					if (!ForEach(std::forward_as_tuple(std::forward<TConfArgs>(args)...), [this](auto& var)
					{
						return var.LoadConf();
					}, true))
					{
						return false;
					}

					if (!m_driver->OnCreate())
						return false;

					return true;
				}

				bool StartConnections()
				{
					try
					{
						if (!ForEach(m_connections, [this](auto& var)
						{
							var.reset(new typename std::remove_reference_t<decltype(var)>::element_type(m_driver));

							return var->StartConnection();
						}, true))
						{
							return false;
						}
					}
					catch (...)
					{
						return false;
					}

					return true;
				}

				ADD_HAS_MEMBER_FUNCTION(ExecuteDirect);

				template <DatabaseEnum N, class... Args>
				std::enable_if_t<has_mem_func_ExecuteDirect<
					typename std::tuple_element_t<N, SQLConnections>::element_type, const TCHAR*, Args...>::value, bool>
					ExecuteDirect(const TCHAR* query, Args&... args)
				{	
					return std::get<N>(m_connections)->ExecuteDirect(query, args...);
				}

				template <class T>
				bool Execute(T& query)
				{
					return std::get<T::DatabaseNum>(m_connections)->Execute(query);
				}

				static void SetErrorHandler(std::function<void(const Error&)> func) { Error::SetErrorHandler(func); }
			};
		}
	}
}