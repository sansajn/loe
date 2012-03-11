/*! \file luatools.hpp
\author Adam Hlavatovič 
\version 20120212 */
#pragma once
#include <string>
#include <vector>
#include <lua5.1/lua.hpp>


namespace loe {
	namespace lua {

typedef void (* errout)(char const * msg);

inline lua_State * newstate() {return luaL_newstate();}

class vm
{
public:
	vm(errout luaerr) : _luaerr(luaerr) {}
	void init(lua_State * L);
	void register_function(lua_State * L, lua_CFunction, char const * lname);

	//! \note Stack: argn, ..., arg1, <chunk-name>
	int call_function(lua_State * L, char const * lname, int narg);
	int run_script(lua_State * L, char const * fname);

private:
	int report(lua_State * L, int state);
	int call_chunk(lua_State * L, int narg);

private:
	errout _luaerr;
};

template <typename R>
R stack_pop(lua_State * L);

template <> inline int stack_pop<int>(lua_State * L)
{
	int tmp = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return tmp;
}

template <> inline double stack_pop<double>(lua_State * L)
{
	double tmp = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return tmp;
}

template <> inline std::string stack_pop<std::string>(lua_State * L)
{
	std::string tmp = lua_tostring(L, -1);
	lua_pop(L, 1);
	return tmp;
}

template <typename T>
class array_range
{
public:
	array_range(lua_State * L, int sidx) : _L(L), _sidx(sidx), _tidx(1) {}
	void operator++() {++_tidx;}

	T operator*() {
		lua_rawgeti(_L, _sidx, _tidx);
		return stack_pop<T>(_L);
	}

	operator bool() {return _tidx <= lua_objlen(_L, _sidx);}

private:
	lua_State * _L;
	int _tidx;  //!< table index
	int const _sidx;  //!< stack index
};


/*! \note Argumenty sú v prúde v opačnom poradí. */
class istack_stream
{
public:
	typedef istack_stream self;

	istack_stream(lua_State * L) : _L(L), _sidx(-1) {}

	self & operator>>(int & val) {
		val = lua_tointeger(_L, _sidx--);
		return *this;
	}

	self & operator>>(double & val) {
		val = lua_tonumber(_L, _sidx--);
		return *this;
	}

	self & operator>>(std::string & val) {
		val = lua_tostring(_L, _sidx--);
		return *this;
	}

	template <typename T>
	self & operator>>(std::vector<T> & val) {
		for (array_range<T> r = get_array<T>(); r; ++r)
			val.push_back(*r);
		return *this;
	}

private:
	template <typename T>
	array_range<T> get_array() {
		return array_range<T>(_L, _sidx--);
	}

private:
	lua_State * _L;
	int _sidx;	//!< stack index
};

template <typename T>
inline void stack_push(lua_State * L, T const & x);

template <> inline void stack_push<int>(lua_State * L, int const & x)
{
	lua_pushinteger(L, x);
}

template <> inline void stack_push<double>(lua_State * L, double const & x)
{
	lua_pushnumber(L, x);
}

template <> 
inline void stack_push<std::string>(lua_State * L, std::string const & x)
{
	lua_pushstring(L, x.c_str());
}

class ostack_stream
{
public:
	typedef ostack_stream self;

	ostack_stream(lua_State * L) : _L(L) {}

	self & operator<<(int const & val) {
		lua_pushinteger(_L, val);
		return *this;
	}

	self & operator<<(double const & val) {
		lua_pushnumber(_L, val);
		return *this;
	}

	self & operator<<(std::string const & val) {
		lua_pushstring(_L, val.c_str());
		return *this;
	}

	template <typename T>
	self & operator<<(std::vector<T> const & val) {
		lua_newtable(_L);
		for (int i = 0; i < val.size(); ++i) {
			stack_push(_L, val[i]);
			lua_rawseti(_L, -2, i+1);
		}
		return *this;
	}

private:
	lua_State * _L;
};

	};  // lua
};  // loe

