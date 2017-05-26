/** \file BinaryVectorList.h
* \brief BinaryVectorList Header File
* \date 5/7/17
* \version 0.0.1
* \author Taylor Dowlen
* \copyright GNU General Public License v3.0
* \warning THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <vector>

/**
* \brief A v-list implementation the array abstract data structure.
* It's API is a combination of those of std::vector and std::list with a few exceptions.
* Anything asymptotically slower than linear with respect to container size from the std::list API is not provided.
* This is a way from keeping users from doing stupid (slow) things, and blaming the data structure.
* std::vector.data() is not provided because it wholly is dependent on the underlying implementation of std::vector.
* The goal here is to use an implementation free interface to develop a new implementation for the array abstract data structure, 
* that is the implementation details can vary from each individual implementor, but the API/interface remain the same. 
* data() doesn't fit that goal, it assumes that std::vector is implemented as 1 continuous block of memory,
* which this implementation will intentionally break.
* BinaryVectorList can be used exactly like std::vector (except for std::vector.data()) 
* including having constant time Random Access Iterators, leveraging some spacial locality of elements, 
* but it should be asymptotically faster on inserts at the end than std::vector.
* The downside is that it uses less spacial locality than std::vector
* by using several different sized blocks of continuous memory instead of just 1.
* \tparam allocator_type The type of allocator to use in the BinaryVectorList container.
* \tparam value_type The type of elements in the BinaryVectorList container.
*/
template<typename value_type, typename allocator_type = std::allocator<value_type> >
class BinaryVectorList
{
public:
	//Typedefs

	/**
	* A reference to value_type
	*/
	typedef value_type& reference;

	/**
	* A const reference to value_type
	*/
	typedef const reference const_reference;

	/**
	* A pointer to value_type given by allocator (usually value_type*)
	*/
	typedef std::allocator_traits<allocator_type>::pointer pointer;

	/**
	* A const pointer to value_type given by allocator (usually const value_type*)
	*/
	typedef std::allocator_traits<allocator_type>::const_pointer const_pointer;

	template<typename iterator_type>
	class BinaryVectorListIterator : public std::iterator<std::random_access_iterator_tag, iterator_type>
	{
	public:
		BinaryVectorListIterator() {}
		BinaryVectorListIterator(iterator_type rhs) : iter(rhs) {}
		BinaryVectorListIterator(const BinaryVectorListIterator &rhs) : iter(rhs.iter) {}
		/* inline Iterator& operator=(Type* rhs) {_ptr = rhs; return *this;} */
		/* inline Iterator& operator=(const Iterator &rhs) {_ptr = rhs._ptr; return *this;} */
		inline BinaryVectorListIterator& operator+=(difference_type rhs) { _ptr += rhs; return *this; }
		inline BinaryVectorListIterator& operator-=(difference_type rhs) { _ptr -= rhs; return *this; }
		inline Type& operator*() const { return *_ptr; }
		inline Type* operator->() const { return _ptr; }
		inline Type& operator[](difference_type rhs) const { return _ptr[rhs]; }

		inline BinaryVectorListIterator& operator++() { ++_ptr; return *this; }
		inline BinaryVectorListIterator& operator--() { --_ptr; return *this; }
		inline BinaryVectorListIterator operator++(int) const { Iterator tmp(*this); ++_ptr; return tmp; }
		inline BinaryVectorListIterator operator--(int) const { Iterator tmp(*this); --_ptr; return tmp; }
		/* inline Iterator operator+(const Iterator& rhs) {return Iterator(_ptr+rhs.ptr);} */
		inline difference_type operator-(const BinaryVectorListIterator& rhs) const { return Iterator(_ptr - rhs.ptr); }
		inline BinaryVectorListIterator operator+(difference_type rhs) const { return Iterator(_ptr + rhs); }
		inline BinaryVectorListIterator operator-(difference_type rhs) const { return Iterator(_ptr - rhs); }
		friend inline BinaryVectorListIterator operator+(difference_type lhs, const BinaryVectorListIterator& rhs) { return Iterator(lhs + rhs._ptr); }
		friend inline BinaryVectorListIterator operator-(difference_type lhs, const BinaryVectorListIterator& rhs) { return Iterator(lhs - rhs._ptr); }

		inline bool operator==(const BinaryVectorListIterator& rhs) const { return _ptr == rhs._ptr; }
		inline bool operator!=(const BinaryVectorListIterator& rhs) const { return _ptr != rhs._ptr; }
		inline bool operator>(const BinaryVectorListIterator& rhs) const { return _ptr > rhs._ptr; }
		inline bool operator<(const BinaryVectorListIterator& rhs) const { return _ptr < rhs._ptr; }
		inline bool operator>=(const BinaryVectorListIterator& rhs) const { return _ptr >= rhs._ptr; }
		inline bool operator<=(const BinaryVectorListIterator& rhs) const { return _ptr <= rhs._ptr; }
	private:
		iterator_type iter;
	};

	/**
	* An iterator type that can be used to iterate through the elements of a BinaryVectorList.
	*/
	//Not sure what type we need here yet. Probably std::vector::iterator, but until implementation is concluded we won't know.
	typedef std::vector<value_type>::iterator iterator;

	/**
	* An iterator type that can be used to iterate through the elements of a BinaryVectorList, but not change the elements. 
	*/
	// Not sure what type we need here yet.Probably std::vector::const_iterator, but until implementation is concluded we won't know.
	typedef std::vector<value_type>::const_iterator const_iterator;

	/**
	* An iterator type that can be used to iterate through the elements of a BinaryVectorList in reverse order.
	*/
	typedef std::reverse_iterator<iterator> reverse_iterator;

	/**
	* An iterator type that can be used to iterate through the elements of a BinaryVectorList in reverse order, but not change the elements.
	*/
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	/**
	* A signed integer type, usually ptrdiff_t.
	*/
	typedef std::iterator_traits<iterator>::difference_type difference_type;

	/**
	* An unsigned integer type that can represent any non-negative value of difference_type, usually size_t.
	*/
	typedef std::iterator_traits<iterator>::size_type size_type;

	//Constructors

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
	* \tparam InputIterator The iterator type to a container whose elements are to be copied to the BinaryVectorList
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
	* \param[in] il		Initializer List to copy elements from.
	* \param[in] alloc	Allocator to use for the BinaryVectorList.
	*/
	BinaryVectorList(std::initializer_list<value_type> il, const allocator_type& alloc = allocator_type())
	{
		m_vTvector(il, alloc);
	}

	//Destructor

	/**
	* \brief Destructor
	* Deallocates all of the memory used by the BinaryVectorList
	*/
	~BinaryVectorList()
	{
		//nothing to do. There is no memory to deallocate.
	}

	//Assignment Operators

	/**
	* \brief Copy Assignment
	* Copies all the elements from bvl into the container (with bvl preserving its contents)
	* \param[in] bvl BinaryVectorList to copy elements from.
	* \return Reference to this (BinaryVectorList). This allows for function chaining.
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
	* \return Reference to this (BinaryVectorList). This allows for function chaining.
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
	* \return Reference to this (BinaryVectorList). This allows for function chaining.
	*/
	BinaryVectorList& operator= (std::initializer_list<value_type> il)
	{
		m_vTvector = bvl.m_vTvector;
		return this;
	}

	//Iterators

	/**
	* \brief Return iterator to beginning
	* Returns an iterator pointing to the first element in the BinaryVectorList.
	* \return An iterator pointing to the first element of the BinaryVectorList
	*/
	iterator begin() noexcept
	{
		return m_vTvector.begin();
	}

	/**
	* \brief Return const iterator to beginning
	* Returns a const_iterator pointing to the first element in the BinaryVectorList.
	* \return A const_iterator pointing to the first element of the BinaryVectorList
	*/
	const_iterator begin() const noexcept
	{
		return m_vTvector.begin();
	}

	/**
	* \brief Return iterator to end
	* Returns an iterator pointing to the past-the-end element in the BinaryVectorList.
	* \return An iterator pointing to the past-the-end element of the BinaryVectorList
	*/
	iterator end() noexcept
	{
		return m_vTvector.end();
	}

	/**
	* \brief Return const iterator to end
	* Returns a const_iterator pointing to the past-the-end element in the BinaryVectorList.
	* \return A const_iterator pointing to the past-the-end element of the BinaryVectorList
	*/
	const_iterator end() const noexcept
	{
		return m_vTvector.end();
	}

	/**
	* \brief Return reverse iterator to the reverse beginning
	* Returns a reverse_iterator pointing to the last element in the BinaryVectorList.
	* \return A reverse_iterator pointing to the last element of the BinaryVectorList
	*/
	reverse_iterator rbegin() noexcept
	{
		return m_vTvector.rbegin();
	}

	/**
	* \brief Return const reverse iterator to the reverse beginning
	* Returns a const_reverse_iterator pointing to the last element in the BinaryVectorList.
	* \return A const_reverse_iterator pointing to the last element of the BinaryVectorList
	*/
	const_reverse_iterator rbegin() const noexcept
	{
		return m_vTvector.rbegin();
	}

	/**
	* \brief Return reverse iterator to the reverse end
	* Returns a reverse_iterator pointing to the theoretical element preceding the first element in the BinaryVectorList.
	* \return A reverse_iterator pointing to the theoretical element preceding the first element of the BinaryVectorList
	*/
	reverse_iterator rend() noexcept
	{
		return m_vTvector.rend();
	}

	/**
	* \brief Return const reverse iterator to the reverse end
	* Returns a const_reverse_iterator pointing to the theoretical element preceding the first element in the BinaryVectorList.
	* \return A const_reverse_iterator pointing to the theoretical element preceding the first element of the BinaryVectorList
	*/
	const_reverse_iterator rend() const noexcept
	{
		return m_vTvector.rend();
	}

	/**
	* \brief Return const_iterator to beginning
	* Returns a const_iterator pointing to the first element in the BinaryVectorList.
	* \return A const_iterator pointing to the first element of the BinaryVectorList
	*/
	const_iterator cbegin() const noexcept
	{
		return m_vTvector.cbegin();
	}

	/**
	* \brief Return const_iterator to end
	* Returns a const_iterator pointing to the past-the-end element in the BinaryVectorList.
	* \return A const_iterator pointing to the past-the-end element of the BinaryVectorList
	*/
	const_iterator cend() const noexcept
	{
		return m_vTvector.cend();
	}

	/**
	* \brief Return const_reverse_iterator to the reverse beginning
	* Returns a const_reverse_iterator pointing to the last element in the BinaryVectorList.
	* \return A const_reverse_iterator pointing to the last element of the BinaryVectorList
	*/
	const_reverse_iterator crbegin() const noexcept
	{
		return m_vTvector.crbegin();
	}

	/**
	* \brief Return const_reverse_iterator to the reverse end
	* Returns a const_reverse_iterator pointing to the theoretical element preceding the first element in the BinaryVectorList.
	* \return A const_reverse_iterator pointing to the theoretical element preceding the first element of the BinaryVectorList
	*/
	const_reverse_iterator crend() const noexcept
	{
		return m_vTvector.crend();
	}

	//Capacity

	/**
	* \brief Return size of BinaryVectorList
	* Returns the number of elements in the BinaryVectorList.
	* \return The number of elements in the BinaryVectorList.
	*/
	size_type size() const noexcept
	{
		return m_vTvector.size();
	}

	/**
	* \brief Return maximum size of BinaryVectorList
	* Returns the maximum number of elements the BinaryVectorList can hold.
	* \return The maximum number of elements the BinaryVectorList can hold.
	*/
	size_type max_size() const noexcept
	{
		return m_vTvector.max_size();
	}

	/**
	* \brief Change size
	* Resizes the BinaryVectorList so that it contains n elements.
	* \param[in] n		Number of elements to resize the BinaryVectorList to.
	* \param[in] val	Value to fill added elements with if n is greater than the current BinaryVectorList size.
	*/
	void resize(size_type n, const value_type val = value_type())
	{
		m_vTvector.resize(n, val);
	}

	/**
	* \brief Return size of currently allocated storage capacity
	* Returns the size of the storage space currently allocated for the BinaryVectorElement, expressed in terms of value_type elements.
	* \return The size of the storage space currently allocated for the BinaryVectorElement, expressed in terms of value_type elements.
	*/
	size_type capacity() const noexcept
	{
		return m_vTvector.capacity();
	}

	/**
	* \brief Test whether the BinaryVectorList is empty
	* Returns whether the BinaryVectorList's size is 0.
	* \return Whether the size is 0
	*/
	bool empty() const noexcept
	{
		return m_vTvector.empty();
	}

	/**
	* \brief Request a change in capacity
	* Requests that the BinaryVectorList capacity be enough to contain at least n elements.
	* \param[in] n Minimum number of elements the BinaryVectorList should contain
	*/
	void reserve(size_type n)
	{
		m_vTvector.reserve(n);
	}

	/**
	* \brief Request a down-size of capacity
	* Requests the BinaryVectorList shrink to a capacity that matches it current size
	*/
	void shrink_to_fit()
	{
		m_vTvector.shrink_to_fit();
	}

	//Element Access

	/**
	* \brief Access element
	* Returns a reference to the element at position n in the BinaryVectorList.
	* \param[in] n Position of the desired element.
	* \return A reference to the element at position n
	*/
	reference operator[] (size_type n)
	{
		return m_vTvector[n];
	}

	/**
	* \brief Access element
	* Returns a const_reference to the element at position n in the BinaryVectorList.
	* \param[in] n position of the desired element.
	* \return A const_reference to the element at position n
	*/
	const_reference operator[] (size_type n) const
	{
		return m_vTvector[n];
	}

	/**
	* \brief Access element
	* Returns a reference to the element at position n in the BinaryVectorList.
	* \param[in] n Position of the desired element.
	* \return A reference to the element at position n
	*/
	reference at(size_type n)
	{
		return m_vTvector.at(n);
	}

	/**
	* \brief Access element
	* Returns a const_reference to the element at position n in the BinaryVectorList.
	* \param[in] n Position of the desired element.
	* \return A const_reference to the element at position n
	*/
	const_reference at(size_type n) const
	{
		return m_vTvector.at(n);
	}

	/**
	* \brief Access first element
	* Returns a reference to the first element in the BinaryVectorList.
	* \return A reference to the first element in the BinaryVectorList.
	*/
	reference front()
	{
		return m_vTvector.front();
	}

	/**
	* \brief Access first element
	* Returns a const_reference to the first element in the BinaryVectorList.
	* \return A const_reference to the first element in the BinaryVectorList.
	*/
	const_reference front() const
	{
		return m_vTvector.front();
	}

	/**
	* \brief Access last element
	* Returns a reference to the last element in the BinaryVectorList.
	* \return A reference to the last element in the BinaryVectorList.
	*/
	reference back()
	{
		return m_vTvector.back();
	}

	/**
	* \brief Access last element
	* Returns a const_reference to the last element in the BinaryVectorList.
	* \return A const_reference to the last element in the BinaryVectorList.
	*/
	const_reference back() const
	{
		return m_vTvector.back();
	}

	//Modifiers

	/**
	* \brief Assign BinaryVectorList content from another container
	* Assigns new content to the BinaryVectorList, replacing its current contents with copies from the elements in [first,last), and modifying it's size accordingly.
	* \tparam InputIterator The iterator type to a container whose elements are to be copied to the BinaryVectorList
	* \param[in] first	InputIterator to the initial position in the container to be copied from
	* \param[in] last	InputIterator to the final position in the container to be copied from
	*/
	template <typename InputIterator>
	void assign(InputIterator first, InputIterator last)
	{
		m_vTvector.assign(first, last);
	}

	/**
	* \brief Assign BinaryVectorList content with copies of a value
	* Assigns new content to the BinaryVectorList, replacing its current contents with n copies of val, and modifying it's size accordingly.
	* \param[in] n		New size for the BinaryVectorList
	* \param[in] val	Value to fill the container with
	*/
	void assign(size_type n, const value_type& val)
	{
		m_vTvector.assign(n, val);
	}

	/**
	* \brief Assign BinaryVectorList content from an initializer_list
	* Assigns new content to the BinaryVectorList, replacing its current contents with the values from the initializer_list, and modifying it's size accordingly.
	* \param[in] il Initializer List to copy elements from.
	*/
	void assign(std::initializer_list<value_type> il)
	{
		m_vTvector.assign(il);
	}

	/**
	* \brief Add an element at the end of the BinaryVectorList
	* Adds a new element at the end of the BinaryVectorList. The content of val is copied to the new element
	* \param[in] val Value to be copied to the new element
	*/
	void push_back(const value_type& val)
	{
		m_vTvector.push_back(val);
	}

	/**
	* \brief Add an element at the end of the BinaryVectorList
	* Adds a new element at the end of the BinaryVectorList. The content of val is moved to the new element
	* \param[in] val Value to be moved to the new element
	*/
	void push_back(value_type&& val)
	{
		m_vTvector.push_back(val);
	}

	/**
	* \brief Add an element at the beginning of the BinaryVectorList
	* Adds a new element at the beginning of the BinaryVectorList. The content of val is copied to the new element
	* \param[in] val Value to be copied to the new element
	*/
	void push_front(const value_type& val)
	{
		m_vTvector.insert(m_vTvector.begin(), val);
	}

	/**
	* \brief Add an element at the beginning of the BinaryVectorList
	* Adds a new element at the beginning of the BinaryVectorList. The content of val is moved to the new element
	* \param[in] val Value to be moved to the new element
	*/
	void push_front(value_type&& val)
	{
		m_vTvector.insert(m_vTvector.begin(), val);
	}

	/**
	* \brief Deletes the last element of the BinaryVectorList
	* Removes the last element in the BinaryVectorList, effectively reducing the container size by 1.
	*/
	void pop_back()
	{
		m_vTvector.pop_back();
	}

	/**
	* \brief Deletes the first element of the BinaryVectorList
	* Removes the first element in the BinaryVectorList, effectively reducing the container size by 1.
	*/
	void pop_front()
	{
		m_vTvector.erase(m_vTvector.begin());
	}

	/**
	* \brief Insert a copy of 1 element
	* The BinaryVectorList is extended by inserting a copy of val before the element at the specified position.
	* \param[in] position	The position where the new elements are to be inserted
	* \param[in] val		The value be copied
	* \return An iterator that points to the newly inserted element
	*/
	iterator insert(const_iterator position, const value_type& val)
	{
		return m_vTvector.insert(position, val);
	}

	/**
	* \brief Insert n copies of 1 element
	* The BinaryVectorList is extended by inserting n copies of val before the element at the specified position.
	* \param[in] position	The position where the new elements are to be inserted
	* \param[in] n			The number of elements to insert
	* \param[in] val		The value be copied
	* \return An iterator that points to the first newly inserted element
	*/
	iterator insert(const_iterator position, size_type n, const value_type& val)
	{
		return m_vTvector.insert(position, n, val);
	}

	/**
	* \brief Insert elements from an InputIterator
	* The BinaryVectorList is extended by inserting copies of elements from the range [first, last) before the element at the specified position.
	* \tparam InputIterator The iterator type to a container whose elements are to be copied to the BinaryVectorList
	* \param[in] position	The position where the new elements are to be inserted
	* \param[in] first		InputIterator specifying the initial position in a container to copy from
	* \param[in] last		InputIterator specifying the final position in a container to copy from
	* \return An iterator that points to the first newly inserted element
	*/
	template<typename InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last)
	{
		return m_vTvector.insert(position, first, last);
	}

	/**
	* \brief Insert a moved element
	* The BinaryVectorList is extended by inserting val before the element at the specified position. val is moved from it's old container
	* \param[in] position	The position where the new elements are to be inserted
	* \param[in] val		The value be moved
	* \return An iterator that points to the newly inserted element
	*/
	iterator insert(const_iterator position, value_type&& val)
	{
		return m_vTvector.insert(position, val);
	}

	/**
	* \brief Insert elements from an initializer_list
	* The BinaryVectorList is extended by inserting copies of elements from the initializer_list before the element at the specified position.
	* \param[in] position	The position where the new elements are to be inserted
	* \param[in] il			Initializer_list to copy from
	* \return An iterator that points to the newly inserted element
	*/
	iterator insert(const_iterator position, std::initializer_list<value_type> il)
	{
		return m_vTvector.insert(position, il);
	}

	/**
	* \brief Erase an element
	* Removes a single element from the binary
	* \param[in] position The position of the element to be remove
	* \return An iterator that points to the new location of the element that was after the erased element.
	*/
	iterator erase(const_iterator position)
	{
		return m_vTvector.erase(position);
	}

	/**
	* \brief Erase a range of elements
	* Removes the elements in the range [first,last)
	* \param[in] first	The position of the first element to remove
	* \param[in] last	The position after the last element to remove
	* \return An iterator that points to the new location of the element that was after the last erased element.
	*/
	iterator erase(const_iterator first, iterator last)
	{
		return m_vTvector.erase(first, last);
	}

	/**
	* \brief Swap BinaryVectorList content
	* Exchanges the content of the BinaryVectorList by the content of another BinaryVectorList, bvl.
	* \param[in] bvl The BinaryVectorList to swap contents with
	*/
	void swap(BinaryVectorList& bvl)
	{
		m_vTvector.swap(bvl.m_vTvector);
	}

	/**
	* \brief Empty BinaryVectorList contents
	* Removes all elements from the BinaryVectorList, leaving the container with a size of 0.
	*/
	void clear() noexcept
	{
		m_vTvector.clear();
	}

	/**
	* \brief Construct and insert an element
	* The container is extended by inserting a new element constructed in place (using args as the arguments for its constructor) at position.
	* \tparam Args The arguments to the constructor for the element being emplaced
	* \param[in] position	The position to add the new element
	* \param[in] args		The arguments to the constructor to build the new element
	* \return An iterator that points to the new element
	*/
	template<typename... Args>
	iterator emplace(const_iterator position, Args&&... args)
	{
		return m_vTvector.emplace(position, args);
	}

	/**
	* \brief Construct and insert an element at the end of the BinaryVector list
	* The container is extended by inserting a new element constructed in place (using args as the arguments for its constructor) at the end of the BinaryVectorList.
	* \tparam Args The arguments to the constructor for the element being emplaced
	* \param[in] args		The arguments to the constructor to build the new element
	* \return An iterator that points to the new element
	*/
	template<typename... Args>
	void emplace_back(Args&&... args)
	{
		m_vTvector.emplace_back(args);
	}

	//Allocator

	/**
	* \brief Get allocator object
	* Returns a copy of the allocator object associated with the BinaryVectorList.
	* \return A copy of the allocator object associated with the BinaryVectorList.
	*/
	allocator_type get_allocator() const noexcept
	{
		return m_vTvector.get_allocator();
	}

	//Operations - Most of these will be slow since they are taken from STL List, but still worth implementing.
	/*
	* Not implementing these on the first pass. There's no vector implementation for these.

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
	*/

protected:
	//std::vector<std::vector<value_type> > m_vvTvectorList;
	std::vector<value_type> m_vTvector;
};

//Swap two BinaryVectorLists

/**
* \brief Exchange content of vectors
* The contents of bvlLeft are exchanged for those of bvlRight
* \tparam value_type The type of elements in the BinaryVectorLists.
* \tparam allocator_type The type of allocator used in the BinaryVectorLists.
*/
template<typename value_type, typename allocator_type>
void swap(BinaryVectorList<value_type, allocator_type>& bvlLeft, BinaryVectorList<value_type, allocator_type>& bvlRight)
{
	bvlLeft.swap(bvlRight);
}

//Relational Operators

/**
* \brief Equality comparison
* Compares sizes and if they match the elements are compared sequentially using operator ==, stopping at the first mismatch
* \tparam value_type The type of elements in the BinaryVectorLists.
* \tparam allocator_type The type of allocator used in the BinaryVectorLists.
*/
template<typename value_type, typename allocator_type>
bool operator == (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	bool bResult = false;
	if (lhs.size() == rhs.size())
	{
		bResult = true;
		BinaryVectorList<value_type, allocator_type>::size_type size = lhs.size();
		size_t i = 0;
		for (i = 0; i < size; ++i)
		{
			if (lhs[i] == rhs[i])
			{
				continue;
			}
			else
			{
				bResult = false;
				break;
			}
		}
	}
	return bResult;
}

/**
* \brief Inequality comparison
* Negation of the equality comparison
* \tparam value_type The type of elements in the BinaryVectorLists.
* \tparam allocator_type The type of allocator used in the BinaryVectorLists.
*/
template<typename value_type, typename allocator_type>
bool operator != (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	return !(lhs == rhs);
}

/**
* \brief Lexicographical less than comparison
* The elements are compared sequentially using operator < (a<b and b<a) and stopping at the first occurrence of inequality.
* If a (lhs) < b (rhs) for some a and b, then lhs < rhs.
* If b (rhs) < a (lhs) for some a and b,then rhs < lhs.
* If sequential comparisons are equal, then the BinaryVectorList with the smallest size is said to be less than the other.
* \tparam value_type The type of elements in the BinaryVectorLists.
* \tparam allocator_type The type of allocator used in the BinaryVectorLists.
*/
template<typename value_type, typename allocator_type>
bool operator < (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	bool bResult = false;
	BinaryVectorList<value_type, allocator_type>::size_type lhsize = lhs.size();
	BinaryVectorList<value_type, allocator_type>::size_type rhsize = rhs.size();
	size_t i = 0;
	for (i = 0; (i < lhsize) && (i < rhsize); ++i)
	{
		if (lhs[i] < rhs[i])
		{
			//lhs < rhs
			bResult = true;
			break;
		}
		if (rhs[i] < lhs[i])
		{
			//rhs < lhs
			bResult = false;
			break;
		}
	}
	if ((i == lhsize) || (i == rhsize))
	{
		//we made it all the way through one of the lists with "equality"
		bResult = lhssize < rhsize;
	}
	return bResult;
}

/**
* \brief Less than or equal lexicographical comparison
* Negation of the greater than comparison
* \tparam value_type The type of elements in the BinaryVectorLists.
* \tparam allocator_type The type of allocator used in the BinaryVectorLists.
*/
template<typename value_type, typename allocator_type>
bool operator <= (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	return !(rhs < lhs)
}

/**
* \brief Greater than lexicographical comparison
* Less than comparison with the arguments swapped
* \tparam value_type The type of elements in the BinaryVectorLists.
* \tparam allocator_type The type of allocator used in the BinaryVectorLists.
*/
template<typename value_type, typename allocator_type>
bool operator > (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	return rhs < lhs;
}

/**
* \brief Greater than or equal lexicographical comparison
* Negation of the less than comparison
* \tparam value_type The type of elements in the BinaryVectorLists.
* \tparam allocator_type The type of allocator used in the BinaryVectorLists.
*/
template<typename value_type, typename allocator_type>
bool operator >= (const BinaryVectorList<value_type, allocator_type>& lhs, const BinaryVectorList<value_type, allocator_type>& rhs)
{
	return !(lhs < rhs);
}