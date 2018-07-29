#pragma once

#include "Includes.hpp"
#include "TParameter.hpp"

#include "../../TypeTraitsEx.hpp"


namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			ADD_HAS_MEMBER_FUNCTION(get_params);

			class Statement;

			class Binder
			{
				std::shared_ptr<Statement>& m_statement;

				unsigned short m_bindNum;
			public:
				explicit Binder(std::shared_ptr<Statement>& statement);				

				template <class T>
				std::enable_if_t<is_tuple_type<T>::value, bool>
					BindParameter(T& rData)
				{
					return ForEach(rData, [this](auto& data)
					{
						return BindParameter(data);
					}, true);
				}
				
				template <class T>
				std::enable_if_t<has_mem_func_get_params<T>::value, bool>
					BindParameter(T& rData)
				{
					return ForEach(rData.get_params(), [this](auto& data)
					{
						return BindParameter(data);
					}, true);
				}

				template <class T>
				std::enable_if_t<is_param_type<T>::value, bool>
					BindParameter(T& rData)
				{
					return BindParameter <T::PARAM_TYPE>(rData.get_value());
// 					return ForEach(rData.GetData(), [this](auto& data)
// 					{
// 						return BindParameter<T::PARAM_TYPE>(data);
// 					}, true);
				}
				
				template <eSQLParamType ParamType, class T>
				std::enable_if_t<std::is_arithmetic<T>::value, bool>
					BindParameter(T& rData)
				{
					return m_statement->BindParameter(++m_bindNum
						, ParamType
						, GetSQLCType<T>()
						, GetSQLType<T>()
						, GetSQLColumnLen<T>()
						, 0
						, &rData
						, sizeof(T)
					);
				}
				
				template <eSQLParamType ParamType, class T>
				std::enable_if_t<is_array_type<T>::value, bool>
					BindParameter(T& rData)
				{
					return m_statement->BindParameter(++m_bindNum
						, ParamType
						, GetSQLCType<T>()
						, GetSQLType<T>()
						, 0
						, 0
						, rData.data()
						, sizeof(T)
					);
				}
				
				template <eSQLParamType ParamType, class T>
				std::enable_if_t<std::is_array<T>::value, bool>
					BindParameter(T& rData)
				{
					return m_statement->BindParameter(++m_bindNum
						, ParamType
						, GetSQLCType<T>()
						, GetSQLType<T>()
						, 0
						, 0
						, &rData
						, sizeof(T)
					);
				}

				template <eSQLParamType ParamType, class T>
				std::enable_if_t<is_string_type<T>::value, bool>
					BindParameter(T& rData)
				{
					static_assert(ParamType == eSQLParamType::eSQL_PARAM_INPUT, "it isn't supported ParamType");

					return m_statement->BindParameter(++m_bindNum
						, ParamType
						, GetSQLCType<T>()
						, GetSQLType<T>()
						, 0
						, 0
						, (SQLPOINTER)rData.data()
						, (SQLINTEGER)rData.length() * sizeof(T::value_type)
					);
				}

				template <eSQLParamType ParamType, class T>
				std::enable_if_t<std::is_same<T, SQL_TIMESTAMP_STRUCT>::value, bool>
					BindParameter(T& rData)
				{
					return m_statement->BindParameter(++m_bindNum
						, ParamType
						, GetSQLCType<T>()
						, GetSQLType<T>()
						, GetSQLColumnLen<T>()
						, 0
						, &rData
						, sizeof(T)
					);
				}

				template <eSQLParamType ParamType, class T>
				std::enable_if_t<std::is_enum<T>::value, bool>
					BindParameter(T& rData)
				{
					return m_statement->BindParameter(++m_bindNum
						, ParamType
						, GetSQLCType<std::underlying_type_t<T>>()
						, GetSQLType<std::underlying_type_t<T>>()
						, GetSQLColumnLen<std::underlying_type_t<T>>()
						, 0
						, &rData
						, sizeof(T)
					);
				}
			};
		}
	}
}
