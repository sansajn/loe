// boost
#define BOOST_TEST_MODULE heap test
#include <boost/test/included/unit_test.hpp>
#include "../unique_ptr.hpp"
using loe::unique_ptr;

struct foo 
{
	int v;
};

BOOST_AUTO_TEST_SUITE(suite_unique_ptr_tests)

BOOST_AUTO_TEST_CASE(test_construct)
{
	unique_ptr<int> u;
	unique_ptr<double> v(new double(10.10));
	unique_ptr<foo> s;
}

BOOST_AUTO_TEST_CASE(test_dereference)
{
	unique_ptr<int> u(new int(101));
	BOOST_CHECK_EQUAL(*u, 101);

	unique_ptr<foo> f(new foo);
	f->v = 102;
	BOOST_CHECK_EQUAL(f->v, 102);
}

BOOST_AUTO_TEST_CASE(test_bool)
{
	unique_ptr<int> u;
	BOOST_CHECK(!u);

	unique_ptr<int> v(new int(101));
	BOOST_CHECK(v);
}

BOOST_AUTO_TEST_CASE(test_reset)
{
	unique_ptr<int> u(new int(101));
	u.reset(new int(102));
	BOOST_CHECK_EQUAL(*u, 102);
}

BOOST_AUTO_TEST_CASE(test_swap)
{
	unique_ptr<int> u(new int(101)), v(new int(102));
	u.swap(v);
	BOOST_CHECK_EQUAL(*u, 102);
	BOOST_CHECK_EQUAL(*v, 101);
}

BOOST_AUTO_TEST_CASE(test_release)
{
	unique_ptr<int> u(new int(101));
	int * p = u.release();
	BOOST_CHECK(!u);
	BOOST_CHECK_EQUAL(*p, 101);
	delete p;
}

BOOST_AUTO_TEST_CASE(test_size)
{
	unique_ptr<int> u(new int(101));
	BOOST_CHECK_EQUAL(sizeof(u), sizeof(int *));
}

/*
BOOST_AUTO_TEST_CASE(test_construct)
{
	BOOST_CHECK(1);
	BOOST_CHECK_EQUAL(1, 1);
}
*/

BOOST_AUTO_TEST_SUITE_END()

