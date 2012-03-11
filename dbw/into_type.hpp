#pragma once

// sqlite3
#include <sqlite3.h>
// dbw
#include "into_fetch.hpp"


namespace dbw
{
	namespace detail
	{

typedef void * any;

template <typename To, typename From>
To & ref_cast(From * f)
{
	return *(To *)(f);
};

	}  // detail


class base_into_type
{
public:
	base_into_type(detail::any data)
		: _data(data)
	{}

	virtual ~base_into_type() {}
	virtual void fetch(sqlite3_stmt * stmt, int col) = 0;

protected:
	detail::any _data;
};

template <typename T>
class standard_into_type
	: public base_into_type
{
public:
	standard_into_type(T & t)
		: base_into_type(reinterpret_cast<void *>(&t))
	{}

	void fetch(sqlite3_stmt * stmt, int col)
	{
		into_fetch<T>(stmt, col, detail::ref_cast<T>(_data));
	}
};


}  // dbw
