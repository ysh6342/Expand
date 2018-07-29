// net_example01.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>

#include "GameNet.h"


int main()
{
	Expand::Error::SetErrorHandler([](const Expand::Error& error)
	{
		std::wcout << error.ToStr() << std::endl;
	});

	GameNet gameNet;

	if (gameNet.Start(_T("./net_example01_conf.ini")))
	{
		getchar();
	}

    return 0;
}