// std
#include <string>
#include <vector>
#include <iostream>

// dbw
#include <loe/dbw.hpp>


void error_with_exit(const std::string & msg, int code);



int main(int argc, char * argv[])
{
	dbw::session sql("test.db");
	if (!sql.good())
		error_with_exit("Can't establish a database connection!", 1);

	std::vector<int> one;
	std::vector<double> two;
	std::string tree;

	sql << "select one, two, tree from test where one=" << 1, one, two, tree;
	std::cout << one[0] << ", " << two[0] << "; "
		<< one[1] << ", " << two[1] << "\n"
		<< "and " << tree << "\n";

	return 0;
}

void error_with_exit(const std::string & msg, int code)
{
	std::cerr << "error: " << msg << "\n";
	exit(code);
}
