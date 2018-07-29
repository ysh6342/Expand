// net_example01.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>

struct TestMsg 
{
	BIND_PACKET_MEMBERS();
};

using P_TestMsg = Expand::Net::Protocol::TProtocol<TestMsg>;


using namespace Expand;

template <class T>
using P_TestMsg_Handler = Expand::Net::Protocol::TProtocolHandler<TestMsg, T>;


template <class T>
using TBasicSession = Net::TCP::TSession<T>;
//==========================================================================
class UserSession
	: public TBasicSession<UserSession>
{
	using Base = TBasicSession<UserSession>;
public:
	using Base::Base;

	virtual void OnLink() override
	{
		int i = 0;
	}
};
class UserLine
	: public Net::TCP::TServer<SessionType::User, UserSession>
{
	using Base = Net::TCP::TServer<SessionType::User, UserSession>;
public:
	using Base::Base;

	virtual bool Init()
	{
		using DispatcherMgr = TSingleton_Static<UserSession::Dispatcher>;

		DispatcherMgr::GetInstance().Regist<P_TestMsg, P_TestMsg_Handler<UserSession&>>();
		 
		return true;
	}
};
//==========================================================================
using GameNet = Net::TManager<UserLine>;//TSingleton_Static<Net::TManager<UserLine>>;


int main()
{
	Error::SetErrorHandler([](const Error& error)
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

void P_TestMsg_Handler<UserSession&>::_OnComplete(const TestMsg& rMsg, UserSession& pSession)
{
	//std::cout << "user - recv" << std::endl;

	{
		P_TestMsg Msg;
		pSession.SendGatherMsg(Msg);
	}
}