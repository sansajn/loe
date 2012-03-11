/*! \file linked.hpp
Linked structures implementations.

Queue and stack structures don't own elements stored inside and
don't use any dynamic memory allocation. Each element must implement
node concept.

{Node concept}
This expressions must allways be valid

	n.next

\example

	struct node {
		node * next;
	};

\author Adam Hlavatovič
\version 20120220 */
namespace loe {

template <class Node>
class linked_stack 
{
public:
	linked_stack() : _head(nullptr) {}
	Node * pop();
	void push(Node * n);
	bool empty() const {return !_head;}

private:
	Node * _head;
};

template <typename Node>
class linked_queue 
{
public:
	linked_queue() {_head = _tail = nullptr;}
	void push(Node * n);		
	Node * pop();
	bool empty() const {return !_head;}

private:
	Node * _head;
	Node * _tail;
};

/*! Expects that sizeof(T) >= sizeof(ptrdiff_t). */
template <typename T>
class linked_pool
{
public:
	linked_pool() {}
	~linked_pool();
	T * alloc();
	void free(T *& elem);

private:
	struct node {
		node * next;
	};

	linked_stack<node> _block;	
};

template <class Node>
void linked_queue<Node>::push(Node * n) 
{
	if (_head)
		_tail->next = n;
	else
		_head = n;  //!< first time
	_tail = n;
	_tail->next = nullptr;
}
	
template <class Node>
Node * linked_queue<Node>::pop() 
{
	Node * n = _head;
	_head = n->next;
	return n;
}

template <class Node>
void linked_stack<Node>::push(Node * n)
{
	n->next = _head;
	_head = n;
}

template <class Node>
Node * linked_stack<Node>::pop()
{
	Node * n = _head;
	_head = _head->next;
	return n;
}
template <typename T>
linked_pool<T>::~linked_pool()
{
	while (!_block.empty())
		delete reinterpret_cast<T *>(_block.pop());
}

template <typename T>
T * linked_pool<T>::alloc()
{
	if (!_block.empty())
		return reinterpret_cast<T *>(_block.pop());
	else
		return new T;
}

template <typename T>
void linked_pool<T>::free(T *& elem)
{
	_block.push(reinterpret_cast<node *>(elem));
	elem = nullptr;
}


}  // loe
