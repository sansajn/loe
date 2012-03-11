/*! \file session.h
Vysokourovnovy wrapper pre sqlite3 databazu umoznujuci pokladat dotazy
nasledujucim sposobom

\code
	session sql("test.db");
	vector<int> years;
	vector<string> names;
	sql << "select year, name from test", years, names;
	...
\endcode

\note Syntax inspirovana skvelou kniznicou soci.

\version 20110822
\author Adam Hlavatovic */
#pragma once

// std
#include <string>
#include <vector>
#include <sstream>
// sqltie3
#include <sqlite3.h>
// dbw
#include "into_type.hpp"


namespace dbw
{


class session
{
private:
	class once
	{
	public:
		once(session & sess)
			: _sess(&sess)
		{}

		~once() {_sess->ask();}

		template <typename T>
		once & operator,(T & t)
		{
			_sess->_intos.push_back(new standard_into_type<T>(t));
			return *this;
		}

		template <typename T>
		once & operator<<(const T & q)
		{
			_sess->_q << q;
			return *this;
		}

	private:
		session * _sess;
	};

public:
	session(const std::string & dbname);
	~session();

	template <typename T>
	once operator<<(const T & q)
	{
		_q << q;
		return once(*this);
	}

	bool good() const {return _isok;}

private:
	void ask();
	void reset();
	void fetch_row(sqlite3_stmt * stmt);
	void fetch_column(sqlite3_stmt * stmt, int col);

private:
	bool _isok;
	sqlite3 * _db;
	std::ostringstream _q;
	std::vector<base_into_type *> _intos;
};


}  // dbw
