#include "stdafx.h"
#include "UserLine.h"
#include "TestMsg.h"

bool UserLine::Init()
{
	UserSession::DispatcherMgr::GetInstance().Regist<P_TestMsg_Handler>();

	return true;
}