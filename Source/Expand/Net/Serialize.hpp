#pragma once

#include "../typetraitsex.hpp"
#include <boost/noncopyable.hpp>

namespace Expand
{
	namespace Net
	{
		class Serializer
			: private boost::noncopyable
		{
			ADD_HAS_MEMBER_FUNCTION(get_packet_members);

			char* m_pBuffer;
			std::size_t m_curPos;
			std::size_t m_maxSize;
		public:
			Serializer(char* pBuffer, std::size_t maxSize)
				: m_pBuffer(pBuffer)
				, m_curPos(0)
				, m_maxSize(maxSize)
			{

			}

			size_t GetCurPos() const { return m_curPos; }

			// Serialize
			template <class T>
			typename std::enable_if<has_mem_func_get_packet_members<T>::value, bool>::type TSerialize(const T& rData)
			{
				return TSerialize(rData.get_packet_members());
			}

			template <class T>
			typename std::enable_if<std::is_arithmetic<T>::value, bool>::type TSerialize(const T& rData)
			{
				return Write(&rData, sizeof(T));
			}

			template <class T>
			typename std::enable_if<std::is_enum<T>::value, bool>::type TSerialize(const T& rData)
			{
				return Write(&rData, sizeof(T));
			}

			template <class T>
			typename std::enable_if<is_tuple_type<T>::value, bool>::type TSerialize(const T& rData)
			{
				return Expand::ForEach(rData, [this](const auto& data)
				{
					return TSerialize(data);
				}, true);
			}

			template <class T>
			typename std::enable_if<is_string_type<T>::value, bool>::type TSerialize(const T& rData)
			{
				if (!TSerialize(int(rData.size())))
					return false;
				if (rData.size() == 0)
					return true;

				return Write(rData.data(), rData.size() * sizeof(T::value_type));
			}

			template <class T>
			typename std::enable_if<is_array_type<T>::value, bool>::type TSerialize(const T& rContainer)
			{
				for (const auto& value : rContainer)
				{
					if (!TSerialize(value))
						return false;
				}

				return true;
			}

			template <class T>
			typename std::enable_if<is_element_container<T>::value, bool>::type TSerialize(const T& rContainer)
			{
				if (!TSerialize(int(rContainer.size())))
					return false;

				for (const auto& value : rContainer)
				{
					if (!TSerialize(value))
						return false;
				}
			}

			template <class T>
			typename std::enable_if<is_pair_container<T>::value, bool>::type TSerialize(const T& rContainer)
			{
				if (!TSerialize(int(rContainer.size())))
					return false;

				for (const auto& pair : rContainer)
				{
					if (!TSerialize(pair.first))
						return false;
					if (!TSerialize(pair.second))
						return false;
				}

				return true;
			}

			// Deserialize
			template <class T>
			typename std::enable_if<has_mem_func_get_packet_members<T>::value, bool>::type TDeserialize(const T& rData)
			{
				return TDeserialize(rData.get_packet_members());
			}

			template <class T>
			typename std::enable_if<std::is_arithmetic<T>::value, bool>::type TDeserialize(T& rData)
			{
				return Read(&rData, sizeof(T));
			}

			template <class T>
			typename std::enable_if<std::is_enum<T>::value, bool>::type TDeserialize(T& rData)
			{
				return Read(&rData, sizeof(T));
			}

			template <class T>
			typename std::enable_if<is_tuple_type<T>::value, bool>::type TDeserialize(T& rData)
			{
				return Expand::ForEach(rData, [this](auto& data)
				{
					return TDeserialize(data);
				}, true);
			}

			template <class T>
			typename std::enable_if<is_string_type<T>::value, bool>::type TDeserialize(T& rData)
			{
				int length = 0;
				if (!TDeserialize(length))
					return false;
				if (length == 0)
					return true;

				rData.resize(length);

				return Read((void*)rData.c_str(), rData.size() * sizeof(T::value_type));
			}

			template <class T>
			typename std::enable_if<is_array_type<T>::value, bool>::type TDeserialize(T& rContainer)
			{
				for (size_t i = 0; i < rContainer.max_size(); ++i)
				{
					if (!TDeserialize(rContainer[i]))
						return false;
				}

				return true;
			}

			template <class T>
			typename std::enable_if<is_element_container<T>::value, bool>::type TDeserialize(T& rContainer)
			{
				int size = 0;
				if (!TDeserialize(size))
					return false;

				for (int i = 0; i < size; ++i)
				{
					T::value_type data;
					if (!TDeserialize(data))
						return false;

					rContainer.emplace_back(data);
				}

				return true;
			}

			template <class T>
			typename std::enable_if<is_pair_container<T>::value, bool>::type TDeserialize(T& rContainer)
			{
				int size = 0;
				if (!TDeserialize(size))
					return false;

				for (int i = 0; i < size; ++i)
				{
					T::key_type key;
					T::value_type data;
					if (!TDeserialize(key))
						return false;
					if (!TDeserialize(data))
						return false;

					rContainer.try_emplace(key, data);
				}

				return true;
			}

		private:
			bool Write(const void* pData, int size)
			{
				if (m_curPos + size > m_maxSize)
					return false;

				memcpy(m_pBuffer + m_curPos, pData, size);
				m_curPos += size;

				return true;
			}
			bool Read(void* pData, int size)
			{
				if (m_curPos + size > m_maxSize)
					return false;

				memcpy(pData, m_pBuffer + m_curPos, size);
				m_curPos += size;

				return true;
			}
		};
	}
}