/*! v20120212 
\author Adam Hlavatovič */
#include <iostream>
using std::cout;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <algorithm>
using std::copy;
#include <iterator>
using std::ostream_iterator;
#include <loe/luatools.hpp>
namespace lua = loe::lua;


void lmessage(char const * msg)
{
	fprintf(stderr, "%s\n", msg);
	fflush(stderr);
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

	lua_pop(L, 2);
}

void test_arrayr(lua::vm & lvm, lua_State * L)
{
	lvm.call_function(L, "dummy_table", 0);
	lua::array_range<int> r(L, -1);
	for (r; r; ++r)
		cout << *r << " ";
	cout << "\n";

	lua_pop(L, 1);
}

void test_arrayis(lua::vm & lvm, lua_State * L)
{
	lvm.call_function(L, "dummy_table", 0);
	vector<int> v;
	lua::istack_stream(L) >> v;
	copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
	cout << "\n";

	lua_pop(L, 1);
}

void test_arrayos(lua::vm & lvm, lua_State * L)
{
	vector<double> x{0.12, 2.34, 6.98, 0.02};
	lua::ostack_stream(L) << x;
	lvm.call_function(L, "filter", 1);

	vector<double> y;
	lua::istack_stream(L) >> y;

	cout << "x = ";
	copy(x.begin(), x.end(), ostream_iterator<double>(cout, " "));
	cout << "\ny = ";
	copy(y.begin(), y.end(), ostream_iterator<double>(cout, " "));
	cout << "\n";

	lua_pop(L, 1);
}

void test_error(lua::vm & lvm, lua_State * L)
{
	lvm.call_function(L, "simple_error", 0);
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

	cout << "stack size: " << lua_gettop(L) << "\n";
}

int main(int argc, char * argv[])
{
	lua::vm lvm(lmessage);
	lua_State * L = lua::newstate();
	lvm.init(L);
	lvm.run_script(L, "test.lua");

	test();

	return 0;
}

