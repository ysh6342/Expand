#pragma once

enum class ProtocolType : std::uint8_t
{
	Test,
};

struct TestMsg
{
	BIND_PACKET_MEMBERS();
};

using P_TestMsg = Expand::Net::Protocol::TProtocol<TestMsg, CategoryType::Test, ProtocolType::Test>;

using P_TestMsg_Handler = Expand::Net::Protocol::TProtocolHandler<P_TestMsg, class UserSession&>;

