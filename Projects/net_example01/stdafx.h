// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#pragma warning(disable : 4250)
#include <Expand.hpp>

#ifdef _DEBUG
#pragma comment(lib, "../../lib/x64/Debug/Expand.lib")
#else
#pragma comment(lib, "../../lib/x64/Release/Expand.lib")
#endif

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
enum class SessionType
{
	User,
};

BIND_ENUM_CLASS_CONVERT(SessionType, User);


enum class CategoryType : std::uint8_t
{
	Test,
};