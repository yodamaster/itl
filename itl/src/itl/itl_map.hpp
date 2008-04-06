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
class itl::map
	a general map class that extends stl-maps
	for concepts InplaceAddable and InplaceSubtractable
--------------------------------------------------------------------*/
#ifndef __itl_map_h_JOFA_070519__
#define __itl_map_h_JOFA_070519__

#include <string>
#include <itl/notate.hpp>
#include <itl/ctxreprbase.hpp>
#include <itl/ctxpred.hpp>
#include <itl/itl_set.hpp>
#include <itl/map_algo.hpp>
#include <map>


namespace itl
{
	/*JODO move this comment to concept InplaceAddable, InplaceSubtractable, InplaceCombinable

		Many features of the itl are based on the concept, that instances of
		template parameter are InplaceAddable(InplaceSubtractible). That is, 
		operators += and -= are implemented.
	*/

	/// an stl based map implementing inplace addition and subtraction operators += and -=
	//JODO 1_0_1 documentation
	/** 

		@author Joachim Faulhaber
	*/
	template 
	<
		typename KeyT, 
		typename DataT, 
		template<class>class Compare = std::less,
		template<class>class Alloc   = std::allocator 
	>
	class map: private std::map<KeyT, DataT, Compare<KeyT>, 
		                        Alloc<std::pair<const KeyT, DataT> > >
	{
	public:
		typedef Alloc<typename std::pair<const KeyT, DataT> >  allocator_type;

		typedef typename itl::map<KeyT, DataT, Compare, Alloc> type;
		typedef typename std::map<KeyT, DataT, Compare<KeyT>, 
			                      allocator_type>              base_type;
		typedef typename itl::set<KeyT, Compare, Alloc >       set_type;

	public:
		typedef KeyT                                       key_type;
		typedef DataT                                      mapped_type;
		typedef DataT                                      data_type;
		typedef std::pair<const KeyT, DataT>               value_type;
		typedef Compare<KeyT>                              key_compare;
		typedef typename base_type::value_compare          value_compare;

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
		map(){}
		map(const key_compare& comp): base_type(comp){}

		template <class InputIterator>
		map(InputIterator f, InputIterator l): base_type(f,l) {}

		template <class InputIterator>
		map(InputIterator f, InputIterator l, const key_compare& comp): base_type(f,l,comp) {}

		map(const map& src): base_type::map(src){}

		map& operator=(const map& src) { base_type::operator=(src); return *this; } 
		void swap(map& src) { base_type::swap(src); }

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

		using base_type::operator[];

	public:
		// --------------------------------------------------------------------
		// itl specific extensions

		//JODO concept set
		/// Checks if a key element is in the map
		bool contains(const KeyT& x)const { return !(find(x) == end()); }

		/** Is <tt>*this</tt> contained in <tt>super</tt>? */
		bool contained_in(const map& super)const { return Map::contained_in(*this, super); }

		/** Does <tt>*this</tt> contain <tt>sub</tt>? */
		bool contains(const map& sub)const { return Map::contained_in(sub, *this); }

		// Default inject-function using += on CodomTV
		iterator inject(const value_type& vp);

		iterator subtract(const key_type& key);
		// Default subtract-function using -= on CodomTV
		iterator subtract(const value_type& val);

		//JODO concept InplaceAddable
		/** Add a map \c x2 to this map. If an element of \c x2 already exists
			in \c *this, add up the contents using <tt>operator +=</tt>. */
		map& operator += (const map& x2) { Set::inject(*this, x2); return *this; }

		//JODO concept InplaceSubtractable
		/** Subtract a map \c x2 from this map. If an element of \c x2 already exists
			in \c *this, subtract the contents using <tt>operator -=</tt>. */
		map& operator -= (const map& x2) { Set::subtract(*this, x2); return *this; }

		/** Subtract a set \c x2 from this map. Every element of \c this map that
			has a key that is element of \c x2 is deleted from the map. */
		map& operator -= (const set_type& x2) { Set::subtract(*this, x2); return *this; }

		//JODO
		/** Intersect map \c x2 and \c *this.
			So \c *this becomes the intersection of \c *this and \c x2 */
		map& operator *= (const map& x2) { Map::intersect(*this, x2); return *this; }

		/** Intersect set \c x2 and \c *this.
			So \c *this becomes the intersection of \c *this and \c x2 */
		map& operator *= (const set_type& x2) { Map::intersect(*this, x2); return *this; }

		template<typename IteratorT>
		static const key_type& key_value(IteratorT& value_){ return (*value_).first; }

		template<typename IteratorT>
		static const data_type& data_value(IteratorT& value_){ return (*value_).second; }

		template<typename LeftIterT, typename RightIterT>
		static bool key_less(LeftIterT& lhs_, RightIterT& rhs_) 
		{ return key_compare()((*lhs_).first,(*rhs_).first); }

		static value_type make_element(const key_type& key_val, const data_type& data_val)
		{ return value_type(key_val, data_val); }

		//JODO: semantics implementation clash size/element_count
		size_t element_count()const { return size(); }


		//JODO concept StringRepresentable
		/** Represent this map as string */
		std::string asString()const;

#ifdef __ITL_EXTENDED__
		/** Keep the elements in *this map to which property \c hasProperty applies. 
		Erase all the rest. */
		map& keep_if(const PropertyT<value_type>& hasProperty);

		/** Erase the elements in *this map to which property \c hasProperty applies. 
		Keep all the rest. */
		map& drop_if(const PropertyT<value_type>& hasProperty);

		/** Copy the elements in map \c src to which property \c hasProperty applies 
		into \c *this map. */
		map& copy_if(const PropertyT<value_type>& hasProperty, const map& src);
#endif
	};


	template <typename KeyT, typename DataT, template<class>class Compare, template<class>class Alloc>
	inline bool operator == (const itl::map<KeyT,DataT,Compare,Alloc>& lhs,
							 const itl::map<KeyT,DataT,Compare,Alloc>& rhs)
	{
		typedef std::map<KeyT,DataT,Compare<KeyT>,Alloc<KeyT> > base_type;
		return operator==((const base_type&)lhs, (const base_type&)rhs);
	}
	
	template <typename KeyT, typename DataT, template<class>class Compare, template<class>class Alloc>
	inline bool operator < (const itl::map<KeyT,DataT,Compare,Alloc>& lhs,
		const itl::map<KeyT,DataT,Compare,Alloc>& rhs)
	{
		typedef std::map<KeyT,DataT,Compare<KeyT>,Alloc<KeyT> > base_type;
		return operator<((const base_type&)lhs, (const base_type&)rhs);
	}

	template <typename KeyT, typename DataT, template<class>class Compare, template<class>class Alloc>
	inline bool operator <= (const itl::map<KeyT,DataT,Compare,Alloc>& lhs,
		const itl::map<KeyT,DataT,Compare,Alloc>& rhs)
	{
		typedef std::map<KeyT,DataT,Compare<KeyT>,Alloc<KeyT> > base_type;
		return operator<=((const base_type&)lhs, (const base_type&)rhs);
	}


	template <typename KeyT, typename DataT, template<class>class Compare, template<class>class Alloc>
	typename map<KeyT,DataT,Compare,Alloc>::iterator
		map<KeyT,DataT,Compare,Alloc>::inject(const value_type& val)
	{
		if(val.CONT_VALUE == DataT())
			return end();

		std::pair<iterator, bool> insertion = insert(val);

		if( insertion.WAS_SUCCESSFUL )
			return insertion.ITERATOR ;
		else
		{
			iterator it = insertion.ITERATOR;
			(*it).CONT_VALUE += val.CONT_VALUE;

			if((*it).CONT_VALUE == DataT()) //neutron absorbtion
			{
				erase(it);
				return end();
			}
			else
				return it ;
		}
	}

	template <typename KeyT, typename DataT, template<class>class Compare, template<class>class Alloc>
	typename map<KeyT,DataT,Compare,Alloc>::iterator
		map<KeyT,DataT,Compare,Alloc>::subtract(const key_type& key)
	{
		iterator it_ = find(key);
		if(it_ != end())
			erase(it_);

		return end();
	}

	template <typename KeyT, typename DataT, template<class>class Compare, template<class>class Alloc>
	typename map<KeyT,DataT,Compare,Alloc>::iterator
		map<KeyT,DataT,Compare,Alloc>::subtract(const value_type& val)
	{
		iterator it_ = find(val.KEY_VALUE);
		if(it_ != end())
		{
			(*it_).CONT_VALUE -= val.CONT_VALUE;

			if((*it_).CONT_VALUE == DataT()) //neutron absorbtion
			{
				erase(it_);
				return end();
			}
			else
				return it_;
		}

		return it_;
	}

	template <typename KeyT, typename DataT, template<class>class Compare, template<class>class Alloc>
	std::string map<KeyT,DataT,Compare,Alloc>::asString()const
	{ 
		std::string repr;
		const_FORALL_THIS(it) {
			std::string elem("(");
			elem += ReprBaseT<KeyT>::toString((*it).KEY_VALUE);
			elem += "->";
			elem += ReprBaseT<DataT>::toString((*it).CONT_VALUE);
			elem += ")";

			repr += elem;
		}
		return repr;
	}


#ifdef __ITL_EXTENDED__
	template <typename KeyT, typename DataT, template<class>class Compare, template<class>class Alloc>
	map<KeyT,DataT,Compare,Alloc>& map<KeyT,DataT,Compare,Alloc>
		::drop_if(const PropertyT<value_type>& hasProperty)
	{
		iterator it = begin(), victim;
		while(it != end())
			if ( hasProperty(*it) ) { victim = it++; erase(victim); } else ++it;
		return *this;
	}

	template <typename KeyT, typename DataT, template<class>class Compare, template<class>class Alloc>
	map<KeyT,DataT,Compare,Alloc>& map<KeyT,DataT,Compare,Alloc>
		::keep_if(const PropertyT<value_type>& hasProperty)
	{
		iterator it = begin(), victim;
		while(it != end())
			if ( !hasProperty(*it) ) { victim = it++; erase(victim); } else ++it;
		return *this;
	}

	template <typename KeyT, typename DataT, template<class>class Compare, template<class>class Alloc>
	map<KeyT,DataT,Compare,Alloc>& map<KeyT,DataT,Compare,Alloc>
		::copy_if(const PropertyT<value_type>& hasProperty, const map<KeyT,DataT,Compare,Alloc>& src)
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
#endif

} // namespace itl

#endif // __itl_map_h_JOFA_070519__

