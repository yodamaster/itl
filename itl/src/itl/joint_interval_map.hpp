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
class joint_interval_map
--------------------------------------------------------------------*/
#ifndef __joint_interval_map_h_JOFA_000706__
#define __joint_interval_map_h_JOFA_000706__

#include <itl/interval_base_map.hpp>
#include <itl/interval_set.hpp>

namespace itl
{

/**    
	Abstract template-class <b>joint_interval_map</b>
	implements a map as a map of intervals - On insertion overlapping
	intervals are unified. The associalted values are summed up according to
	the codomains operation +=.

	Template parameter <b>DomainT</b>: Domain type of the map: Type of the
	map's keys.
	
	Suitable as domain types are all datatypes that posess a partial order.
	In particular all discrete atomic datatypes like <tt>int, short, long</tt> and
	atomic pseudo-continuous datatypes <tt>float, double</tt> may be instantiated.
	  
   	Template parameter <b>CodomTV</b>: The map's codomain type or value type: Type
	of values contained in the map.

    Datatypes for the codomain parameter has to <b>implement</b> operations
	<tt>+=</tt>, <tt>-=</tt>, <tt>==</tt> (equality) and <tt>CodomTV()</tt> (default constructor).

    The default constructor <tt>CodomTV()</tt> has to contruct a neutral element
	such that the following holds:

	  If <tt>x = y; y += CodomTV();</tt> then <tt>x==y;</tt> and
	  If <tt>x = y; y -= CodomTV();</tt> then <tt>x==y;</tt>
  
	Template parameter <b>IntervT=IntervT<DomainT></b>: Type of interval used
	to implement the set. The default <b>IntervT<DomainT></b> uses the
	interval class that comes with this library. Own implementation of interval
	classes are possible (but not trivial).
  
	<b>ItvMapAT</b> implements a map <tt>MapT<DomainT, CodomTV></tt> as a map
	of intervals <tt>MapT<IntervT<DomainT>, CodomTV, ExclusiveLessT<IntervT> ></tt>

	Interval maps <tt>ItvMapAT<DomainT,CodomTV></tt> can be used similar (and in many
	aspects exactly like) common stl-maps. Unlike to stl-maps where you store
	a value for every key an interval map stores a contents value for an interval of
	keys. In it's degenerated form the key intervals contain a single element
	only. Then the interval map works like a normal stl-map. But if you work in problem
	domains where you associate values to large contiguous intervals, interval maps
	are very useful and efficient.

	<b>Class <tt>joint_interval_map</tt>  yields the following benefits: </b>

	<ul>
		<li> A set of intervals is conceived as the domain set of the map. 
			The complexity involved with
			operations on intervals maps is encapsulated. The user of the class who
			wants to perform operations on interval maps is no more concerned
			with questions of overlapping, joining and bordering intervals.
		<li>
			<b>joint_interval_map</b> gives an efficient implementation of maps consisting
			of larger contiguous chunks. Very large, even uncountably infinite maps
			can be represented in a compact way and handled efficiently.
		<li> <b>joint_interval_map can used as unification machine</b>. joint_interval_map
			computes the union on assoiciated values if intervals are overlapping.
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
template <typename DomainT, typename CodomainT, typename IntervT=interval<DomainT>, 
		  typename CompareT = std::less<DomainT>,
		  typename AllocT   = std::allocator<std::pair<const IntervT, CodomainT> > >
class joint_interval_map: public interval_base_map<DomainT,CodomainT,IntervT,CompareT,AllocT>
{
public:
	typedef typename interval_base_map<DomainT,CodomainT,IntervT,CompareT,AllocT> base_type;

	// inherit all typedefs
	joint_interval_map(): base_type() {}
	joint_interval_map(const joint_interval_map& src): base_type(src) {}

	virtual base_type* cons()const
	{ return new joint_interval_map(); }

	virtual bool contains(const value_type& x)const;

	virtual void insert(const value_type&);
	virtual void subtract(const value_type& x);

	//JODO joint_interval_map is not completely implemented and not tested
	virtual void handle_neighbours(const iterator& it);

	void toItvSetT(interval_set<DomainT,IntervT,CompareT,AllocT>& dst)const
	{ dst.clear(); const_FORALL(ImplMapT, it, _map) dst.insert(*it); }

private:
	iterator joint_insert(const iterator& left_it, const iterator& right_it);
};



template <typename DomainT, typename CodomainT, typename IntervT, typename CompareT, typename AllocT>
bool joint_interval_map<DomainT,CodomainT,IntervT,CompareT,AllocT>::contains(const value_type& x_y)const
{
	IntervT x = x_y.KEY_VALUE;
	if(x.empty()) return true;

	joint_interval_map match_map;
	matchMap(match_map, x_y);

	if(match_map.interval_count() != 1) return false;
	iterator match_it = match_map._map.find(x);
	if(! x.contained_in((*match_it).KEY_VALUE) ) return false;

	return (*match_it).CONT_VALUE==x_y.CONT_VALUE;
}



template <typename DomainT, typename CodomainT, typename IntervT, typename CompareT, typename AllocT>
void joint_interval_map<DomainT,CodomainT,IntervT,CompareT,AllocT>::insert(const value_type& x)
{
	/*
#ifdef _DEBUG
	interval_set<DomainT,IntervT,CompareT,AllocT> clone; toItvSetT(clone);
#endif
	*/

	base_type::insert(x);

	// ON_DEBUG(clone.insert(x);)
	// J_ASSERT(isEqual(clone))
}


template <typename DomainT, typename CodomainT, typename IntervT, typename CompareT, typename AllocT>
void joint_interval_map<DomainT,CodomainT,IntervT,CompareT,AllocT>::subtract(const value_type& x)
{
	/*
#ifdef _DEBUG
	interval_set<DomainT,IntervT,CompareT,AllocT> clone; toItvSetT(clone);
#endif
	*/

	base_type::subtract(x);

	// ON_DEBUG(clone.subtract(x);)
	// J_ASSERT(isEqual(clone))
}


template <typename DomainT, typename CodomainT, typename IntervT, typename CompareT, typename AllocT>
void joint_interval_map<DomainT,CodomainT,IntervT,CompareT,AllocT>::handle_neighbours(const iterator& it)
{
	DEV_ASSERT(_set.contains(*it));
	IntervT itv = (*it).KEY_VALUE;

	if(it == _map.begin())
	{
		ImplMapT::iterator it_nxt=it; it_nxt++;
		if(it_nxt!=_map.end() && itv.touches((*it_nxt).KEY_VALUE)) 
			joint_insert(it, it_nxt);
	}
	else
	{
		// there is a predecessor
		iterator it_pred = it; it_pred-- ;

		if((*it_pred).KEY_VALUE.touches(itv)) 
		{
			iterator it_extended = joint_insert(it_pred, it);

			iterator it_succ=it_extended; it_succ++;
			if(it_succ!=_map.end())
			{
				// it's a non border element that might have two touching neighbours
				if((*it_extended).KEY_VALUE.touches((*it_succ).KEY_VALUE)) 
					joint_insert(it_extended, it_succ);
			}
		}
		else
		{
			iterator it_succ=it; it_succ++;
			if(it_succ!=_map.end())
			{
				// it's a non border element that might have a right touching neighbours
				if(itv.touches((*it_succ).KEY_VALUE)) 
					joint_insert(it, it_succ);
			}
		}
	}
}


template <typename DomainT, typename CodomainT, typename IntervT, typename CompareT, typename AllocT>
	typename joint_interval_map<DomainT,CodomainT,IntervT,CompareT,AllocT>::iterator 
joint_interval_map<DomainT,CodomainT,IntervT,CompareT,AllocT>
	::joint_insert(const iterator& left_it, const iterator& right_it)
{
	// both left and right are in the set and they are neighbours
	DEV_ASSERT((*left_it).KEY_VALUE.excl_less((*right_it).KEY_VALUE));
	DEV_ASSERT((*left_it).KEY_VALUE.touches((*right_it).KEY_VALUE));

	IntervT curItv = (*left_it).KEY_VALUE;
	curItv.extend((*right_it).KEY_VALUE);

	CodomainT covalue = (*left_it).CONT_VALUE;
	covalue += (*right_it).CONT_VALUE;

	_map.erase(left_it);
	_map.erase(right_it);

	ImplMapT::iterator new_it = _map.insert(value_type(curItv, covalue)).ITERATOR;
	J_ASSERT(new_it!=_map.end());
	return new_it;
}


} // namespace itl

#endif


