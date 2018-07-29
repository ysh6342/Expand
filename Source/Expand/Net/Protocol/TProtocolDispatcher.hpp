#pragma once

#include <unordered_map>
#include "TProtocolToType.hpp"
#include "../../TConcreteCreator.hpp"

namespace Expand
{
	namespace Net
	{
		namespace Protocol
		{
			class IPacket;

			template <class Handler>
			class TProtocolDispatcher
			{
				typedef std::unordered_map<int, std::unique_ptr<Handler>> MapFactory;
				MapFactory m_mapFactory;

			public:
				TProtocolDispatcher()
				{
				}
				~TProtocolDispatcher()
				{
				}

				template <class T>
				bool Regist()
				{
					MapFactory::iterator itFind = m_mapFactory.find(T::Type::value);
					if (itFind == m_mapFactory.end())
					{
						return m_mapFactory.try_emplace(T::Type::value, new T()).second;
					}

					return false;
				}
				Handler* GetHandler(short type)
				{
					MapFactory::iterator itFind = m_mapFactory.find(type);
					if (itFind != m_mapFactory.end())
					{
						return itFind->second.get();
					}

					return nullptr;
				}
			};
		};
	}
};