/*! Range concept implementation.
\author Adam Hlavatovič
\version 20111005 */
#pragma once


namespace loe {

/*! Allows following code
\code
	vector<int> v;
	...
	for (auto r = make_range(v); r; ++r)
		*r = 2*(*r); */
template <class Iterator>
class iterator_range
{
public:
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
	typedef typename Iterator::reference const const_reference;
	typedef Iterator iterator;
	typedef const Iterator const_iterator;

	iterator_range(Iterator first, Iterator last)
		: _cur(first), _end(last)
	{}

	operator bool() const {return _cur != _end;}
	reference operator*() {return *_cur;}
	const_reference operator*() const {return *_cur;}
	iterator_range & operator++() {++_cur; return *this;}
	iterator begin() {return _cur;}
	const_iterator begin() const {return _cur;}
	iterator end() {return _end;}
	const_iterator end() const {return _end;}

private:
	Iterator _cur, _end;
};

//! Creates range from std-like container.
template <class Container>
inline iterator_range<typename Container::iterator> make_range(
	Container & c)
{
	return iterator_range<typename Container::iterator>(c.begin(), c.end());
}

template <class Container>
inline iterator_range<typename Container::const_iterator> make_range(
	const Container & c)
{
	return iterator_range<typename Container::const_iterator>(c.begin(),
		c.end());
}

//! Creates range from two iterators.
template <class Iterator>
inline iterator_range<Iterator> make_range(Iterator first, Iterator last)
{
	return iterator_range<Iterator>(first, last);
}

}  // loe

