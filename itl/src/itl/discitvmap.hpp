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
class DiscItvMapT
--------------------------------------------------------------------*/
#ifndef __DISCITVMAP_H_JOFA_000706__
#define __DISCITVMAP_H_JOFA_000706__

#include <itl/itvmapat.hpp>
#include <itl/itvset.hpp>

namespace itl
{

template <class ItvDomTV, class CodomTV, class ItvTV=IntervalT<ItvDomTV> > class DiscItvMapT:
	public  ItvMapAT<ItvDomTV,CodomTV,ItvTV>
{
public:
	DiscItvMapT(): ItvMapAT<ItvDomTV,CodomTV,ItvTV>() {}
	DiscItvMapT(const DiscItvMapT& src): ItvMapAT<ItvDomTV,CodomTV,ItvTV>(src) {}

	virtual bool contains(const value_type& x)const;

	virtual void insert(const value_type&);
	virtual void subtract(const value_type& x);

	virtual void handleNeighbours(const iterator& it){}

	void toItvSetT(ItvSetT<ItvDomTV, ItvTV>& dst)const
	{ dst.clear(); const_FORALL(ImplMapT, it, m_map) dst.insert(*it); }

} ;



template <class ItvDomTV, class CodomTV, class ItvTV>
bool DiscItvMapT<ItvDomTV,CodomTV,ItvTV>::contains(const value_type& x_y)const
{
	ItvTV x = x_y.KEY_VALUE;
	if(x.empty()) return true;

	ImplMapT::const_iterator fst_it = m_map.lower_bound(x);
	ImplMapT::const_iterator end_it = m_map.upper_bound(x);

	ItvSetT<ItvDomTV,ItvTV> matchSet;
	for(ImplMapT::const_iterator it=fst_it; it!=end_it; it++) 
		matchSet.insert((*it).KEY_VALUE);

	ItvSetT<ItvDomTV,ItvTV> x_asSet; x_asSet.insert(x);
	return x_asSet.isSubsetOf(matchSet);
}



template <class ItvDomTV, class CodomTV, class ItvTV>
void DiscItvMapT<ItvDomTV,CodomTV,ItvTV>::insert(const value_type& x)
{
	/*
#ifdef _DEBUG
	ItvSetT<ItvDomTV,ItvTV> clone; toItvSetT(clone);
#endif
	*/

	ItvMapAT<ItvDomTV,CodomTV,ItvTV>::insert(x);

	// ON_DEBUG(clone.insert(x);)
	// J_ASSERT(isEqual(clone))
}


template <class ItvDomTV, class CodomTV, class ItvTV>
void DiscItvMapT<ItvDomTV,CodomTV,ItvTV>::subtract(const value_type& x)
{
	/*
#ifdef _DEBUG
	ItvSetT<ItvDomTV,ItvTV> clone; toItvSetT(clone);
#endif
	*/

	ItvMapAT<ItvDomTV,CodomTV,ItvTV>::subtract(x);

	// ON_DEBUG(clone.subtract(x);)
	// J_ASSERT(isEqual(clone))
}

} // namespace itl

#endif


