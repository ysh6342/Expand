#pragma once

#include "../NetBase.h"

namespace Expand
{
	namespace Net
	{
		namespace TCP
		{
			class ClientBase
				: public NetBase
			{
			protected:
				using NetBase::NetBase;
			};
		}
	}
}