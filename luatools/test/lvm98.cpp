/*! \author Adam Hlavatovič */
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cassert>
#include <loe/luatools.hpp>

using std::string;
using std::vector;
using std::map;
using std::copy;
using std::cout;
using std::ostream_iterator;
namespace lua = loe::lua;

void lmessage(char const * msg);

void test();
void test_test(lua::vm & lvm, lua_State * L);
void test_person(lua::vm & lvm, lua_State * L);
void test_arrayr(lua::vm & lvm, lua_State * L);
void test_arrayis(lua::vm & lvm, lua_State * L);
void test_arrayos(lua::vm & lvm, lua_State * L);
void test_ostream_table(lua::vm & lvm, lua_State * L);
void test_error(lua::vm & lvm, lua_State * L);
void test_libs(lua::vm & lvm, lua_State * L);
void test_io(lua::vm & lvm, lua_State * L);
void test_luasql(lua::vm & lvm, lua_State * L);

// do skriptu posle uzivatelsky definovanu strukturu
void test_custom_structure(lua::vm & lvm, lua_State * L);
void test_custom_structure_overload(lua::vm & lvm, lua_State * L);

// do skriptu posle pole uzivatelsky definovanej strukturi
void test_custom_structure_array(lua::vm & lvm, lua_State * L);


int main(int argc, char * argv[])
{
	test();

	return 0;
}

void test()
{
	lua::vm lvm(lmessage);
	lua_State * L = lua::newstate();
	lvm.init(L);
	lvm.run_script(L, "test.lua");

	test_test(lvm, L);
	test_person(lvm, L);
	test_arrayr(lvm, L);
	test_arrayis(lvm, L);
	test_arrayos(lvm, L);
	test_error(lvm, L);
	test_libs(lvm, L);
	test_ostream_table(lvm, L);
	test_custom_structure(lvm, L);
	test_custom_structure_overload(lvm, L);
	test_custom_structure_array(lvm, L);

	cout << "stack size: " << lua_gettop(L) << "\n";

	lua_close(L);
}


void test_test(lua::vm & lvm, lua_State * L)
{
	lvm.call_function(L, "test", 0);

	string name;
	int age;
	double weigth;

	lua::istack_stream(L) >> weigth >> age >> name;

	cout << "result:\n" 
		<< "  name  : " << name << "\n"
		<< "  age   : " << age << "\n"
		<< "  weigth: " << weigth << "\n";

	assert(strcmp("Frenk", name.c_str()) == 0 && age == 22 
		&& weigth == 87.5 && "unexpected return values");

	lua_pop(L, 3);	
}

void test_person(lua::vm & lvm, lua_State * L)
{
	lua::ostack_stream(L) << "Frenk Black";
	lvm.call_function(L, "person_data", 1);

	int salary, age;
	lua::istack_stream(L) >> salary >> age;

	cout << "Frenk Black:\n" 
		<< "  age   : " << age << "\n"
		<< "  salary: " << salary << "\n";

	assert(age == 25 && salary == 1890 && "unexpected return values");

	lua_pop(L, 2);
}

void test_arrayr(lua::vm & lvm, lua_State * L)
{
	lvm.call_function(L, "dummy_table", 0);
	for (lua::array_range<int> r(L, -1); r; ++r)
		cout << *r << " ";
	cout << "\n";

	int expected_data[] = {1, 2, 3, 2, 1};

	lua::array_range<int> r(L, -1);

	bool failed = false;
	for (int i = 0; r; ++r, ++i)
	{
		cout << *r << " ";
		if (*r != expected_data[i])
			failed = true;
	}
	cout << "\n";

	assert(!failed && "returned array not match");

	lua_pop(L, 1);
}

void test_arrayis(lua::vm & lvm, lua_State * L)
{
	lvm.call_function(L, "dummy_table", 0);
	vector<int> v;
	lua::istack_stream(L) >> v;
	copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
	cout << "\n";

	int data[] = {1, 2, 3, 2, 1};
	vector<int> expected_data(data, data+5);

	assert(v == expected_data && "returned array not match");

	lua_pop(L, 1);
}

void test_arrayos(lua::vm & lvm, lua_State * L)
{
	double data[] = {0.12, 2.34, 6.98, 0.02};
	vector<double> x(data, data+4);
	lua::ostack_stream(L) << x;
	lvm.call_function(L, "filter", 1);

	vector<double> y;
	lua::istack_stream(L) >> y;

	cout << "x = ";
	copy(x.begin(), x.end(), ostream_iterator<double>(cout, " "));
	cout << "\ny = ";
	copy(y.begin(), y.end(), ostream_iterator<double>(cout, " "));
	cout << "\n";

	vector<double> z;
	for (vector<double>::const_iterator it = x.begin(); it != x.end(); ++it) 
		z.push_back((*it)*(*it));

	assert(z == y && "returned array not match");

	lua_pop(L, 1);
}

void test_ostream_table(lua::vm & lvm, lua_State * L)
{
	int i = 1, j = 2, k = 3;

/*	
	lua::ostack_stream los(L);
	los << lua::newtable << lua::tab("one", i) 
		<< lua::tab("two", j) << lua::tab("tree", k);
*/		

	lua::ostack_stream(L) << lua::newtable << lua::tab("one", i) 
		<< lua::tab("two", j) << lua::tab("tree", k);

	lvm.call_function(L, "echo_table", 1);
	
	map<string, int> received;
	lua::istack_stream(L) >> received;

	assert(received.size() == 3 
		&& "unexpected number of return values");

	assert(received["one"] == 1 && received["two"] == 2 
		&& received["tree"] == 3 && "returned map not match");

	lua_pop(L, 1);
}

void test_error(lua::vm & lvm, lua_State * L)
{
	lvm.call_function(L, "simple_error", 0);
}

void test_io(lua::vm & lvm, lua_State * L)
{
	lvm.call_function(L, "write_test", 0);
}

void test_luasql(lua::vm & lvm, lua_State * L)
{
	lvm.call_function(L, "sqlite3_test", 0);
}

void test_libs(lua::vm & lvm, lua_State * L)
{
	test_io(lvm, L);
	test_luasql(lvm, L);
}


struct person
{
	string name;
	int age;
};

void test_custom_structure(lua::vm & lvm, lua_State * L)
{
	person p = {"Franta Spelec", 29};

	lua::ostack_stream(L) << lua::newtable << lua::tab("name", p.name)
		<< lua::tab("age", p.age);

	lvm.call_function(L, "custom_structure_test", 1);

	int result = 0;
	lua::istack_stream(L) >> result;

	assert(result && "custom-structure-test failed");

	lua_pop(L, 1);
}

namespace loe {
	namespace lua {

template <>
inline void stack_push<person>(lua_State * L, person const & p)
{
	ostack_stream(L) << newtable << tab("name", p.name) << tab("age", p.age);
}

	}  // lua
}  // loe

void test_custom_structure_overload(lua::vm & lvm, lua_State * L)
{
	person p = {"Franta Spelec", 29};

	lua::ostack_stream(L) << p;

	lvm.call_function(L, "custom_structure_test", 1);

	int result = 0;
	lua::istack_stream(L) >> result;

	assert(result && "custom-structure-test failed");

	lua_pop(L, 1);
}

void test_custom_structure_array(lua::vm & lvm, lua_State * L)
{
	person p[] = {
		{"Franta Spelec", 29},
		{"John Rambo", 33},
		{"Janosikov sveter", 26}
	};

	vector<person> arr(p, p+3);

	lua::ostack_stream(L) << arr;
	lvm.call_function(L, "custom_structure_array_test", 1);

	int result = 0;
	lua::istack_stream(L) >> result;

	assert(result && "custom-structure-array-test failed");

	lua_pop(L, 1);
}

void lmessage(char const * msg)
{
	fprintf(stderr, "%s\n", msg);
	fflush(stderr);
}

