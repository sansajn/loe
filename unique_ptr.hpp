/*! \file unique_ptr.hpp
Smart pointer unique_ptr c++98 implementation. */
#pragma once

#include <utility>

namespace loe {

template <typename T>
struct default_delete
{
	void operator()(T * p) const {delete p;}
};


//! unique_ptr c++98 implementation
template <typename T, typename D = default_delete<T> >
class unique_ptr
{
public:
	typedef T * pointer;
	typedef T element_type;
	typedef D deleter_type;

	explicit unique_ptr(pointer p = 0) {_p.pointee = p;}
	~unique_ptr();

	pointer operator->() const {return _p.pointee;}
	T & operator*() const {return *_p.pointee;}
	pointer get() const {return _p.pointee;}
	deleter_type & get_deleter() {return deleter();}
	deleter_type const & get_deleter() const {return deleter();}
	explicit operator bool() const {return _p.pointee;}
	pointer release();
	void reset(pointer p = pointer()) {unique_ptr(p).swap(*this);}
	void swap(unique_ptr & u) {std::swap(_p, u._p);}

private:
	struct holder : D  // use the "empty member" c++ optimization
	{
		T * pointee;
	};

	D & deleter() {return _p;}
	D const & deleter() const {return _p;}

	unique_ptr(unique_ptr const &);
	unique_ptr & operator=(unique_ptr const &);

	holder _p;
};

template <typename T, typename D>
unique_ptr<T, D>::~unique_ptr()
{
	if (_p.pointee)
		deleter()(_p.pointee);
	_p.pointee = 0;
}

template <typename T, typename D>
typename unique_ptr<T, D>::pointer unique_ptr<T, D>::release()
{
	pointer p = _p.pointee;
	_p.pointee = 0;
	return p;
}

};  // loe

