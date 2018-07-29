#pragma once

#include "TTypes.hpp"
#include "StrConvert.hpp"

namespace Expand
{
	//==============================================================================
	class Error
	{
		TString<TCHAR> m_strError;
	public:
		template <class... Args>
		Error(const TCHAR* pszFormat, Args&&... args)
		{
			m_strError = StrConvert::Format(pszFormat, std::forward<Args>(args)...);
		}

		const TCHAR* ToStr() const { return m_strError.c_str(); }

		static void SetErrorHandler(std::function<void(const Error&)> func) { s_errorHandler = func; }
		template <class... Args>
		static void OnError(const TCHAR* pszFormat, Args&&... args)
		{
			if (s_errorHandler)
				s_errorHandler(Error(pszFormat, std::forward<Args>(args)...));
		}

	private:
		inline static std::function<void(const Error&)> s_errorHandler;
	};

	//==============================================================================
	class Exception
		: public Error
		, public std::exception
	{

	public:
		template <class... Args>
		Exception(const TCHAR* pszFormat, Args&&... args)
			: Error(pszFormat, std::forward<Args>(args)...)
			, std::exception(StrConvert::ConvertT2Multi(ToStr()).c_str())
		{

		}
	};
}
