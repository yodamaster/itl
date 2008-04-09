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
class maps
	a general and generic set interface
--------------------------------------------------------------------*/
#ifndef __maps_h_JOFA_990223__
#define __maps_h_JOFA_990223__

#include <map>
#include <itl/ctxpred.hpp>

namespace itl
{

/**  
	Template interface-class <b>maps</b> defines a general interface
	for maps.

	Template-parameter <b>DomainT</b>: Domain-type or key type of the map.
	
	Template-parameter <b>CodomainT</b>: Codomain-typ or data type of the map.
	
	@author  Joachim Faulhaber
*/
template <class DomainT, class CodomainT> class maps
{
public:
	/// base-type of the map; that is (key,value)-pairs 
	typedef std::pair<DomainT,CodomainT> base_value_type ;

	/// Remove all elements of the map
	virtual void clear()=0;

	/// Is the map empty?
	virtual bool empty()const=0;

	/// Does the map contain an element for key x
	virtual bool contains(const DomainT& x)const=0;

	/// Does the map contain a pair(x,y), where x is a key and y a value
	virtual bool contains(const base_value_type& x_y)const=0;

	/// insert a pair(x,y) into the map
	virtual void insert  (const base_value_type& x_y)=0;

	/// remove a pair(x,y) from the map
	virtual void subtract(const base_value_type& x_y)=0;

} ;

} // namespace itl

#endif

