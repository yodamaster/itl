/*----------------------------------------------------------------------------+
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_separate_interval_set_h_JOFA_080608__
#define __itl_separate_interval_set_h_JOFA_080608__

#include <itl/interval_base_set.hpp>
#include <itl/j_assert.hpp>

namespace itl
{

/// Implements a set as a set of intervals - leaving adjoining intervals separate
/**    
    Template-class <b>split_interval_set</b> 
    implements a set as a set of intervals - bordering interval bounds are not merged

    Template parameter <b>DomainT</b>: The set's domain-type. Type of the 
    set's elements.
    
    Suitable as domain types are all datatypes that posess a partial order.
    In particular all discrete atomic datatypes like <tt>int, short, long</tt> and
    atomic pseudo-continuous datatypes <tt>float, double</tt> may be instantiated.
    
      Template parameter <b>Interval=itl::interval</b>: Type of interval used
    to implement the set. The default <b>itl::interval</b> uses the
    interval class template that comes with this library. Own implementation of interval
    classes are possible (but not trivial).

    <b>split_interval_set</b> implements a set <tt>set<DomainT></tt> as a set of intervals
    <tt>set<interval<DomainT>></tt>. Intervals will not be <b>not</b> merged on 
    insertion, if they border each other or overlap. The inserted intervalls will be
    be preserved. 
  
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

    <b>Restrictions: </b>
    
    A small number of functions can only be used for <b>discrete</b> domain datatypes 
    (<tt>short, int, Date</tt> etc.) that implement operators <tt>++</tt> and <tt>--</tt>.
    These functions are tagged in the documentation.

    @author  Joachim Faulhaber

*/
template 
<
    typename             DomainT, 
    template<class>class Interval = itl::interval,
    template<class>class Compare  = std::less,
    template<class>class Alloc    = std::allocator
> 
class separate_interval_set: 
    public interval_base_set<separate_interval_set<DomainT,Interval,Compare,Alloc>,
                             DomainT,Interval,Compare,Alloc>
{
public:
    // inherit all typedefs

    typedef interval_base_set<itl::separate_interval_set<DomainT,Interval,Compare,Alloc>,
                              DomainT,Interval,Compare,Alloc> base_type;

    typedef interval_set<DomainT,Interval,Compare,Alloc> joint_type;

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
    separate_interval_set(): base_type() {}
    /// Copy constructor
    separate_interval_set(const separate_interval_set& src): base_type(src) {}
    /// Constructor for a single interval
    explicit separate_interval_set(const interval_type& itv): base_type() { insert(itv); }


    /// Does the set contain the interval  <tt>x</tt>?
    bool contains(const interval_type& x)const;

    /// Insertion of an interval <tt>x</tt>
    void insert(const value_type& x);

    /// Removal of an interval <tt>x</tt>
    void subtract(const value_type& x);

    /// Treatment of adjoint intervals on insertion
    void handle_neighbours(const iterator& it){}
} ;


template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
bool separate_interval_set<DomainT,Interval,Compare,Alloc>::contains(const interval_type& x)const
{
    if(x.empty()) return true;

    typename ImplSetT::const_iterator fst_it = this->_set.lower_bound(x);
    typename ImplSetT::const_iterator end_it = this->_set.upper_bound(x);

    interval_set<DomainT,Interval,Compare,Alloc> matchSet;
    for(typename ImplSetT::const_iterator it=fst_it; it!=end_it; it++) 
        matchSet.insert(*it);

    interval_set<DomainT,Interval,Compare,Alloc> x_asSet; 
    x_asSet.insert(x);
    return x_asSet.contained_in(matchSet);
}



template<class DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void separate_interval_set<DomainT,Interval,Compare,Alloc>::insert(const value_type& x)
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
        insert(extended);
    }

}


template<class DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void separate_interval_set<DomainT,Interval,Compare,Alloc>::subtract(const value_type& x)
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

    insert(leftResid);
    insert(rightResid);
}


template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
inline bool is_element_equal(const separate_interval_set<DomainT,Interval,Compare,Alloc>& lhs,
                             const separate_interval_set<DomainT,Interval,Compare,Alloc>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class Type>
struct type<itl::separate_interval_set<Type> >
{
    static bool is_set() { return true; }
    static bool is_interval_container() { return true; }
    static bool is_interval_splitter() { return false; }
    static bool is_neutron_absorber() { return false; }
    static bool is_neutron_emitter() { return false; }

    static std::string to_string()
    { return "separate_interval_set<"+ type<Type>::to_string() +">"; }
};

} // namespace itl

#endif


