#include "Expand/Config/INILoader.h"
//------------------------------------
#include <stdarg.h>
#include <codecvt>
//-------------------------------------

//============================================
using namespace Expand::Config;
//============================================

INILoader::INILoader(const TCHAR* pszFileName)
	: m_strFileName(pszFileName)
{

}

INILoader::~INILoader()
{

}

void INILoader::SetSection(const TCHAR* pszSection, ...)
{
	va_list args;
	va_start(args, pszSection);
	m_strSectionName.resize(_vsctprintf(pszSection, args) + 1);
	_vstprintf_s((TCHAR*)m_strSectionName.c_str(), m_strSectionName.size(), pszSection, args);
	va_end(args);
}

INILoader::ConfigStr INILoader::GetValueStr(const TCHAR* pszKey)
{
	return GetValue<ConfigStr>(pszKey);
}