#pragma once

#include "Includes.hpp"

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			class Config
			{
			public:
				using ConfStr = std::tstring;

				ConfStr m_driver;
				ConfStr m_database;
				ConfStr m_ip;
				ConfStr m_id;
				ConfStr m_pass;

			public:
				Config() = default;
				Config(const TCHAR* pszDriver, const TCHAR* pszDatabase, const TCHAR* pszIp, const TCHAR* pszId, const TCHAR* pszPass);
				virtual ~Config();

				ConfStr ToConnectionStr() const;
			};
		}
	}
}
