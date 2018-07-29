#pragma once

#include "Includes.hpp"

#include <type_traits>

#include "../../TypeTraitsEx.hpp"

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			enum class eSQLParamType
			{
				eSQL_PARAM_INPUT = SQL_PARAM_INPUT,
				eSQL_PARAM_OUTPUT = SQL_PARAM_OUTPUT,
				eSQL_PARAM_INPUT_OUTPUT = SQL_PARAM_INPUT_OUTPUT,
				eSQL_PARAM_RETURN_VALUE = SQL_RETURN_VALUE,
			};

			enum class eSQLCType
			{
				//eSQL_C_BOOL = SQL_C_BIT,
				eSQL_C_SBIGINT = SQL_C_SBIGINT,
				eSQL_C_UBIGINT = SQL_C_UBIGINT,
				eSQL_C_SINT = SQL_C_SLONG,
				eSQL_C_UINT = SQL_C_ULONG,
				eSQL_C_SSMALLINT = SQL_C_SSHORT,
				eSQL_C_USMALLINT = SQL_C_USHORT,
				eSQL_C_STINYINT = SQL_C_STINYINT,
				eSQL_C_UTINYINT = SQL_C_UTINYINT,
				eSQL_C_FLOAT = SQL_C_FLOAT,
				eSQL_C_DOUBLE = SQL_C_DOUBLE,
				eSQL_C_CHAR = SQL_C_CHAR,
				eSQL_C_WCHAR = SQL_C_WCHAR,
				eSQL_C_TIMESTAMP = SQL_C_TIMESTAMP,
			};

			enum class eSQLType
			{
				//eSQL_BOOL = SQL_BIT,
				eSQL_BIGINT = SQL_BIGINT,
				eSQL_INT = SQL_INTEGER,
				eSQL_SMALLINT = SQL_SMALLINT,
				eSQL_TINYINT = SQL_TINYINT,
				eSQL_FLOAT = SQL_FLOAT,
				eSQL_DOUBLE = SQL_DOUBLE,
				eSQL_CHAR = SQL_VARCHAR,
				eSQL_WCHAR = SQL_UNICODE_VARCHAR,
				eSQL_TIMESTAMP = SQL_TIMESTAMP,
			};

			//================================================================================================
// 			template <class T>
// 			constexpr typename std::enable_if<std::is_same<bool, T>::value
// 				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_BOOL; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<__int64, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_SBIGINT; }
			template <class T>
			constexpr typename std::enable_if<std::is_same<unsigned __int64, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_UBIGINT; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<int, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_SINT; }
			template <class T>
			constexpr typename std::enable_if<std::is_same<unsigned int, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_UINT; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<short, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_SSMALLINT; }
			template <class T>
			constexpr typename std::enable_if<std::is_same<unsigned short, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_USMALLINT; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<char, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_STINYINT; }
			template <class T>
			constexpr typename std::enable_if<std::is_same<unsigned char, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_UTINYINT; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<float, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_FLOAT; }
			template <class T>
			constexpr typename std::enable_if<std::is_same<double, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_DOUBLE; }

			template <class T>
			constexpr typename std::enable_if<std::is_array<T>::value & std::is_same<char, std::remove_cv_t<std::remove_all_extents_t<T>>>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_CHAR; }

			template <class T>
			constexpr typename std::enable_if<std::is_array<T>::value & std::is_same<wchar_t, std::remove_cv_t<std::remove_all_extents_t<T>>>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_WCHAR; }

			template <class T>
			constexpr typename std::enable_if<is_array_type<T>::value & std::is_same<char, std::remove_cv_t<typename T::value_type>>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_CHAR; }

			template <class T>
			constexpr typename std::enable_if<is_array_type<T>::value & std::is_same<wchar_t, std::remove_cv_t<typename T::value_type>>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_WCHAR; }

			template <class T>
			constexpr typename std::enable_if<is_string_type<T>::value & std::is_same<char, std::remove_cv_t<typename T::value_type>>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_CHAR; }

			template <class T>
			constexpr typename std::enable_if<is_string_type<T>::value & std::is_same<wchar_t, std::remove_cv_t<typename T::value_type>>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_WCHAR; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<SQL_TIMESTAMP_STRUCT, T>::value
				, eSQLCType>::type GetSQLCType() { return eSQLCType::eSQL_C_TIMESTAMP; }
	
			//================================================================================================
// 			template <class T>
// 			constexpr typename std::enable_if<std::is_same<bool, T>::value
// 				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_BOOL; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<char, T>::value | std::is_same<unsigned char, T>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_TINYINT; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<short, T>::value | std::is_same<unsigned short, T>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_SMALLINT; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<int, T>::value | std::is_same<unsigned int, T>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_INT; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<__int64, T>::value | std::is_same<unsigned __int64, T>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_BIGINT; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<float, T>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_FLOAT; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<double, T>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_DOUBLE; }

			template <class T>
			constexpr typename std::enable_if<std::is_array<T>::value & std::is_same<char, std::remove_cv_t<std::remove_all_extents_t<T>>>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_CHAR; }

			template <class T>
			constexpr typename std::enable_if<std::is_array<T>::value & std::is_same<wchar_t, std::remove_cv_t<std::remove_all_extents_t<T>>>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_WCHAR; }

			template <class T>
			constexpr typename std::enable_if<is_array_type<T>::value & std::is_same<char, std::remove_cv_t<typename T::value_type>>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_CHAR; }

			template <class T>
			constexpr typename std::enable_if<is_array_type<T>::value & std::is_same<wchar_t, std::remove_cv_t<typename T::value_type>>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_WCHAR; }

			template <class T>
			constexpr typename std::enable_if<is_string_type<T>::value & std::is_same<char, std::remove_cv_t<typename T::value_type>>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_CHAR; }

			template <class T>
			constexpr typename std::enable_if<is_string_type<T>::value & std::is_same<wchar_t, std::remove_cv_t<typename T::value_type>>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_WCHAR; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<SQL_TIMESTAMP_STRUCT, T>::value
				, eSQLType>::type GetSQLType() { return eSQLType::eSQL_TIMESTAMP; }

			//================================================================================================
// 			template <class T>
// 			constexpr typename std::enable_if<std::is_same<bool, T>::value
// 				, SQLUINTEGER>::type GetSQLColumnLen() { return 1; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<char, T>::value | std::is_same<unsigned char, T>::value
				, SQLUINTEGER>::type GetSQLColumnLen() { return 3; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<short, T>::value | std::is_same<unsigned short, T>::value
				, SQLUINTEGER>::type GetSQLColumnLen() { return 5; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<int, T>::value | std::is_same<unsigned int, T>::value
				, SQLUINTEGER>::type GetSQLColumnLen() { return 10; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<__int64, T>::value
				, SQLUINTEGER>::type GetSQLColumnLen() { return 19; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<unsigned __int64, T>::value
				, SQLUINTEGER>::type GetSQLColumnLen() { return 20; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<float, T>::value
				, SQLUINTEGER>::type GetSQLColumnLen() { return 7; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<double, T>::value
				, SQLUINTEGER>::type GetSQLColumnLen() { return 15; }

			template <class T>
			constexpr typename std::enable_if<std::is_same<SQL_TIMESTAMP_STRUCT, T>::value
				, SQLUINTEGER>::type GetSQLColumnLen() { return 21; }
		}
	}
}