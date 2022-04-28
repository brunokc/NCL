#pragma once

template<class E>
constexpr E operator&(const E& a, const E& b)
{
	return static_cast<E>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
}

template<class E>
constexpr E& operator|=(E& a, const E& b)
{
	a = static_cast<E>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
	return a;
}

template<class E>
constexpr std::enable_if_t<std::is_enum_v<E>, bool> operator!(E a)
{
	return a == static_cast<E>(0);
}

struct Enum
{
	template<class E>
	static constexpr bool FlagSet(E a, E v)
	{
		return ((a & v) == v);
	}
};
