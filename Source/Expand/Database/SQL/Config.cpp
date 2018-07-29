#include "Config.h"
//--------------------------------------------------------------
using namespace Expand::Database::SQL;
//==============================================================

Config::Config(const TCHAR* pszDriver, const TCHAR* pszDatabase, const TCHAR* pszIp, const TCHAR* pszId, const TCHAR* pszPass)
	: m_driver(pszDriver)
	, m_database(pszDatabase)
	, m_ip(pszIp)
	, m_id(pszId)
	, m_pass(pszPass)
{

}
Config::~Config()
{
}

Config::ConfStr Config::ToConnectionStr() const
{
	return _T("DRIVER={") + m_driver + _T("};SERVER=") + m_ip + _T(";DATABASE=") + m_database + _T(";UID=") + m_id + _T(";PWD=") + m_pass + _T(";");
}