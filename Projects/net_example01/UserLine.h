#pragma once


//==========================================================================
class UserSession
	: public Expand::Net::TCP::TSession<UserSession>
{
	using Base = Expand::Net::TCP::TSession<UserSession>;
public:
	using Base::Base;
};

class UserLine
	: public Expand::Net::TCP::TServer<SessionType::User, UserSession>
{
	using Base = Expand::Net::TCP::TServer<SessionType::User, UserSession>;
public:
	using Base::Base;

	virtual bool Init();
};