#pragma once

#include "../Includes.hpp"
#include "SessionBase.h"
#include "TReciever.hpp"
#include "TSender.hpp"

namespace Expand
{
	namespace Net
	{
		namespace TCP
		{
			//=========================================================================================
			template <class T, class Reciever = TReciever<T>, class Sender = TSendGather<T>>
			class TSession
				: public Reciever
				, public Sender
			{			
			protected:
				using Reciever::Reciever;
				using Sender::Sender;
			};
		}
	}
}