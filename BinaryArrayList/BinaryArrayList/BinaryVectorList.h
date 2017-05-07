/** \file BinaryVectorList.h
* \brief BinaryVectorList Header File
* Last Update: 5/6/17 9:54AM
* Author: Taylor Dowlen
*/

#pragma once

#include <vector>

/** \class BinaryVectorList
* \brief A v-list implementation the array data type/vector STL container.
* It's API matches std::vector almost perfectly.
* It should be asymptotically faster on inserts and deletes, but uses less spacial locality than std::vector
*/

/** \typedef value_type
* \brief The type of elements in the BinaryVectorList container.
*/
template<typename value_type, typename allocator_type = std::allocator<value_type> >
class BinaryVectorList
{
public:
	//typedefs

	/**
	* \brief A reference to value_type
	*/
	typedef reference value_type&;

	/**
	* \brief A const reference to value_type
	*/
	typedef const_reference const value_type&;

	/**
	* \brief A pointer to value_type given by allocator (usually value_type*)
	*/
	typedef pointer std::allocator_traits<allocator_type>::pointer;

	/**
	* \brief A const pointer to value_type given by allocator (usually const value_type*)
	*/
	typedef const_pointer std::allocator_traits<allocator_type>::const_pointer;

	/**
	* \brief An iterator type that can be used to iterate through the elements of a BinaryVectorList.
	* Not sure what type we need here yet. Probably std::vector::iterator, but until implementation is concluded we won't know.
	*/
	typedef iterator std::vector<value_type>::iterator;

	/**
	* \brief An iterator type that can be used to iterate through the elements of a BinaryVectorList, but not change the elements.
	* Not sure what type we need here yet. Probably std::vector::const_iterator, but until implementation is concluded we won't know.
	*/
	typedef const_iterator std::vector<value_type>::const_iterator;

	/**
	* \brief An iterator type that can be used to iterate through the elements of a BinaryVectorList in reverse order.
	* A reverse iterator based on iterator.
	*/
	typedef reverse_iterator std::reverse_iterator<iterator>;

	/**
	* \brief An iterator type that can be used to iterate through the elements of a BinaryVectorList in reverse order, but not change the elements.
	* A const reverse iterator based on const iterator.
	*/
	typedef const_reverse_iterator std::reverse_iterator<const_iterator>;

	/**
	* \brief A signed integer type usually ptrdiff_t.
	*/
	typedef difference_type std::iterator_traits<iterator>::difference_type;

	/**
	* \brief An unsigned integer type that can represent any non-negative value of difference_type, usually size_t.
	*/
	typedef size_type std::iterator_traits<iterator>::size_type;

	//constructors
	/**
	* \brief Empty Container Constructor
	* Constructs an empty container with no elements.
	* \param[in] alloc Allocator to use for the BinaryVectorList.
	*/
	BinaryVectorList(const allocator_type& alloc = allocator_type())
	{
		m_vTvector(alloc);
	}

	/**
	* \brief Fill Constructor
	* Constructs a container with n elements. Each element is a copy of val.
	* \param[in] n		Number of elements to create in the BinaryVectorList container.
	* \param[in] val	The value to copy for the elements of the BinaryVectorList.
	* \param[in] alloc	Allocator to use for the BinaryVectorList.
	*/
	BinaryVectorList(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
	{
		m_vTvector(n, val, alloc);
	}

	/**
	* \brief Range Constructor
	* Constructs a container with as many elements as the range [first,last), 
	* with each element constructed from its corresponding element in that range, in the same order.
	* \param[in] first	Iterator to the first element in the container whose elements are to be copied into the BinaryVectorList.
	* \param[in] last	Iterator to the last element in the container whose elements are to copied into the BinaryVectorList. NOTE: The last element is not copied into the BinaryVectorList.
	* \param[in] alloc	Allocator to use for the BinaryVectorList.
	*/
	template<typename InputIterator>
	BinaryVectorList(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type())
	{
		m_vTvector(first, last, alloc);
	}

	/**
	* \brief Copy Constructor
	* Constructs a container with a copy of each of the elements in bvl, in the same order.
	* \param[in] bvl	BinaryVectorList to copy elements from.
	* \param[in] alloc	Allocator to use for the BinaryVectorList.
	*/
	BinaryVectorList(const BinaryVectorList& bvl, const allocator_type& alloc = allocator_type())
	{
		m_vTvector(bvl.m_vTvector, alloc);
	}

	/**
	* \brief Move Constructor
	* Constructs a container that acquires the elements of bvl.
	* If alloc is specified and is different from bvl's allocator, the elements are moved. 
	* Otherwise, no elements are constructed (their ownership is directly transferred).
	* bvl is left in an unspecified but valid state.
	* \param[in] bvl	BinaryVectorList to acquire elements from.
	* \param[in] alloc	Allocator to use for the BinaryVectorList.
	*/
	BinaryVectorList(BinaryVectorList&& bvl, const allocator_type& alloc = allocator_type())
	{
		m_vTvector(bvl.m_vTvector, alloc);
	}

	/**
	* \brief Initializer List Constructor
	* Constructs a container with a copy of each of the elements in il, in the same order.
	* \param[in] il Initializer List to copy elements from.
	* \param[in] alloc	Allocator to use for the BinaryVectorList.
	*/
	BinaryVectorList(std::initializer_list<value_type> il, const allocator_type& alloc = allocator_type())
	{
		m_vTvector(il, alloc);
	}

	//destructor
	/**
	* \brief Destructor
	* Deallocates all of the memory used by the BinaryVectorList
	*/
	~BinaryVectorList()
	{
		//nothing to do. There is no memory to deallocate.
	}

	//Assignment Pperators

	/**
	* \brief Copy Assignment
	* Copies all the elements from bvl into the container (with bvl preserving its contents)
	* \param[in] bvl BinaryVectorList to copy elements from.
	*/
	BinaryVectorList& operator= (const BinaryVectorList& bvl)
	{
		m_vTvector = bvl.m_vTvector;
		return this;
	}

	/**
	* \brief Move Assignment
	* Moves the elements of bvl into the container (bvl is left in an unspecified but valid state)
	* \param[in] bvl BinaryVectorList to copy elements from.
	*/
	BinaryVectorList& operator= (BinaryVectorList&& bvl)
	{
		m_vTvector = bvl.m_vTvector;
		return this;
	}

	/**
	* \brief Initializer List Assignment
	* Copies the elements of il into the container.
	* \param[in] il Initializer List to copy elements from.
	*/
	BinaryVectorList& operator= (std::initializer_list<value_type> il)
	{
		m_vTvector = bvl.m_vTvector;
		return this;
	}

	//Iterators

	iterator begin()
	{
		return m_vTvector.begin();
	}

	const_iterator begin() const
	{
		return m_vTvector.begin();
	}

	iterator end()
	{

	}

	const_iterator end() const
	{

	}

	reverse_iterator rbegin()
	{

	}

	const_reverse_iterator rbegin() const
	{

	}

	reverse_iterator rend()
	{

	}

	const_reverse_iterator rend() const
	{

	}

	const_iterator cbegin() const noexcept
	{

	}

	const_iterator Cend() const noexcept
	{

	}

	const_reverse_iterator crbegin() const noexcept
	{

	}

	const_reverse_iterator crend() const noexcept
	{

	}

	//Capacity

	size_type size() const
	{

	}

	size_type max_size() const
	{

	}

	void resize(size_type n, value_type val = value_type())
	{

	}

	size_type capacity() const
	{

	}

	bool empty() const
	{

	}

	void reserve(size_type n)
	{

	}

	void shrink_to_fit()
	{

	}

	//Element Access

	value_type& operator[] (size_type n)
	{

	}

	const value_type& operator[] (size_type n) const
	{

	}

	value_type& at(size_type n)
	{

	}

	const value_type& at(size_type n) const
	{

	}

	value_type& front()
	{

	}

	const value_type& front() const
	{

	}

	value_type& back()
	{

	}

	const value_type& back() const
	{

	}

	//Modifiers

	template <typename InputIterator>
	void assign(InputIterator first, InputIterator last)
	{

	}

	void assign(size_type n, const value_type& val)
	{

	}

	void push_back(const value_type& val)
	{

	}

	void push_front(const value_type& val)
	{

	}

	void pop_back()
	{

	}

	void pop_front()
	{

	}

	iterator insert(iterator position, const value_type& val)
	{

	}

	void insert(iterator position, size_type n, const value_type& val)
	{

	}

	template<typename InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last)
	{

	}

	iterator erase(iterator position)
	{

	}

	iterator erase(iterator first, iterator last)
	{

	}

	void swap(BinaryVectorList& x)
	{

	}

	void clear()
	{

	}

	template<typename... Args>
	iterator emplace(const_iterator position, Args&&... args)
	{
		m_vTvector.emplace(position, args);
	}

	template<typename... Args>
	iterator emplace_back(Args&&... args)
	{
		m_vTvector.emplace_back(args);
	}

	//Allocator
	allocator_type get_allocator() const noexcept
	{
		return m_vTvector.get_allocator();
	}

	//Operations - Most of these will be slow since they are taken from STL List, but still worth implementing.

	void splice(iterator position, BinaryVectorList& x)
	{

	}

	void splice(iterator position, BinaryVectorList& x, iterator i)
	{
		
	}

	void splice(iterator position, BinaryVectorList& x, iterator first, iterator last)
	{

	}

	void remove(const value_type& val)
	{

	}

	template <typename Predicate>
	void remove_if(Predicate pred)
	{

	}

	void unique()
	{

	}

	template <typename BinaryPredicate>
	void unique(BinaryPredicate binary_pred)
	{

	}

	void merge(BinaryVectorList& x)
	{

	}

	template <typename Compare>
	void merge(BinaryVectorList&x, Compare comp)
	{

	}

	void sort()
	{

	}

	template <typename Compare>
	void sort(Compare comp)
	{

	}

	void reverse()
	{

	}

protected:
	//std::vector<std::vector<value_type> > m_vvTvectorList;
	std::vector<value_type> m_vTvector;
};

//Swap two BinaryVectorLists
template<typename value_type, typename allocator_type>
void swap(BinaryVectorList<value_type, allocator_type>& bvlLeft, BinaryVectorList<value_type, allocator_type>& bvlRight)
{
	bvlLeft.swap(bvlRight);
}

//Relational Operators
template<typename value_type, typename allocator_type>
bool operator == (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	return true;
}

template<typename value_type, typename allocator_type>
bool operator != (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	return true;
}

template<typename value_type, typename allocator_type>
bool operator < (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	return true;
}

template<typename value_type, typename allocator_type>
bool operator <= (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	return true;
}

template<typename value_type, typename allocator_type>
bool operator > (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	return true;
}

template<typename value_type, typename allocator_type>
bool operator >= (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	return true;
}