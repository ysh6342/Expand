#pragma once

#include "Includes.hpp"
#include <functional>

namespace Expand
{
	namespace Net
	{
		class ICompletor;

		class OVERLAPPED_EX
			: public OVERLAPPED
		{
			ICompletor* m_pCompletor;

		public:
			OVERLAPPED_EX(ICompletor* pCompletor);
			void OnCompleted(DWORD dwErrorCode, DWORD dwBytesTransferred);
		};

		class ICompletor
		{
		public:
			virtual ~ICompletor() = 0 {}
			virtual void OnCompleted(DWORD dwErrorCode, DWORD dwBytesTransferred) = 0;

			virtual OVERLAPPED* GetOverlappedPtr() = 0;
		};

		class Completor
			: public ICompletor
		{
			OVERLAPPED_EX m_overlapped;

			std::function<void(DWORD, DWORD)> m_func;
		public:
			template <class T>
			Completor(T&& func) : m_overlapped(this), m_func(std::forward<T>(func)) {}
			virtual void OnCompleted(DWORD dwErrorCode, DWORD dwBytesTransferred) override
			{
				m_func(dwErrorCode, dwBytesTransferred);

				delete this;
			}

			virtual OVERLAPPED* GetOverlappedPtr() override { return &m_overlapped; }
		};
	}
}
