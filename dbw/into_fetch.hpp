#pragma once

// std
#include <string>
#include <vector>
#include <cassert>
// sqlite3
#include <sqlite3.h>


namespace dbw
{

template <typename T>
void into_fetch(sqlite3_stmt * stmt, int col, T & data)
{
	assert(0);  // not ment to be call
}

template <>
inline void into_fetch<int>(sqlite3_stmt * stmt, int col, int & val)
{
	val = sqlite3_column_int(stmt, col);
}

template <>
inline void into_fetch<double>(sqlite3_stmt * stmt, int col, double & val)
{
	val = sqlite3_column_double(stmt, col);
}

template <>
inline void into_fetch<std::string>(sqlite3_stmt * stmt, int col,
	std::string & val)
{
	val = std::string(reinterpret_cast<const char *>(
		sqlite3_column_text(stmt, col)));
}

template <>
inline void into_fetch<std::vector<int>>(sqlite3_stmt * stmt, int col,
	std::vector<int> & data)
{
	data.push_back(sqlite3_column_int(stmt, col));
}

template <>
inline void into_fetch<std::vector<double>>(sqlite3_stmt * stmt, int col,
	std::vector<double> & data)
{
	data.push_back(sqlite3_column_double(stmt, col));
}

template <>
inline void into_fetch<std::vector<std::string>>(sqlite3_stmt * stmt, int col,
	std::vector<std::string> & data)
{
	data.push_back(reinterpret_cast<const char *>(
		sqlite3_column_text(stmt, col)));
}

}  // dbw

