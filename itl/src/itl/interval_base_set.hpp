/*----------------------------------------------------------------------------+
Copyright (c) 2007: Joachim Faulhaber
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
class interval_base_set
--------------------------------------------------------------------*/
#ifndef __interval_base_set_h_JOFA_990223__
#define __interval_base_set_h_JOFA_990223__

#include <limits>
#include <itl/sets.hpp>
#include <itl/itl_set.hpp>
#include <itl/itl_interval.hpp>
#include <itl/notate.hpp>


#define const_FOR_IMPL(iter) for(typename ImplSetT::const_iterator iter=_set.begin(); (iter)!=_set.end(); (iter)++)
#define FOR_IMPL(iter) for(typename ImplSetT::iterator iter=_set.begin(); (iter)!=_set.end(); (iter)++)


namespace itl
{

//JODO update documentation (all invterval containers; template parameters have changed)
/// Implements a set as a set of intervals (abstract base class)
/**    
    Abstract template-class <b>interval_base_set</b> 
    implements a set as a set of intervals

    Template parameter <b>DomainT</b>: The set's domain-type. Type of the 
    set's elements.

    Suitable as domain types are all datatypes that posess a partial order.
    In particular all discrete atomic datatypes like <tt>int, short, long</tt> and
    atomic pseudo-continuous datatypes <tt>float, double</tt> may be instantiated.
    
      Template parameter <b>IntervT=interval<DomainT></b>: Type of interval used
    to implement the set. The default <b>interval<DomainT></b> uses the
    interval class that comes with this library. Own implementation of interval
    classes are possible (but not trivial).

    <b>interval_base_set</b> implements a set <tt>SetT<DomainT></tt> as a set of intervals
    <tt>SetT<interval<DomainT>></tt>.
  
    interval_base_set<DomainT> can thus be used like a set. As it is known from mathematics
    the union over a set of intervls is a set itself.

    <b>Class <tt>interval_base_set</tt> yields the following benefits: </b>
  
    <ul>
        <li> 
            A set of intervals is conceived as a set. The complexity involved with
            operations on intervals and sets of intervals is encapsulated.
            The user of the class who
            wants to perform set operations on sets of intervals is no more concerned
            with questions of overlapping, joining and bordering intervals.
        <li>
            <b>interval_base_set</b> gives an efficient implementation of sets consisting
            of larger contiguous chunks. Very large, even uncountably infinite sets of
            elements can be represented in a compact way and handled efficiently.
    </ul>

    <b>Restrictions: </b>
    
    A small number of functions can only be used for <b>discrete</b> domain datatypes 
    (<tt>short, int, Date</tt> etc.) that implement operators <tt>++</tt> and <tt>--</tt>.

    These functions are tagged in the documentation. Using such functions
    for continuous domain datatypes yields compiletime errors. C.f. getting
    the <tt>first()</tt> element of a left open interval makes sense for intervals of
    int but not for intervals of double.

    @author  Joachim Faulhaber
*/
template 
<
    typename             DomainT, 
    template<class>class Interval = itl::interval,
    template<class>class Compare  = std::less,
    template<class>class Alloc    = std::allocator
> 
class interval_base_set: public sets<DomainT>
{
public:

/** @name A: Type definitions for the template class 
    */
//@{ 
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

    //JODO Make exported types consistent
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
//@}

    // B: Constructors, destructors, assignment
    /// Default constructor for the empty set 
    interval_base_set(): _set() {}
    /// Copy constructor
    interval_base_set(const interval_base_set& src): _set(src._set) {}

    /// Virtual constructor
    virtual interval_base_set* cons()const=0;
    /// Virtual destruktor
    virtual ~interval_base_set(){};

    /// Assignment operator
    interval_base_set& operator = (const interval_base_set& src) 
    { 
        _set.ImplSetT::operator=(src._set);
        return *this;  
    }


    // ---------------------------------------------------------------------------
    // Interface SetIT
    // C:
    void clear() { _set.clear(); }
    bool empty()const { return _set.size()==0; }

    bool contains(const DomainT& x)const
    { typename ImplSetT::const_iterator it = _set.find(interval_type(x)); 
      return it != _set.end(); }

    void insert(const DomainT& x) { insert(interval_type(x)); }
    void subtract(const DomainT& x) { subtract(interval_type(x)); }

/** @name D: Virtual functions
    @memo By overwriting these functions we can implement interval sets
        that join neighbouring interval-bounds, or those that keep them
        distinct.        
    */
//@{ 
    /// Does the set contain the interval x?
    virtual bool contains(const interval_type& x)const=0;

    /** Is <tt>*this</tt> contained in <tt>super</tt>? */
    bool contained_in(const interval_base_set& super)const;

    /** Does <tt>*this</tt> contain <tt>sub</tt>? */
    bool contains(const interval_base_set& sub)const { return sub.contained_in(*this); }

    /// Treatment of adjoint intervals on insertion
    virtual void handle_neighbours(const typename ImplSetT::iterator& it)=0;
//@}

/** @name E: Bounds and other selectors
    */
//@{ 
    /// lower bound of all intervals in the set
    DomainT lower_bound()const { return (*(_set.begin())).lower_bound(); }
    /// upper bound of all intervals in the set
    DomainT upper_bound()const { return (*(_set.rbegin())).upper_bound(); }

    /// first (smallest) interval in the set
    interval_type first_interval()const { return (*(_set.begin())); }
    /// last (largest) interval in the set
    interval_type last_interval()const { return (*(_set.rbegin())); }

    /// enclosing Interval
    interval_type enclosure()const { return first_interval().span(last_interval()); }

    /// number of intervals
    size_t interval_count()const { return _set.size(); }
    size_t element_count()const { return _set.size(); }
//@}


/** @name F: Tester
    */
//@{
    /// Equality
    bool equal(const interval_base_set& x2)const
    { return contained_in(x2) && x2.contained_in(*this); }

    ///  <tt>*this</tt> and <tt>x2</tt> are disjoint; their intersection is empty.
    bool disjoint_to(const interval_base_set& x2)const;

    ///  <tt>*this</tt> and <tt>x2</tt> are disjoint; their intersection is empty.
    bool disjoint_to(const interval_type& x2)const;
//@}


/** @name G: Modificators
    */
//@{ 

    //CL
    ///// Insertion of an interval <tt>x</tt>
    virtual void insert(const value_type& x);
    ///// Removal of an interval <tt>x</tt>
    virtual void subtract(const value_type& x);

    /** Intersection with intervall x; The intersection is assigned to <tt>section</tt>. 
    
        Intersection also serves a generalized <tt>find</tt>-function to search
        for intervals in the set:

        <tt>ItvSetT<int> x, sec; interval<int> i; fill x; fill i;</tt>

        <tt>x.intersect(sec,i);</tt> 
        
        If <tt>i</tt> is an interval of <tt>x</tt>, then 
        
        <tt>sec.nOfIntervals()==1</tt> and <tt>*(sec.begin())==x</tt> 
    */
    void intersect(interval_base_set& section, const value_type& x)const;

    /** Perform intersection of <tt>*this</tt> and <tt>x</tt>; assign result
        to <tt>section</tt>
    */
    void intersect(interval_base_set& section, const interval_base_set& x)const;

    /** Perform intersection of <tt>*this</tt> and <tt>x</tt>; assign result
        to <tt>*this</tt>

        Aufruf <tt>x *= y</tt> bedeutet <tt>x = x geschnitten mit y </tt>
    */
    interval_base_set& operator *= (const interval_base_set& x);


    /// Union with set <tt>x</tt>
    interval_base_set& operator +=(const interval_base_set& x)
    { const_FORALL(typename ImplSetT, it, x._set) insert(*it); return *this; }

    /// Perform set difference with the set <tt>x</tt>
    interval_base_set& operator -= (const interval_base_set& x)
    { const_FORALL(typename ImplSetT, it, x._set) subtract(*it); return *this; }


    /// Join bordering intervals    
    interval_base_set& join();

    interval_base_set& scale_up(DomainT factor, DomainT max)
    { 
        FORALL(typename ImplSetT, it, _set) 
            (const_cast<interval_type&>(*it)).scale_up(factor, max); 
        return *this; 
    }
    
    interval_base_set& scale_up(const interval_base_set& src, DomainT factor, DomainT max);

    interval_base_set& scale_down(DomainT factor)
    { 
        FORALL(typename ImplSetT, it, _set) 
            (const_cast<interval_type&>(*it)).scale_down(factor); 
        return *this; 
    }
    
    interval_base_set& scale_down(const interval_base_set& src, DomainT factor);
    //@}

/** @name H: Interval search
    */
//@{
    /** A <tt>find</tt>-function is <b>NOT</b> implemented; use ref intersect
    
        All find operations can be expressed by means of ref intersect or  
        ref operator *=.
    */
//@}


/** @name I: Interval iterators
    */
//@{
    ///
    iterator begin() { return _set.begin(); }
    ///
    iterator end()   { return _set.end(); }
    ///
    const_iterator begin()const { return _set.begin(); }
    ///
    const_iterator end()const   { return _set.end(); }
//@}



/** @name S: String representation
    */
//@{
    /// Interval-set as string
    const std::string asString()const
    { std::string res(""); const_FOR_IMPL(it) res += (*it).asString(); return res; }
//@}

    
/** @name T: For discrete domain datatypes only that implement operators <tt>++</tt> 
        and <tt>--</tt>
    */
//@{
    /** Smallest element of the set (wrt. the partial ordering on DomainT).
        first() does not exist for continuous datatypes and open interval bounds.
    */
    DomainT first()const { return (*(_set.begin())).first(); }  // JODO NONCONT

    /** Largest element of the set (wrt. the partial ordering on DomainT).
        first() does not exist for continuous datatypes and open interval bounds.
    */
    DomainT last()const { return (*(_set.rbegin())).last(); } // JODO NONCONT

    // JODO TEST
    /** Number of elements in the set (cardinality). 
        Infinite for continuous domain datatyps
        <b>Nicht getestet</b>
    */
    DomainT size()const;

    /**    Set interval bounds to the type <tt>bt</tt> for intervals in the set.

        Interval bounds of different types are created by opeations on
        interval sets. This function allows to reset them uniformly without,
        of course, changing their value. This is only possible for discrete
        domain datatypes.
    */
    void uniform_bounds(typename interval<DomainT>::bound_types bt);

//@}


    template<typename IteratorT>
    static const key_type& key_value(IteratorT& value_){ return (*value_); }

    template<typename IteratorT>
    static codomain_type codomain_value(IteratorT& value_)
    { return (*value_).empty()? codomain_type() : (*value_).first(); }

    template<typename LeftIterT, typename RightIterT>
    static bool key_less(LeftIterT& lhs_, RightIterT& rhs_) 
    { return key_compare()(*lhs_, *rhs_); }

    static value_type make_domain_element(const domain_type& dom_val, const codomain_type& codom_val)
    { return value_type(interval_type(dom_val)); }


    // TESTCODE KEEP
    void to_set(element_set& s)const
    { const_FOR_IMPL(it) for(DomainT i=(*it).first(); i<=last(); i++) s.insert(i); } // JODO NONCONT 

    bool equal(element_set& x2)const
    { element_set x1; to_set(x1); return x1.contained_in(x2) && x2.contained_in(x1); }

private:
    void intersect_ascending(interval_base_set& section, const DomainT& start, const DomainT& span)const;
    void intersect_descending(interval_base_set& section, const DomainT& start, const DomainT& span)const;

protected:
    ImplSetT _set;

} ;


template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
DomainT interval_base_set<DomainT,Interval,Compare,Alloc>::size()const
{
    DomainT size = DomainT();
    const_FOR_IMPL(it) size += (*it).size();
    return size;
}


template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
bool interval_base_set<DomainT,Interval,Compare,Alloc>::contained_in(const interval_base_set& x2)const
{
    // The empty set is subset of every set
    if(empty())
        return true;
    else if (x2.empty())
        return false;
    else if(last() < x2.first())
        return false;
    else if(x2.last() < first())
        return false;
    else
    {
        // x2 should be larger than *this; so every element in this should be in x2
        const_FOR_IMPL(it) 
            if(!x2.contains(*it)) 
                return false;
        return true;
    }
}


//CL 
//template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
//bool interval_base_set<DomainT,Interval,Compare,Alloc>::operator <= (const interval_base_set<DomainT,Interval,Compare,Alloc>& x2)const
//{ return contained_in(x2); }



template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void interval_base_set<DomainT,Interval,Compare,Alloc>::insert(const value_type& x)
{
    if(x.empty()) return;

    std::pair<typename ImplSetT::iterator,bool> insertion = _set.insert(x);

    if(insertion.WAS_SUCCESSFUL)
        handle_neighbours(insertion.ITERATOR);
    else
    {
        typename ImplSetT::iterator fst_it = _set.lower_bound(x);
        typename ImplSetT::iterator end_it = _set.upper_bound(x);

        typename ImplSetT::iterator it=fst_it, nxt_it=fst_it, victim;
        Interval<DomainT> leftResid;  (*it).left_surplus(leftResid,x);
        Interval<DomainT> rightResid;

        while(it!=end_it)
        { 
            if((++nxt_it)==end_it) (*it).right_surplus(rightResid,x);
            victim = it; it++; _set.erase(victim);
        }

        Interval<DomainT> extended = x;
        extended.extend(leftResid).extend(rightResid);

        insert(extended);
    }

}



template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void interval_base_set<DomainT,Interval,Compare,Alloc>::subtract(const value_type& x)
{
    if(x.empty()) return;
    typename ImplSetT::iterator fst_it = _set.lower_bound(x);
    if(fst_it==_set.end()) return;
    typename ImplSetT::iterator end_it = _set.upper_bound(x);

    typename ImplSetT::iterator it=fst_it, nxt_it=fst_it, victim;
    interval_type leftResid; (*it).left_surplus(leftResid,x);
    interval_type rightResid;

    while(it!=end_it)
    { 
        if((++nxt_it)==end_it) (*it).right_surplus(rightResid,x);
        victim = it; it++; _set.erase(victim);
    }

    insert(leftResid);
    insert(rightResid);
}




template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
bool interval_base_set<DomainT,Interval,Compare,Alloc>::disjoint_to(const interval_type& x)const
{
    interval_base_set<DomainT,Interval,Compare,Alloc>* section = cons();
    intersect(*section, x);
    return section->empty();
}

template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
bool interval_base_set<DomainT,Interval,Compare,Alloc>::disjoint_to(const interval_base_set& x)const
{
    interval_base_set<DomainT,Interval,Compare,Alloc>* section = cons();
    intersect(*section, x);
    return section->empty();
}


template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void interval_base_set<DomainT,Interval,Compare,Alloc>::intersect(interval_base_set& section, const value_type& x)const
{
    section.clear();
    // any intersection with the empty intervall is empty
    if(x.empty()) return;

    typename ImplSetT::const_iterator fst_it = _set.lower_bound(x);
    typename ImplSetT::const_iterator end_it = _set.upper_bound(x);

    for(typename ImplSetT::const_iterator it=fst_it; it != end_it; it++) {
        interval_type isec; (*it).intersect(isec, x);
        section.insert(isec);
    }
}

//JODO CL?
//template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
//void interval_base_set<DomainT,Interval,Compare,Alloc>::intersect_ascending(interval_base_set& section, 
//                                         const DomainT& start, const DomainT& span)const
//{
//    section.clear();
//
//    // for right reach
//    interval<DomainT> maxItv = closedInterval<DomainT>(start,last());
//
//    ImplSetT::const_iterator fst_it = _set.lower_bound(maxItv);
//    DomainT maxSize = span;
//    DomainT curSize = 0;
//
//    ImplSetT::const_iterator it;
//    for(it=fst_it; curSize < maxSize && it != _set.end(); it++) 
//    {
//        interval_type isec; (*it).intersect(isec, maxItv);
//        curSize += isec.size();
//        // if curSize grows greater than max size, the last interval has to be
//        // shortened
//        if(curSize > maxSize)
//        {
//            DomainT right_surplus = curSize - maxSize;
//            interval_type lastSec = closedInterval<DomainT>(isec.first(),isec.last()-right_surplus);
//            section.insert(lastSec);
//            curSize -= right_surplus;
//        }
//        else section.insert(isec);
//
//        J_ASSERT(isec.size() <= maxSize);
//    }
//
//    J_ASSERT(it==_set.end() || curSize==maxSize);
//}

/*CL? CONTINUE
CAUTION first()/last() is wrong for ContinTV's
template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void interval_base_set<DomainT,Interval,Compare,Alloc>::intersect_descending(interval_base_set<DomainT,Interval,Compare,Alloc>& section, 
                                         const DomainT& start, const DomainT& span)const
{
    section.clear();

    // for left reach
    interval<DomainT> maxItv = closedInterval<DomainT>(first(), start);

    ImplSetT::const_iterator fst_it = _set.lower_bound(maxItv);
    DomainT maxSize = span;
    DomainT curSize = 0;

    ImplSetT::const_iterator it;
    for(it=fst_it; curSize < maxSize && it != _set.end(); it++) 
    {
        interval_type isec; (*it).intersect(isec, maxItv);
        curSize += isec.size();
        // if curSize grows greater than max size, the last interval has to be
        // shortened
        if(curSize > maxSize)
        {
            DomainT right_surplus = curSize - maxSize;
            interval_type lastSec = closedInterval<DomainT>(isec.first(),isec.last()-right_surplus);
            section.insert(lastSec);
            curSize -= right_surplus;
        }
        else section.insert(isec);

        J_ASSERT(isec.size() <= maxSize);
    }

    J_ASSERT(it==_set.end() || curSize==maxSize);
}
*/


template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void interval_base_set<DomainT,Interval,Compare,Alloc>::intersect(interval_base_set& interSection, 
                                            const interval_base_set& x)const
{
    interSection.clear();
    if(x.empty()) return;

    interval_base_set<DomainT,Interval,Compare,Alloc>* aux = cons();
    const_FORALL(typename ImplSetT, it, x._set)
    {
        aux->clear();
        intersect(*aux, *it);
        interSection += (*aux);
    }
    delete aux;
}


template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
interval_base_set<DomainT,Interval,Compare,Alloc>& 
    interval_base_set<DomainT,Interval,Compare,Alloc>::operator *= (const interval_base_set& x)
{
    interval_base_set<DomainT,Interval,Compare,Alloc>* section = cons();
    intersect(*section, x);
    section->_set.swap(_set);
    delete section;
    return *this;
}
    




template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
interval_base_set<DomainT,Interval,Compare,Alloc>& interval_base_set<DomainT,Interval,Compare,Alloc>::join()
{
    iterator it=_set.begin();
    if(it==_set.end()) 
        return *this;

    iterator nxt=it; nxt++;
    if(nxt==_set.end()) 
        return *this;

    while(nxt != _set.end())
    {
        if( (*it).touches(*nxt) )
        {
            iterator fst_mem = it;  // hold the fist member
            
            // go noodling on while touchin members found
            it++; nxt++;
            while(     nxt != _set.end()
                    && (*it).touches(*nxt) )
            { it++; nxt++; }

            // finally we arrive at the end of a sequence of joinable intervals
            // and it points to the last member of that sequence
            iterator lst_mem = it, end_mem = nxt;
            interval_type joinedInterval(*fst_mem);
            joinedInterval.extend(*lst_mem);
            
            _set.erase(fst_mem, end_mem);
            it = _set.insert(joinedInterval).ITERATOR;

            it++; // go on for the next after the currently inserted
            nxt=it; if(nxt!=_set.end())nxt++;
        }
        else { it++; nxt++; }
    }
    return *this;
}



template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
void interval_base_set<DomainT,Interval,Compare,Alloc>::uniform_bounds(typename interval<DomainT>::bound_types bt)
{
    // I can do this only, because I am shure that the contents and the
    // ordering < on interval is invariant wrt. this transformation on bounds
    FOR_IMPL(it) const_cast<interval_type&>(*it).transform_bounds(bt);
}


template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
interval_base_set<DomainT,Interval,Compare,Alloc>& interval_base_set<DomainT,Interval,Compare,Alloc>::scale_up(const interval_base_set& src, DomainT factor, DomainT max)
{ 
    clear();
    const_FORALL(typename interval_base_set, it, src)
    {
        interval_type itv = *it;
        itv.scale_up(factor, max);
        _set.insert(itv);
    }
    return *this;
}

template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
interval_base_set<DomainT,Interval,Compare,Alloc>& interval_base_set<DomainT,Interval,Compare,Alloc>::scale_down(const interval_base_set& src, DomainT factor)
{ 
    clear();
    const_FORALL(typename interval_base_set, it, src)
    {
        interval_type itv = *it;
        itv.scale_down(factor);
        _set.insert(itv); //JODO THINK Preconditions
    }
    return *this;
}



template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
inline bool operator == (const interval_base_set<DomainT,Interval,Compare,Alloc>& lhs,
                         const interval_base_set<DomainT,Interval,Compare,Alloc>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
inline bool operator < (const interval_base_set<DomainT,Interval,Compare,Alloc>& lhs,
                        const interval_base_set<DomainT,Interval,Compare,Alloc>& rhs)
{
    return std::lexicographical_compare(
        lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), Compare<Interval<DomainT> >());
}

template <typename DomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
inline bool operator <= (const interval_base_set<DomainT,Interval,Compare,Alloc>& lhs,
                         const interval_base_set<DomainT,Interval,Compare,Alloc>& rhs)
{
    return lhs < rhs || lhs == rhs;
}




} // namespace itl

#endif


