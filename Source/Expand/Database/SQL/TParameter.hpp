#pragma once

#include "Enums.hpp"

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			template <eSQLParamType ParamType, class T>
			class TParameter
			{
			public:
				static constexpr SQL::eSQLParamType PARAM_TYPE = ParamType;
				using value_type = T;
			public:
				TParameter() = delete;
				TParameter(T&& var) : m_var(std::forward<T>(var)) {}

				value_type get_value() { return m_var; }
			private:
				value_type m_var;
			};
			template <class T>
			struct is_param_type : std::false_type {};
			template <SQL::eSQLParamType ParamType, class T>
			struct is_param_type< TParameter<ParamType, T> > : std::true_type {};
		}
	}
}

template <class T>
constexpr decltype(auto)
make_in_param(T& value)
{
	return (Expand::Database::SQL::TParameter<Expand::Database::SQL::eSQLParamType::eSQL_PARAM_INPUT, T&>(value));
}
template <class T>
constexpr decltype(auto)
make_out_param(T& value)
{
	return (Expand::Database::SQL::TParameter<Expand::Database::SQL::eSQLParamType::eSQL_PARAM_OUTPUT, T&>(value));
}
template <class T>
constexpr decltype(auto)
make_inout_param(T& value)
{
	return (Expand::Database::SQL::TParameter<Expand::Database::SQL::eSQLParamType::eSQL_PARAM_INPUT_OUTPUT, T&>(value));
}
template <class T>
constexpr decltype(auto)
make_return_value_param(T& value)
{
	return (Expand::Database::SQL::TParameter<Expand::Database::SQL::eSQLParamType::eSQL_PARAM_RETURN_VALUE, T&>(value));
}

template <class... Args>
constexpr decltype(auto)
make_in_params(Args&... args)
{
	return std::make_tuple(make_in_param(args)...);
}
template <class... Args>
constexpr decltype(auto)
make_out_params(Args&... args)
{
	return std::make_tuple(make_out_param(args)...);
}
template <class... Args>
constexpr decltype(auto)
make_inout_params(Args&... args)
{
	return std::make_tuple(make_inout_params(args)...);
}
