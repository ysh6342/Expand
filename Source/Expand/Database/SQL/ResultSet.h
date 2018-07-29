#pragma once

#include "Enums.hpp"

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			ADD_HAS_MEMBER_FUNCTION(get_results);

			ADD_HAS_MEMBER_FUNCTION(push_back);

			class Statement;

			class ResultSet
			{
				std::shared_ptr<Statement>& m_statement;
			public:
				ResultSet(std::shared_ptr<Statement>& statement);

				bool CheckNext();

				template <class T>
				typename std::enable_if<is_element_container<T>::value & has_mem_func_push_back<T, typename T::value_type>::value
					, bool>::type GetData(unsigned short& rIndex, T& rData)
				{
					T::value_type data;
					if (GetData(rIndex, data))
					{
						rData.push_back(data);
				
						return true;
					}
				
					return false;
				}
				
				template <class T>
				typename std::enable_if<is_tuple_type<T>::value, bool>::type GetData(unsigned short& rIndex, T& rData)
				{
					return ForEach(rData, [this, &rIndex](auto& var)
					{
						return GetData(rIndex, var);
					}, true);
				}
				
				template <class T>
				typename std::enable_if<has_mem_func_get_results<T>::value, bool>::type GetData(unsigned short& rIndex, T& rData)
				{
					return ForEach(rData.get_results(), [this, &rIndex](auto& var)
					{
						return GetData(rIndex, var);
					}, true);
				}
				
				template <class T>
				typename std::enable_if<is_shared_ptr<T>::value, bool>::type GetData(unsigned short& rIndex, T& rData)
				{
					rData = std::make_shared<T::element_type>();
					return GetData(rIndex, *rData);
				}
				
				template <class T>
				typename std::enable_if<std::is_arithmetic<T>::value, bool>::type GetData(unsigned short& rIndex, T& rData)
				{
					return m_statement->GetData(++rIndex, GetSQLCType<T>(), &rData);
				}
				
				template <class T>
				typename std::enable_if<is_array_type<T>::value, bool>::type GetData(unsigned short& rIndex, T& rData)
				{
					return m_statement->GetData(++rIndex, GetSQLCType<T>(), rData.data(), sizeof(T));
				}
				
				template <class T>
				typename std::enable_if<std::is_array<T>::value, bool>::type GetData(unsigned short& rIndex, T& rData)
				{
					return m_statement->GetData(++rIndex, GetSQLCType<T>(), rData, sizeof(T));
				}
				
				template <class T>
				typename std::enable_if<std::is_enum<T>::value, bool>::type GetData(unsigned short& rIndex, T& rData)
				{				
					return m_statement->GetData(++rIndex, GetSQLCType<std::underlying_type_t<T>>, &rData);
				}
				
				template <class T>
				typename std::enable_if<std::is_same<T, SQL_TIMESTAMP_STRUCT>::value, bool>::type GetData(unsigned short& rIndex, T& rData)
				{
// 					bool result = 
// 					if (result)
// 					{
// 						rData.tm_year = tm.year - 1900;
// 						rData.tm_mon = tm.month - 1;
// 						rData.tm_mday = tm.day;
// 						rData.tm_hour = tm.hour;
// 						rData.tm_min = tm.minute;
// 						rData.tm_sec = tm.second;
// 						rData.tm_isdst = 0;
// 					}
					return m_statement->GetData(++rIndex, GetSQLCType<T>(), &rData);;
				}
			};
		}
	}
}