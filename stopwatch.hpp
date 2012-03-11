/*! Time measure.
\author Adam Hlavatovič
\version 20110924 */
#pragma once
// linux
#include <ctime>


namespace loe {

//! Time measure.
/*! \note Link with rt library (-lrt). */
class stopwatch
{
public:
	void start();
	long stop() const;  //!< Returns elapsed time in ns.

private:
	long _start;
};


inline void stopwatch::start()
{
	timespec ts;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
	_start = ts.tv_nsec;
}

inline long stopwatch::stop() const
{
	timespec ts;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
	return ts.tv_nsec - _start;
}

}  // loe

