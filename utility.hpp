/*! \file utility.hpp
\author Adam Hlavatovič
\version 20110831 */
#pragma once

// std
#include <utility>
#include <string>
#include <sstream>


namespace loe {

	namespace detail {

template <typename T, typename U>
struct tier
{
	T & first;
	U & second;

	tier(T & f, U & s)
		: first(f), second(s)
	{}

	template <typename R, typename S>
	tier<T, U> & operator=(std::pair<R, S> const & p)
	{
		first = p.first;
		second = p.second;
		return *this;
	}
};

}  // detail


/*! Allows following code
\code
	int a, b;
	tie(a, b) = std::make_pair(1, 2);
	... */
template <typename T, typename U>
inline detail::tier<T, U> tie(T & a, U & b)
{
	return detail::tier<T, U>(a, b);
}


template <class Range>
std::string join(Range r, std::string const & sep = " ")
{
	std::ostringstream oss;
	while (r)
	{
		oss << *r;
		if (++r)
			oss << sep;
	}
	return oss.str();
}


}  // loe

