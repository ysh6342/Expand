#pragma once

#include <memory>

namespace Expand
{
	/*
	abstract factory pattern example

	class AbstractFactory
	{
	public:
	virtual Product1* CreateProduct1() = 0;
	virtual Product2* CreateProduct2() = 0;
	};

	class ConcreteFactory
	{
	public:
	virtual Product1* CreateProduct1() { return TAbstractCreator<Product1, 구체화>::Create(a); }
	virtual Product2* CreateProduct2() { return TAbstractCreator<Product2, 구체화>::Create(a,b); }
	};
	*/

	template <class T, class U>
	class TAbstractCreator
	{
	public:
		template <class... Args>
		static T* Create(Args&&... args)
		{
			return new U(std::forward<Args>(args)...);
		}
	};

	template <class T, class... Args>
	class TCreator
	{
	public:
		virtual ~TCreator() {}

		// factory method pattern
		virtual T* Create(Args... args) = 0;
	};

	template <class T, class U, class... Args>
	class TConcreteCreator : public TCreator<T, Args...>
	{
	public:
		virtual ~TConcreteCreator() {}

		virtual T* Create(Args... args) override sealed
		{
			return new U(std::forward<Args>(args)...);
		}
	};

	template <template <class, class, class...> class MapContainer, class T, class Key, class... Args>
	class TConcreteCreatorManager
	{
		typedef MapContainer<Key, std::shared_ptr< TCreator<T> > > MapFactory;

		MapFactory m_mapFactory;

	public:
		virtual ~TConcreteCreatorManager() {}

		template <class V>
		bool Register(Key key)
		{
			MapFactory::iterator itFind = m_mapFactory.find(key);
			if (itFind == m_mapFactory.end())
			{
				m_mapFactory.emplace(key, std::shared_ptr< TCreator<T> >(new TConcreteCreator<T, V>()));
				return true;
			}

			return false;
		}

		std::shared_ptr<T> Create(Key key, Args... args)
		{
			MapFactory::iterator itFind = m_mapFactory.find(key);
			if (itFind != m_mapFactory.end())
			{
				return std::shared_ptr<T>(itFind->second->Create(std::forward<Args>(args)...));
			}

			return std::shared_ptr<T>();
		}
	};
};