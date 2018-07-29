#pragma once

namespace Expand
{
	namespace Database
	{
		namespace SQL
		{
			template <class T>
			struct TConfFile 
			{
				const TCHAR* pszFileName;

				bool LoadConf()
				{
					return typename T::LoadConf(pszFileName);
				}
			};

			template <class T, class DatabaseEnum, DatabaseEnum Num, class query_name_type>
			struct TConfSQL
			{
				bool LoadConf()
				{
					return typename T::LoadConf<DatabaseEnum, Num, query_name_type>();
				}
			};
		}
	}
}