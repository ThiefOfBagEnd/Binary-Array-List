#pragma once

#include <vector>

template<typename value_type>
class BinaryVectorList
{
public:
	typedef size_type std::vector<value_type>::size_type;
	typedef iterator std::vector<value_type>::iterator;
	typedef const_iterator std::vector<value_type>::const_iterator;
	typedef reverse_iterator std::vector<value_type>::reverse_iterator;
	typedef const_reverse_iterator std::vector<value_type>::const_reverse_iterator;


	BinaryVectorList()
	{

	}
	~BinaryVectorList()
	{

	}

	BinaryVectorList& operator= (const BinaryVectorList& bvl)
	{
		return this;
	}

	iterator begin()
	{

	}

	const_iterator begin() const
	{

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


	//TODO:Relational Operators
	//TODO:swap two BinaryVectorLists
	//TODO:Decide whether to support emplace
	//After a little reading, emplace will not be too dificult to implement
	//Not Supported data()- memory is not stored in consecutive addresses so it cannot be returned
	//TODO:Decide whether to support data(int) for part of the memory addresses
	//TODO:Decide whether to support the allocator template argument-leaning toward not supporting

protected:
	std::vector<std::vector<value_type> > vectorList;
};

