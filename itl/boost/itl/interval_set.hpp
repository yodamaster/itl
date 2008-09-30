/*----------------------------------------------------------------------------+
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
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
class interval_set
--------------------------------------------------------------------*/
#ifndef __itl_interval_set_h_JOFA_990223__
#define __itl_interval_set_h_JOFA_990223__

#include <itl/interval_base_set.hpp>
#include <itl/interval_sets.hpp>
#include <itl/j_assert.hpp>

namespace itl
{

/// Implements a set as a set of intervals - merging adjoining intervals
/**    
    Template-class <b>interval_set</b> 
    implements a set as a set of intervals - interval bounds are merged if 
    inserted intervals overlap or adjoin.

    Template parameter <b>DomainT</b>: The set's domain-type. Type of the 
    set's elements.

    Suitable as domain types are all datatypes that posess a partial order.
    In particular all discrete atomic datatypes like <tt>int, short, long</tt> and
    atomic pseudo-continuous datatypes <tt>float, double</tt> may be instantiated.
    
      Template parameter <b>Interval=itl::interval</b>: Type of interval used
    to implement the set. The default <b>itl::interval</b> uses the
    interval class that comes with this library. Own implementation of interval
    classes are possible (but not trivial).

    <b>interval_set</b> implements a set <tt>set<DomainT></tt> as a set of intervals
    <tt>set<interval<DomainT>></tt>. Intervals will be merged on insertion, if they
    border each other or overlap.
  
    interval_base_set<DomainT> can thus be used like a set. As it is known from mathematics
    the union over a set of intervls is a set itself.

    <b>Class <tt>interval_set</tt> yields the following benefits: </b>
  
    <ul>
        <li> 
            A set of intervals is conceived as a set. The complexity involved with
            operations on intervals and sets of intervals is encapsulated.
            The user of the class who
            wants to perform set operations on sets of intervals is no more concerned
            with questions of overlapping, joining and bordering intervals.
        <li>
            <b>interval_set</b> gives an efficient implementation of sets consisting
            of larger contiguous chunks. Very large, even uncountably infinite sets of
            elements can be represented in a compact way and handled efficiently.
    </ul>

    Intervals in <b>interval_sets</b> are always unified, if intervals are adjoint. So the number
    of interval in an <b>interval_set</b> is always minimal. So informations about bounds
    of inserted intervals are lost if those intervals are adjoint to or overlapping
    with intervals within the <b>interval_set</b>.

    <b>Restrictions: </b>
    
    A small number of functions can only be used for <b>discrete</b> domain datatypes 
    (<tt>short, int, Date</tt> etc.) that implement operators <tt>++</tt> and <tt>--</tt>.
    These functions are tagged in the documentation.

    @author Joachim Faulhaber
*/

/*  JODO Using such functions
    for continuous domain datatypes yields compiletime errors. E.g. getting
    the <tt>first()</tt> element of a left open interval makes sense for intervals of
    int but not for intervals of double.
*/

template 
<
    typename             DomainT, 
    template<class>class Interval = itl::interval,
    template<class>class Compare  = std::less,
    template<class>class Alloc    = std::allocator
> 
class interval_set: 
    public interval_base_set<interval_set<DomainT,Interval,Compare,Alloc>,
                             DomainT,Interval,Compare,Alloc>
{
public:

    /// The base_type of this class
    typedef interval_base_set<itl::interval_set<DomainT,Interval,Compare,Alloc>,
                              DomainT,Interval,Compare,Alloc> base_type;

    typedef interval_set<DomainT,Interval,Compare,Alloc> type;
    typedef type joint_type;

    /// The domain type of the set
    typedef DomainT   domain_type;
    /// The codomaintype is the same as domain_type
    typedef DomainT   codomain_type;

    /// The interval type of the set
    typedef Interval<DomainT> interval_type;

    /// Comparison functor for domain values
    typedef Compare<DomainT> domain_compare;
    /// Comparison functor for intervals
    typedef exclusive_less<interval_type> interval_compare;

    /// Comparison functor for keys
    typedef exclusive_less<interval_type> key_compare;

    /// The allocator type of the set
    typedef Alloc<interval_type> allocator_type;

    /// allocator type of the corresponding element set
    typedef Alloc<DomainT> domain_allocator_type;

    /// The type of the set of elements that is equivalent to the set of intervals
    typedef typename itl::set<DomainT,Compare,Alloc> element_set;

    /// The corresponding atomized type representing this ineterval container of elements
    typedef typename itl::set<DomainT,Compare,Alloc> atomized_type;

    /// Container type for the implementation 
    typedef typename itl::set<interval_type,exclusive_less,Alloc> ImplSetT;

    /// key type of the implementing container
    typedef typename ImplSetT::key_type   key_type;
    /// data type of the implementing container
    typedef typename ImplSetT::data_type  data_type;
    /// value type of the implementing container
    typedef typename ImplSetT::value_type value_type;

    /// iterator for iteration over intervals
    typedef typename ImplSetT::iterator iterator;
    /// const_iterator for iteration over intervals
    typedef typename ImplSetT::const_iterator const_iterator;



    // B: Constructors, destructors, assignment
    /// Default constructor for the empty set 
    interval_set(): base_type() {}
    /// Copy constructor
    interval_set(const interval_set& src): base_type(src) {}

	/// Copy constructor for base_type
	template<class SubType>
    interval_set
		(const interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& src)
	{ assign(src); }

    /// Constructor for a single element
	explicit interval_set(const domain_type& itv): base_type() 
	{ add(interval_type(itv)); }
    /// Constructor for a single interval
	explicit interval_set(const interval_type& itv): base_type() 
	{ add(itv); }

    /// Assignment from a base interval_set.
	template<class SubType>
	void assign(const interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& src)
	{
		typedef interval_base_set<SubType,DomainT,Interval,Compare,Alloc> base_set_type;
		this->clear();
		// Has to be implemented via add. there might be touching borders to be joined
		const_FORALL(base_set_type, it, src) 
			this->add(*it);
	}

    /// Does the set contain the interval  <tt>x</tt>?
    bool contains_(const interval_type& x)const;

    /// Insertion of an interval <tt>x</tt>
    void add_(const value_type& x);

    /// Removal of an interval <tt>x</tt>
    void subtract_(const value_type& x);

    /// Treatment of adjoint intervals on insertion
    void handle_neighbours(const iterator& it);

protected:
    iterator joint_insert(const iterator& left_it, const iterator& right_it);
} ;




template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
bool interval_set<DomainT,Interval,Compare,Alloc>::contains_(const interval_type& x)const
{ 
    // Emptiness is contained in everything
    if(x.empty()) 
        return true;
    else if (this->empty())
        return false;
    else if(x.upper() < this->lower())
        return false;
    else if(this->upper() < x.lower())
        return false;
    {
        typename ImplSetT::const_iterator it = this->_set.find(x);
        if(it == this->_set.end())
            return false;
        else
            return x.contained_in(*it);
    }
}


template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void interval_set<DomainT,Interval,Compare,Alloc>::handle_neighbours(const iterator& it)
{
    if(it == this->_set.begin())
    {
        iterator it_nxt=it; it_nxt++;
        if(it_nxt!=this->_set.end() && (*it).touches(*it_nxt)) 
            joint_insert(it, it_nxt);
    }
    else
    {
        // there is a predecessor
        iterator it_pred = it; it_pred-- ;

        if((*it_pred).touches(*it)) 
        {
            iterator it_extended = joint_insert(it_pred, it);

            iterator it_succ=it_extended; it_succ++;
            if(it_succ!=this->_set.end())
            {
                // it's a non border element that might have two touching neighbours
                if((*it_extended).touches(*it_succ)) 
                    joint_insert(it_extended, it_succ);
            }
        }
        else
        {
            iterator it_succ=it; it_succ++;
            if(it_succ!=this->_set.end())
            {
                // it's a non border element that might have a right touching neighbours
                if((*it).touches(*it_succ)) 
                    joint_insert(it, it_succ);
            }
        }
    }
}



template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
typename interval_set<DomainT,Interval,Compare,Alloc>::iterator 
    interval_set<DomainT,Interval,Compare,Alloc>
    ::joint_insert(const iterator& left_it, const iterator& right_it)
{
    // both left and right are in the set and they are neighbours
    DEV_ASSERT((*left_it).excl_less(*right_it));
    DEV_ASSERT((*left_it).touches(*right_it));

    interval_type curItv = (*left_it);
    curItv.extend(*right_it);

    this->_set.erase(left_it);
    this->_set.erase(right_it);
    
    iterator new_it = this->_set.insert(curItv).ITERATOR;
    J_ASSERT(new_it!=this->_set.end());
    return new_it;
}


template<class DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void interval_set<DomainT,Interval,Compare,Alloc>::add_(const value_type& x)
{
    if(x.empty()) return;

    std::pair<typename ImplSetT::iterator,bool> insertion = this->_set.insert(x);

    if(insertion.WAS_SUCCESSFUL)
        handle_neighbours(insertion.ITERATOR);
    else
    {
        typename ImplSetT::iterator fst_it = this->_set.lower_bound(x);
        typename ImplSetT::iterator end_it = this->_set.upper_bound(x);

        typename ImplSetT::iterator it=fst_it, nxt_it=fst_it, victim;
        Interval<DomainT> leftResid;  (*it).left_surplus(leftResid,x);
        Interval<DomainT> rightResid;

        while(it!=end_it)
        { 
            if((++nxt_it)==end_it) 
                (*it).right_surplus(rightResid,x);
            victim = it; it++; this->_set.erase(victim);
        }

        Interval<DomainT> extended = x;
        extended.extend(leftResid).extend(rightResid);
        extended.extend(rightResid);
        add(extended);
    }
}


template<class DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void interval_set<DomainT,Interval,Compare,Alloc>::subtract_(const value_type& x)
{
    if(x.empty()) return;
    typename ImplSetT::iterator fst_it = this->_set.lower_bound(x);
    if(fst_it==this->_set.end()) return;
    typename ImplSetT::iterator end_it = this->_set.upper_bound(x);

    typename ImplSetT::iterator it=fst_it, nxt_it=fst_it, victim;
    interval_type leftResid; (*it).left_surplus(leftResid,x);
    interval_type rightResid;

    while(it!=end_it)
    { 
        if((++nxt_it)==end_it) (*it).right_surplus(rightResid,x);
        victim = it; it++; this->_set.erase(victim);
    }

    add(leftResid);
    add(rightResid);
}


//-----------------------------------------------------------------------------
// equality of elements
//-----------------------------------------------------------------------------
template <typename DomainT, template<class>class Interval, 
          template<class>class Compare, template<class>class Alloc>
inline bool is_element_equal(const interval_set<DomainT,Interval,Compare,Alloc>& lhs,
                             const interval_set<DomainT,Interval,Compare,Alloc>& rhs)
{
	return &lhs == &rhs || Set::lexicographical_equal(lhs, rhs);
}

template 
<
    class SubType, class DomainT, template<class>class Interval, 
    template<class>class Compare, template<class>class Alloc
>
inline bool 
is_element_equal
(
    const interval_set             <DomainT,Interval,Compare,Alloc>& lhs,
    const interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& rhs
)
{
	typedef itl::interval_set<DomainT,Interval,Compare,Alloc> joined_type;
	joined_type joined_rhs(rhs);
	return Set::lexicographical_equal(lhs, joined_rhs);
}


//-----------------------------------------------------------------------------
// addition (set union) += and subtraction (set difference) -=
//-----------------------------------------------------------------------------
template 
<
    class SubType, class DomainT, template<class>class Interval, 
    template<class>class Compare, template<class>class Alloc
>
interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& 
operator +=
(
          interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& object,
    const interval_set             <DomainT,Interval,Compare,Alloc>& operand
)
{
	typedef itl::interval_set<DomainT,Interval,Compare,Alloc> set_type;
    const_FORALL(typename set_type, elem_, operand) 
        object.add(*elem_); 

    return object; 
}

template 
<
    class SubType, class DomainT, template<class>class Interval, 
    template<class>class Compare, template<class>class Alloc
>
interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& 
operator -=
(
          interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& object,
    const interval_set             <DomainT,Interval,Compare,Alloc>& operand
)
{
	typedef itl::interval_set<DomainT,Interval,Compare,Alloc> set_type;
    const_FORALL(typename set_type, elem_, operand) 
        object.subtract(*elem_); 

    return object; 
}

template 
<
    class SubType, class DomainT, template<class>class Interval, 
    template<class>class Compare, template<class>class Alloc
>
interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& 
erase
(
          interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& object,
    const interval_set             <DomainT,Interval,Compare,Alloc>& operand
)
{
    return object -= operand;
}

//-----------------------------------------------------------------------------
// intersection *=
//-----------------------------------------------------------------------------
//template 
//<
//	class SubType, class DomainT, template<class>class Interval, 
//	template<class>class Compare, template<class>class Alloc
//>
//interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& 
//operator *=
//(
//		  interval_base_set<SubType,DomainT,Interval,Compare,Alloc>& object,
//	const interval_set             <DomainT,Interval,Compare,Alloc>& operand
//)
//{
//	typedef interval_base_set<SubType,DomainT,Interval,Compare,Alloc> object_type;
//	typedef interval_set             <DomainT,Interval,Compare,Alloc> operand_type;
//	object_type intersection;
//
//	if(operand.empty())
//	{
//		object.clear();
//		return object;
//	}
//
//	operand_type::const_iterator common_lwb;
//	operand_type::const_iterator common_upb;
//
//	if(!Set::common_range(common_lwb, common_upb, operand, object))
//	{
//		object.clear();
//		return object;
//	}
//
//	operand_type::const_iterator it = common_lwb;
//	while(it != common_upb)
//		object.add_intersection(intersection, *it++);
//
//	object.swap(intersection);
//
//	return object; 
//}



template <class Type>
struct type<itl::interval_set<Type> >
{
    static bool is_set() { return true; }
    static bool is_interval_container() { return true; }
    static bool is_interval_splitter() { return false; }
    static bool is_neutron_absorber() { return false; }
    static bool is_neutron_emitter() { return false; }

    static std::string to_string()
    { return "interval_set<"+ type<Type>::to_string() +">"; }
};

} // namespace itl

#endif


