#include "session.hpp"


namespace dbw {

void session::fetch_row(sqlite3_stmt * stmt)
{
	for (size_t i = 0; i < _intos.size(); ++i)
		fetch_column(stmt, i);
}

void session::fetch_column(sqlite3_stmt * stmt, int col)
{
	_intos[col]->fetch(stmt, col);
}

void session::ask()
{
	// priprav prikaz
	sqlite3_stmt * stmt;
	std::string q(_q.str());
	int rc = sqlite3_prepare_v2(_db, q.c_str(), -1, &stmt, NULL);

	if (rc == SQLITE_OK)
	{
		while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
			fetch_row(stmt);
	}

	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	_isok = (rc == SQLITE_DONE);
}

void session::reset()
{
	for (auto & x : _intos)
		delete x;
	_intos.clear();
	_q.clear();
	_q.str(std::string());
}

session::session(const std::string & dbname)
	: _db(nullptr)
{
	_isok = !sqlite3_open(dbname.c_str(), &_db);
}

session::~session()
{
	if (_db)
		sqlite3_close(_db);
	_db = nullptr;
}

}  // dbw
