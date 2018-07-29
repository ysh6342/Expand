// net_example01.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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