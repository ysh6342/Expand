#include "stdafx.h"
#include "TestMsg.h"
#include "UserLine.h"

void P_TestMsg_Handler::_OnComplete(const TestMsg& rMsg, class UserSession& rUserSession)
{
	P_TestMsg Msg;
	rUserSession.SendGatherMsg(Msg);
}