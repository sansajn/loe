/*! \file luatools.hpp
\author Adam Hlavatovič 
\version 20120212 */
#pragma once
#include <map>
#include <vector>
#include <string>
#include <lua5.1/lua.hpp>


namespace loe {
	namespace lua {

typedef void (* errout)(char const * msg);

inline lua_State * newstate() {return luaL_newstate();}

//! Lua Virtual Machine.
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


//! Lua stack low-level manipulators.
//@{
template <typename R>
inline R stack_pop(lua_State * L);

template <typename T>
inline void stack_push(lua_State * L, T const & x);

template <typename R>
inline R stack_at(lua_State * L, int idx);

template <> 
inline int stack_pop<int>(lua_State * L)
{
	int tmp = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return tmp;
}

template <> 
inline double stack_pop<double>(lua_State * L)
{
	double tmp = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return tmp;
}

template <> 
inline std::string stack_pop<std::string>(lua_State * L)
{
	std::string tmp = lua_tostring(L, -1);
	lua_pop(L, 1);
	return tmp;
}

template <> 
inline void stack_push<int>(lua_State * L, int const & x)
{
	lua_pushinteger(L, x);
}

template <> 
inline void stack_push<double>(lua_State * L, double const & x)
{
	lua_pushnumber(L, x);
}

template <> 
inline void stack_push<std::string>(lua_State * L, std::string const & x)
{
	lua_pushstring(L, x.c_str());
}

inline void stack_push(lua_State * L, char const * x)
{
	lua_pushstring(L, x);
}

template <>
inline int stack_at<int>(lua_State * L, int idx)
{
	return lua_tointeger(L, idx);
}

template <>
inline double stack_at<double>(lua_State * L, int idx)
{
	return lua_tonumber(L, idx);
}

template <>
inline std::string stack_at<std::string>(lua_State * L, int idx)
{
	return lua_tostring(L, idx);
}

template <typename Value, typename Key>
Value get_table(lua_State * L, Key k, int sidx) 
{
	stack_push(L, k);
	lua_gettable(L, sidx-1);
	return stack_pop<Value>(L);
}
//@}

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

template <typename T>
class map_range
{
public:
	map_range(lua_State * L, int sidx = -1) : _L(L) {
		_sidx = lua_gettop(_L) + (sidx + 1);
		lua_pushnil(_L);
		_ok = lua_next(_L, _sidx);
	}

	~map_range() {lua_pop(_L, 1);}

	void operator++() {
		lua_pop(_L, 1);
		_ok = lua_next(_L, _sidx);
	}

	std::pair<std::string, T> operator*() {
		return std::make_pair(stack_at<std::string>(_L, -2), 
			stack_at<T>(_L, -1));
	}

	operator bool() {return _ok;}

private:
	bool _ok;
	int _sidx;  //!< table stack index
	lua_State * _L;
};

/*! \note Argumenty sú v prúde v opačnom poradí ako ich vracia volaná (lua)
funkcia. */
class istack_stream
{
public:
	typedef istack_stream self;

	istack_stream(lua_State * L, int sidx = -1) : _L(L), _sidx(sidx) {}

	//! unary-manipulators
	self & operator>>(void (*fn)(self & is)) {
		fn(*this);
		return *this;
	}

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

	template <typename T>
	self & operator>>(std::map<std::string, T> & val) {
		for (map_range<T> r = get_map<T>(); r; ++r)
		{
			std::pair<std::string, T> p = *r;
			val[p.first] = p.second;
		}
		return *this;
	}

	void next() {--_sidx;}

	template <typename Value, typename Key>
	Value get_table(Key k) {
		return lua::get_table<Value>(_L, k, _sidx);
	}
	
private:
	template <typename T>
	array_range<T> get_array() {
		return array_range<T>(_L, _sidx--);
	}

	template <typename T>
	map_range<T> get_map() {
		return map_range<T>(_L, _sidx--);
	}

private:
	lua_State * _L;
	int _sidx;	//!< stack index
};

/*! \note Tabuľkový manipulátor implicitne neposunie ukazateľ na ďalší prvok v
zásobníku. Ak chceme zo zásobníku čítať dalšie prvky, je potrebné zavolať
manipulátor next(). */
template <typename Key, typename Value>
inline std::pair<Key, Value &> tab(Key k, Value & v)
{
	return std::pair<Key, Value &>(k, v);
}

/* \note Parameter f sa nemusí predavať ako referencia, lebo kompilator použije
optimalizáciu a celý kódu sa inlajnuje. */
template <typename Value, typename Key>
inline istack_stream & operator>>(istack_stream & is, std::pair<Key, Value &> f)
{
	f.second = is.get_table<Value>(f.first);
	return is;
}

template <typename Value, typename Key>
inline istack_stream & operator>>(istack_stream && is, std::pair<Key, Value &> f)
{
	f.second = is.get_table<Value>(f.first);
	return is;
}

inline void next(istack_stream & is) {is.next();}


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

