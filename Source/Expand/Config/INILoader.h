#pragma once

#include <windows.h>
#include <string>
#include <tchar.h>

#include "../StrConvert.hpp"
#include "../TTypes.hpp"

namespace Expand
{
	namespace Config
	{
		class INILoader
		{
		public:
			template <class T>
			using TConfigStr = Expand::TString<T>;

			using ConfigStr = TConfigStr<TCHAR>;

		private:
			ConfigStr m_strFileName;
			ConfigStr m_strSectionName;

		public:
			INILoader(const TCHAR* pszFileName);
			virtual	~INILoader();

			void SetSection(const TCHAR* pszSection, ...);

			template <class T>
			T GetValue(const TCHAR* pszKey)
			{
				return static_cast<T>(GetPrivateProfileInt(m_strSectionName.c_str(), pszKey, 0, m_strFileName.c_str()));
			}
			template <>
			bool GetValue<bool>(const TCHAR* pszKey)
			{
				return 0 < GetPrivateProfileInt(m_strSectionName.c_str(), pszKey, 0, m_strFileName.c_str());
			}

			template <>
			TConfigStr<wchar_t> GetValue<TConfigStr<wchar_t>>(const TCHAR* pszKey)
			{
				TCHAR szTemp[1024] = { 0, };
				GetPrivateProfileString(m_strSectionName.c_str(), pszKey, _T("*"), szTemp, _countof(szTemp), m_strFileName.c_str());
				return StrConvert::ConvertT2Wide(szTemp);
			}
			template <>
			TConfigStr<char> GetValue<TConfigStr<char>>(const TCHAR* pszKey)
			{
				TCHAR szTemp[1024] = { 0, };
				GetPrivateProfileString(m_strSectionName.c_str(), pszKey, _T("*"), szTemp, _countof(szTemp), m_strFileName.c_str());
				return StrConvert::ConvertT2Multi(szTemp);
			}

			ConfigStr GetValueStr(const TCHAR* pszKey);
		};
	}
}
