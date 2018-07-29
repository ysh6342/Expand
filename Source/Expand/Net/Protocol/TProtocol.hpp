#pragma once

#include "TProtocolToType.hpp"
#include "../Serialize.hpp"
#include "IPacket.h"
#include "../../Automation.hpp"

#define BIND_PACKET_MEMBERS(...) \
BIND_MEM_FUNC_TIE(get_packet_members, __VA_ARGS__)

struct MSG_HEART_BEAT_CMD
{
	BIND_PACKET_MEMBERS();
};

namespace Expand
{
	namespace Net
	{
		namespace Protocol
		{
			class IPacket;

			class IProtocol
			{
			public:
				virtual ~IProtocol() {}

				virtual short GetType() const = 0;

				virtual short GetCType() const = 0;
				virtual short GetPType() const = 0;

				virtual short GetSize() const = 0;

				virtual bool Serialize(IPacket& packet) = 0;
				virtual bool Deserialize(IPacket& packet) = 0;
			};

			template <class T, auto Category, auto Value>
			class TProtocol 
				: public IProtocol
			{
			public:
				using Type = T;
				static constexpr short value = static_cast<std::uint8_t>(Category) << 8 & static_cast<std::uint8_t>(Value);
				static_assert(!std::is_same_v<T, MSG_HEART_BEAT_CMD> && value == 0);
			public:
				T Data;

				virtual short GetType() const { return value; }

				virtual short GetCType() const override { return HIBYTE(GetType()); }
				virtual short GetPType() const override { return LOBYTE(GetType()); }

				virtual short GetSize() const override
				{
					return 4 + get_packet_size(Data);
				}

				virtual bool Serialize(IPacket& packet) override
				{
					short type = GetType();
					memcpy(packet.GetPtr() + 2, &type, sizeof(short));

					Serializer ss(packet.GetPtr() + 4, packet.GetSize() - 4);
					if (!ss.TSerialize(Data))
						return false;

					short size = static_cast<short>(ss.GetCurPos() + 4);
					if (size > packet.GetSize())
						return false;

					memcpy(packet.GetPtr(), &size, sizeof(short));

					return true;
				}

				virtual bool Deserialize(IPacket& packet) override
				{
					Serializer ss(packet.GetPtr(), packet.GetSize());
					return ss.TDeserialize(Data);
				}

			private:
				ADD_HAS_MEMBER_FUNCTION(get_packet_members);

				template <class T>
				typename std::enable_if<has_mem_func_get_packet_members<T>::value, bool>::type get_packet_size(const T& rData) const
				{
					return get_packet_size(rData.get_packet_members());
				}

				template <class T>
				typename std::enable_if<std::is_arithmetic<T>::value, bool>::type get_packet_size(const T& rData) const
				{
					return sizeof(T);
				}

				template <class T>
				typename std::enable_if<std::is_enum<T>::value, bool>::type get_packet_size(const T& rData) const
				{
					return sizeof(T);
				}

				template <class T>
				typename std::enable_if<is_tuple_type<T>::value, bool>::type get_packet_size(const T& rData) const
				{
					return calc_(rData, std::make_index_sequence<std::tuple_size<T>::value>{});
				}

				template <class T>
				typename std::enable_if<is_string_type<T>::value, bool>::type get_packet_size(const T& rData) const
				{
					return rData.size();
				}

				template<typename Tuple, std::size_t... I>
				auto calc_(const Tuple& data, std::index_sequence<I...>) const
				{
					return f(std::get<I>(data)...);
				}

				template <class T, class... Args>
				static auto f(const T& v, Args&&... args)
				{
					return get_packet_size(v) + f(std::forward<Args>(args)...);
				}

				static auto f()
				{
					return 0;
				}

				template <class T>
				typename std::enable_if<is_array_type<T>::value, bool>::type get_packet_size(const T& rContainer) const
				{
					return calc_(rContainer);
				}

				template<typename T, std::size_t N>
				auto calc_(const std::array<T, N>& data) const
				{
					auto f = [](auto v, auto...)
					{
						return v + ...;
					}
					return calc_(std::make_tuple(data[I]...));
				}
			};
		}
	};
};

using PACKET_HEART_BEAT_CMD = Expand::Net::Protocol::TProtocol<MSG_HEART_BEAT_CMD, 0, 0>;