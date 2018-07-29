#pragma once

#include "TProtocol.hpp"

namespace Expand
{
	namespace Net
	{
		namespace Protocol
		{
			class IPacket
			{
			public:
				virtual ~IPacket() = 0 {}

				virtual char* GetPtr() = 0;
				virtual short GetSize() const = 0;
				virtual short GetType() const = 0;
			};
		}
	};
};