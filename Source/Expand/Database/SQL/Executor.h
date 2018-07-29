#pragma once

#include "Binder.h"
#include "ResultSet.h"

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			class Executor
			{
				std::shared_ptr<Statement>& m_statement;
			public:
				explicit Executor(std::shared_ptr<Statement>& statement);
				~Executor();

				template <class Params, class ResultDatas>
				bool Execute(Params& params, ResultDatas& resultDatas)
				{
					Binder binder(m_statement);
					if (!binder.BindParameter(params))
						return false;

					using type = meta::to_tstring_t<Builder <T::QueryNameType, Params, T::CallType>::type >;
					if (!m_statement->ExecuteDirect(type::value))
						return false;

					if (std::tuple_size<ResultDatas>::value == 0)
						return true;

					ResultSet results(m_statement);
					while (results.CheckNext())
					{
						unsigned short index = 0;
						if (!results.GetData(index, resultDatas))
							return false;
					}

					return true;
				}
			};

			class DirectExecutor
			{
				std::shared_ptr<Statement>& m_statement;
			public:
				DirectExecutor(std::shared_ptr<Statement>& statement);
				~DirectExecutor();

				template <class... Args>
				bool Execute(const TCHAR* query, Args&... args)
				{
					if (!m_statement->ExecuteDirect(query))
						return false;

 					if (sizeof...(Args) == 0)
 						return true;

					ResultSet results(m_statement);
					while (results.CheckNext())
					{
						unsigned short index = 0;
						if (results.GetData(index, std::tie(args...)))
							return false;
					}

					return true;
				}
			};
		}
	}
}

