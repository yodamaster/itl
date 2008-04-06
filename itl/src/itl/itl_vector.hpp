/*----------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
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
class vector
a general list class that extends stl-vectors
--------------------------------------------------------------------*/
#ifndef __itl_vector_h_JOFA_001005__
#define __itl_vector_h_JOFA_001005__

#include <vector>
#include <itl/notate.hpp>
#include <itl/j_assert.hpp>
#include <string>

namespace itl
{	
	
	//JODO 1_0_1 documentation
	/// an stl based list implementing inplace addition operators += 
	/** 
		@author Joachim Faulhaber
	*/
	template <typename DataT, typename AllocT = std::allocator<DataT> >
	class vector: private std::vector<DataT, AllocT>
	{
	public:
		typedef DataT    value_type;
		typedef AllocT   allocator_type;

	private:
		typedef typename itl::vector<DataT, AllocT> type;
		typedef typename std::vector<DataT, AllocT> base_type;

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
		explicit vector(const AllocT& alloc = AllocT()): base_type(alloc) {}

		explicit vector(size_type size, const DataT& value = DataT(), const AllocT& alloc = AllocT()): 
			base_type(size, value, alloc) {}
		
		vector(const vector& src): base_type(src) {}

		template<typename InputIteratorT>
		vector(InputIteratorT first, InputIteratorT last, const AllocT& alloc = AllocT()):
			base_type(first, last, alloc) {}
		
		~vector(){}

		using base_type::assign;
		vector& operator=(const vector& src) { base_type::operator=(src); return *this; } 
		void swap(vector& src) { base_type::swap(src); }

		using base_type::begin;
		using base_type::end;
		using base_type::rbegin;
		using base_type::rend;

		using base_type::size;
		using base_type::max_size;
		using base_type::capacity;
		using base_type::reserve;
		using base_type::resize;
		using base_type::empty;

		using base_type::front;
		using base_type::back;
		using base_type::push_back;
		using base_type::pop_back;

		using base_type::insert;
		using base_type::erase;
		using base_type::clear;

		using base_type::operator[];
		vector& operator += (const vector& rhs);
	} ;
		
	template <typename DataT, typename AllocT> 
	vector<DataT,AllocT>& vector<DataT,AllocT>::operator += (const vector<DataT,AllocT>& rhs)
	{
		J_ASSERT(size()==rhs.size());
		for(int idx=0; idx<size(); idx++)
			(*this)[idx] += rhs[idx];
		return *this;
	}

	template <typename DataT, typename AllocT>
	inline bool operator == (const itl::vector<DataT,AllocT>& lhs,
		const itl::vector<DataT,AllocT>& rhs)
	{
		typedef std::vector<DataT,AllocT> base_type;
		return operator==((const base_type&)lhs, (const base_type&)rhs);
	}

	template <typename DataT, typename AllocT>
	inline bool operator < (const itl::vector<DataT,AllocT>& lhs,
		const itl::vector<DataT,AllocT>& rhs)
	{
		typedef std::vector<DataT,CompareT,AllocT> base_type;
		return operator<((const base_type&)lhs, (const base_type&)rhs);
	}



	//-------------------------------------------------------------------------
	// ptr_vector 
	//-------------------------------------------------------------------------


	//JODO 1_0_1 documentation
	/// an stl based list implementing inplace addition operators += 
	/** 
	@author Joachim Faulhaber
	*/
	template <typename DataT, typename AllocT = std::allocator<DataT*> >
	class ptr_vector: private std::vector<DataT*, AllocT>
	{
	public:
		typedef DataT*   value_type;
		typedef AllocT   allocator_type;

	private:
		typedef typename itl::vector<DataT, AllocT> type;
		typedef typename std::vector<value_type, AllocT> base_type;

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
		explicit ptr_vector(const AllocT& alloc = AllocT()): base_type(alloc) {}
		explicit ptr_vector(size_type size, value_type value = value_type(), const AllocT& alloc = AllocT()): 
		base_type(size, value, alloc) {}

		ptr_vector(const ptr_vector& src): base_type(src) {}

		template<typename InputIteratorT>
		ptr_vector(InputIteratorT first, InputIteratorT last, const AllocT& alloc = AllocT()):
		base_type(first, last, alloc) {}

		~ptr_vector(){}

		using base_type::assign;
		ptr_vector& operator=(const ptr_vector& src) { base_type::operator=(src); return *this; } 
		void swap(ptr_vector& src) { base_type::swap(src); }

		using base_type::begin;
		using base_type::end;
		using base_type::rbegin;
		using base_type::rend;

		using base_type::size;
		using base_type::max_size;
		using base_type::capacity;
		using base_type::reserve;
		using base_type::resize;
		using base_type::empty;

		using base_type::front;
		using base_type::back;
		using base_type::push_back;
		using base_type::pop_back;

		using base_type::insert;
		using base_type::erase;
		using base_type::clear;

		using base_type::operator[];
		ptr_vector& operator += (const ptr_vector& rhs);

		std::string asString(std::string(DataT::*asStr)()const, const char* sep=" ")const;
		bool contains(DataT*)const;
		ptr_vector<DataT,AllocT>& clone(const ptr_vector&);
		bool append_unique(DataT*);

	} ;

	template <typename DataT, typename AllocT> 
	ptr_vector<DataT,AllocT>& ptr_vector<DataT,AllocT>::operator += (const ptr_vector<DataT,AllocT>& rhs)
	{
		J_ASSERT(size()==rhs.size());
		for(int idx=0; idx<size(); idx++)
			(*(*this)[idx]) += (*rhs[idx]);
		return *this;
	}

	template <typename DataT, typename AllocT>
	inline bool operator == (const itl::ptr_vector<DataT,AllocT>& lhs,
		const itl::ptr_vector<DataT,AllocT>& rhs)
	{
		typedef std::ptr_vector<DataT,AllocT> base_type;
		return operator==((const base_type&)lhs, (const base_type&)rhs);
	}

	template <typename DataT, typename AllocT>
	inline bool operator < (const itl::ptr_vector<DataT,AllocT>& lhs,
		const itl::ptr_vector<DataT,AllocT>& rhs)
	{
		typedef std::ptr_vector<DataT,CompareT,AllocT> base_type;
		return operator<((const base_type&)lhs, (const base_type&)rhs);
	}	
	
	template <typename DataT, typename AllocT>
	std::string ptr_vector<DataT,AllocT>::asString(std::string(DataT::*asStr)()const, const char* sep /* =" " */)const
	{
		const_iterator it=begin();
		if(it == end()) return stringT("");
		else
		{
			stringT y(((*it)->*asStr)()); it++;
			while(it != end()) 
			{ y+=sep; y += ((*it)->*asStr)(); it++; }
			return y;
		}
	}
	
	template <typename DataT, typename AllocT>
		bool ptr_vector<DataT,AllocT>::contains(DataT* val)const
	{
		const_FORALL_THIS(it)
			if(*it==val) return true;
			return false;
	}
	
	template <typename DataT, typename AllocT>
		ptr_vector<DataT,AllocT>& ptr_vector<DataT,AllocT>::clone(const ptr_vector<DataT,AllocT>& src)
	{
		clear();
		
		for(int i=0; i<src.size(); i++)
		{
			DataT* cl = dynamic_cast<DataT*>(src[i]->clone());
			push_back(cl);
		}
		return *this;

		/*JODO CL resize(src.size());
		for(int i=0; i<src.size(); i++)
		{
			DataT* cl = dynamic_cast<DataT*>(src[i]->clone());
			this->operator[](cl) = cl;
		}
		return *this;
		*/
	}

	/** Append x, if it is not in the vector already.
		CAUTION: Use for short vectors only. For long containers
		use fast inserters like sorted sets or hash sets
	*/
	template <typename DataT, typename AllocT>
		bool ptr_vector<DataT,AllocT>::append_unique(DataT* x)
	{		
		for(int i=0; i<size(); i++)
			if((*this)[i]==x)
				return false;

		push_back(x);
		return true;
	}


} // namespace itl

#endif

