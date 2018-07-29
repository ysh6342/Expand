#pragma once

#include <boost/metaparse/string.hpp>

#include "Executor.h"

#include "Builder.hpp"

#define BIND_MEM_FUNC_GET_PARAMS(...)	BIND_MEM_FUNC_TUPLE(get_params, __VA_ARGS__)
#define BIND_MEM_FUNC_GET_RESULTS(...)	BIND_MEM_FUNC_TIE(get_results, __VA_ARGS__)

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			using CALL_RETURN_SQL = BOOST_METAPARSE_STRING("{ ? = CALL ");
			using CALL_SQL = BOOST_METAPARSE_STRING("{ CALL ");

			template <class T, class DatabaseEnum, DatabaseEnum N, class query_name_type, class CALL = CALL_SQL>
			class TQuery
			{
			public:
				using QueryNameType = meta::convert_t<query_name_type>;
				using CallType = meta::convert_t<CALL>;

			protected:
				using type = TQuery<T, DatabaseEnum, N, query_name_type>;
				TQuery() = default;

			public:
				static constexpr DatabaseEnum DatabaseNum = N;

				virtual bool OnExecute(std::shared_ptr<Statement>& statement)
				{
					auto self = static_cast<T*>(this);
					Executor executor(statement);
					if (!executor.Execute(self->get_params(), self->get_results()))
						return false;

					return true;
				}
			};

			template <class T, class DatabaseEnum, DatabaseEnum N, class query_name_type>
			class TRQuery
				: public TQuery<T, DatabaseEnum, N, query_name_type, CALL_RETURN_SQL>
			{
				int m_result;
			protected:
				using type = TRQuery<T, DatabaseEnum, N, query_name_type>;
				TRQuery() = default;

				virtual bool OnExecute(std::shared_ptr<Statement>& statement) override
				{
					auto self = static_cast<T*>(this)
					Executor executor(statement);
					if (!executor.Execute(std::make_tuple(make_return_value_param(m_result), self->get_params()), self->get_results()))
						return false;

					return true;
				}
			public:
				int get_result() const { return m_result; }
			};

			template <class T, class DatabaseEnum, DatabaseEnum N, class query_name_type>
			class TMQuery
				: public TQuery<T, DatabaseEnum, N, query_name_type>
			{
				unsigned short m_db_num;
			protected:
				using type = TMQuery<T, DatabaseEnum, N, query_name_type>;
				TMQuery(unsigned short db_num)
					: m_db_num(db_num) {}
			public:
				unsigned short get_db_num() const { return m_db_num; }
			};

			template <class T, class DatabaseEnum, DatabaseEnum N, class query_name_type>
			class TMRQuery
				: public TRQuery<T, DatabaseEnum, N, query_name_type>
			{
				unsigned short m_db_num;
			protected:
				TMRQuery(unsigned short db_num)
					: m_db_num(db_num) {}

			public:
				unsigned short get_db_num() const { return m_db_num; }
			};

			template <class DatabaseEnum, DatabaseEnum Num, class query_name_type>
			class TQueryLoadConf
				: public TQuery< TQueryLoadConf<DatabaseEnum, Num, query_name_type>
				, DatabaseEnum
				, Num
				, query_name_type>
			{
				struct Conf
				{
					std::array<TCHAR, 32> m_driver;
					std::array<TCHAR, 32> m_database;
					std::array<TCHAR, 32> m_ip;
					std::array<TCHAR, 32> m_id;
					std::array<TCHAR, 32> m_pass;

					BIND_MEM_FUNC_GET_RESULTS(m_driver
						, m_database
						, m_ip
						, m_id
						, m_pass
					);
				};

			public:
				std::list<Conf> m_confs;

			public:
				BIND_MEM_FUNC_GET_PARAMS();
				BIND_MEM_FUNC_GET_RESULTS(m_confs);
			};
		}
	}
}