/*----------------------------------------------------------------------------+
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
+----------------------------------------------------------------------------*/
/* ------------------------------------------------------------------
class itl::set
a general set class that extends stl-sets
for concepts InplaceAddable and InplaceSubtractable
--------------------------------------------------------------------*/
#ifndef __itl_set_h_JOFA_070519__
#define __itl_set_h_JOFA_070519__

#include <string>
#include <set>
#include <itl/ctxreprbase.hpp>
#include <itl/set_algo.hpp>
#include <itl/ctxpred.hpp>


namespace itl
{
	//JODO 1_0_1 documentation
	/// an stl based set implementing inplace addition and subtraction operators += and -=
	/** 

	@author Joachim Faulhaber
	*/
	template 
	<
		typename KeyT, 
		template<class>class Compare = std::less,
		template<class>class Alloc   = std::allocator 
	>
	class set: private std::set<KeyT, Compare<KeyT>, Alloc<KeyT> >
	{
	public:
		typedef typename itl::set<KeyT, Compare,       Alloc >       type;
		typedef typename std::set<KeyT, Compare<KeyT>, Alloc<KeyT> > base_type;

	public:
		typedef KeyT     key_type;
		typedef KeyT     value_type;
		typedef KeyT     data_type;
		typedef Compare<KeyT> key_compare;
		typedef Compare<KeyT> value_compare;
		typedef Alloc<KeyT>   allocator_type;

	public:		
		typedef typename base_type::pointer                pointer;
		typedef typename base_type::const_pointer          const_pointer;
		typedef typename base_type::reference              reference;
		typedef typename base_type::const_reference        const_reference;
		typedef typename base_type::iterator               iterator;
		typedef typename base_type::const_iterator         const_iterator;
		typedef typename base_type::size_type              size_type;
		typedef typename base_type::difference_type        difference_type;
		typedef typename base_type::reverse_iterator       reverse_iterator;
		typedef typename base_type::const_reverse_iterator const_reverse_iterator;

	public:
		set(){}
		set(const Compare<KeyT>& comp): 
			std::set<KeyT, Compare<KeyT>, Alloc<KeyT> >(comp){}

		template <class InputIterator>
		set(InputIterator f, InputIterator l): std::set<InputIterator>(f,l) {}

		template <class InputIterator>
		set(InputIterator f, InputIterator l, const key_compare& comp): std::set<InputIterator>(f,l,comp) {}

		set(const set& src): base_type::set(src){}

		set& operator=(const set& src) { base_type::operator=(src); return *this; } 
		void swap(set& src) { base_type::swap(src); }

		using base_type::begin;
		using base_type::end;
		using base_type::rbegin;
		using base_type::rend;

		using base_type::size;
		using base_type::max_size;
		using base_type::empty;

		using base_type::key_comp;
		using base_type::value_comp;

		using base_type::insert;
		using base_type::erase;
		using base_type::clear;
		using base_type::find;
		using base_type::count;

		using base_type::lower_bound;
		using base_type::upper_bound;
		using base_type::equal_range;

	public:
		// --------------------------------------------------------------------
		// itl specific extensions

		//JODO concept set
		/// Checks if the element \c x is in the set
		bool contains(const KeyT& x)const { return !(find(x) == end()); }

		/** Is <tt>*this</tt> contained in <tt>super</tt>? */
		bool contained_in(const set& super)const { return Set::contained_in(*this, super); }

		/** Does <tt>*this</tt> contain <tt>sub</tt>? */
		bool contains(const set& sub)const { return Set::contained_in(sub, *this); }

		/** <tt>*this</tt> and <tt>x2</tt> are disjoint, if their intersection is empty */
		bool disjoint(const set& x2)const { return disjoint(*this, x2); }

		// Default inject-function using += on CodomTV
		// iterator inject(const value_type& vp);

		// Default subtract-function using -= on CodomTV
		iterator subtract(const value_type& vp);

		//JODO concept InplaceAddable
		/// Add a set \c x2 to this set.
		set& operator += (const set& x2);

		//JODO concept InplaceSubtractable
		/// Subtract a set \c x2 from this set.
		set& operator -= (const set& x2) { Set::subtract(*this, x2); return *this; }

		/** Intersect set \c x2 \c *this.
		So \c *this becomes the intersection of \c *this and \c x2 */
		set& operator *= (const set& x) { Set::intersect(*this, x); return *this; }

		//JODO concept StringRepresentable
		/** Represent this set as a string */
		std::string asString(const char* sep = " ")const;

		/** Keep the elements in *this set to which property \c hasProperty applies. 
		Erase all the rest. */
		set& keep_if(const PropertyT<value_type>& hasProperty);

		/** Erase the elements in *this set to which property \c hasProperty applies. 
		Keep all the rest. */
		set& drop_if(const PropertyT<value_type>& hasProperty);

		/** Copy the elements in set \c src to which property \c hasProperty applies 
		into \c *this set. */
		set& copy_if(const PropertyT<value_type>& hasProperty, const set& src);

		template<typename IteratorT>
		static const key_type& key_value(IteratorT& value_){ return (*value_); }

		template<typename IteratorT>
		static const data_type& data_value(IteratorT& value_){ return (*value_); }

		template<typename LeftIterT, typename RightIterT>
		static bool key_less(LeftIterT& lhs_, RightIterT& rhs_) { return key_compare()(*lhs_, *rhs_); }

		static value_type make_element(const key_type& key_val, const data_type& data_val)
		{ return key_val; }

		//JODO: semantics implementation clash size/element_count
		size_t element_count()const { return size(); }
	};


	template <typename KeyT, template<class>class Compare, template<class>class Alloc>
	inline bool operator == (const itl::set<KeyT,Compare,Alloc>& lhs,
		                     const itl::set<KeyT,Compare,Alloc>& rhs)
	{
		typedef std::set<KeyT,Compare<KeyT>,Alloc<KeyT> > base_type;
		return operator==((const base_type&)lhs, (const base_type&)rhs);
	}

	template <typename KeyT, template<class>class Compare, template<class>class Alloc>
	inline bool is_element_equal(const itl::set<KeyT,Compare,Alloc>& lhs,
		                         const itl::set<KeyT,Compare,Alloc>& rhs)
	{
		typedef std::set<KeyT,Compare<KeyT>,Alloc<KeyT> > base_type;
		return operator==((const base_type&)lhs, (const base_type&)rhs);
	}

	template <typename KeyT, template<class>class Compare, template<class>class Alloc>
	inline bool operator < (const itl::set<KeyT,Compare,Alloc>& lhs,
		const itl::set<KeyT,Compare,Alloc>& rhs)
	{
		typedef std::set<KeyT,Compare<KeyT>,Alloc<KeyT> > base_type;
		return operator<((const base_type&)lhs, (const base_type&)rhs);
	}

	template <typename KeyT, template<class>class Compare, template<class>class Alloc>
	inline bool operator <= (const itl::set<KeyT,Compare,Alloc>& lhs,
		const itl::set<KeyT,Compare,Alloc>& rhs)
	{
		typedef std::set<KeyT,Compare<KeyT>,Alloc<KeyT> > base_type;
		return operator<=((const base_type&)lhs, (const base_type&)rhs);
	}


	template <typename KeyT, template<class>class Compare, template<class>class Alloc>
	typename set<KeyT,Compare,Alloc>::iterator
		set<KeyT,Compare,Alloc>::subtract(const value_type& val)
	{
		iterator it_ = find(val);
		if(it_ != end())
			erase(it_);

		return end();
	}


	template <typename KeyT, template<class>class Compare, template<class>class Alloc>
	set<KeyT,Compare,Alloc>& 
		set<KeyT,Compare,Alloc>::operator += (const set<KeyT,Compare,Alloc>& x2)
	{
		// Union with onself stays the same
		if(this == &x2)
			return *this;

		const_iterator i = x2.begin(); 
		while(i != x2.end()) 
			insert(*i++); 

		return *this; 
	}

	template <typename KeyT, template<class>class Compare, template<class>class Alloc>
	std::string set<KeyT,Compare,Alloc>::asString(const char* sep)const
	{ 
		const_iterator it_ = begin();
		
		if(it_ == end()) return std::string();
		else
		{
			std::string y = ReprBaseT<KeyT>::toString(*it_++);
			while(it_ != end()) { y += sep; y += ReprBaseT<KeyT>::toString(*it_++); }
			return y;
		}
	}


	template <typename KeyT, template<class>class Compare, template<class>class Alloc>
	set<KeyT,Compare,Alloc>& set<KeyT,Compare,Alloc>
		::drop_if(const PropertyT<value_type>& hasProperty)
	{
		iterator it = begin(), victim;
		while(it != end())
			if ( hasProperty(*it) ) { victim = it++; erase(victim); } else ++it;
		return *this;
	}

	template <typename KeyT, template<class>class Compare, template<class>class Alloc>
	set<KeyT,Compare,Alloc>& set<KeyT,Compare,Alloc>
		::keep_if(const PropertyT<value_type>& hasProperty)
	{
		iterator it = begin(), victim;
		while(it != end())
			if ( !hasProperty(*it) ) { victim = it++; erase(victim); } else ++it;
		return *this;
	}

	template <typename KeyT, template<class>class Compare, template<class>class Alloc>
	set<KeyT,Compare,Alloc>& set<KeyT,Compare,Alloc>
		::copy_if(const PropertyT<value_type>& hasProperty, const set<KeyT,Compare,Alloc>& src)
	{
		if(this == &src) return keep_if(hasProperty);
		// otherwise
		clear();
		const_iterator it = src.begin();
		while(it != src.end()) {
			if ( hasProperty(*it) ) insert(*it); it++;
		}
		return *this;
	}

} // namespace itl

#endif // __itl_set_h_JOFA_070519__

