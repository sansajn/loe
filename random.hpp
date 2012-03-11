/*! \file random.hpp 
\author Adam Hlavatovič
\version 20110915 */
#pragma once
// std
#include <ctime>
#include <cstdlib>


namespace loe {

/*! Random number generator utility.
\note Link with rt library (-lrt).

\code
	vector<int> v(100);
	generate(v.begin(), v.end(), random_generator()); */
class random_generator
{
public:
	random_generator(int max = RAND_MAX);
	int operator()() const {return rand() % _max;}

public:
	int _max;
};


inline random_generator::random_generator(int max)
	: _max(max)
{
	timespec ts;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);  		
	srand(ts.tv_nsec);  
}


}  // loe
