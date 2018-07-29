#pragma once

#include <boost/thread/tss.hpp>

namespace Expand
{
	//===================================================================================
	template<class T>
	class TSingleton_Static
	{
		TSingleton_Static() = delete;
	public:
		static T& GetInstance() { static T s_Instance; return s_Instance; }
	};

	//===================================================================================
	template <class T>
	class TSingleton_Local
	{
		TSingleton_Local() = delete;
		static boost::thread_specific_ptr<T> s_pInstance;
	public:
		template <class... Args>
		static T* CreateInstance(Args&&... args)
		{
			if (s_pInstance.get() == nullptr)
			{
				s_pInstance.reset(new __declspec(thread) T(std::forward<Args>(args)...));
			}

			return s_pInstance.get();
		}
		static T* GetInstance()
		{
			return s_pInstance.get();
		}
	};
}