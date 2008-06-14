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
class split_interval_map
--------------------------------------------------------------------*/
#ifndef __split_interval_map_h_JOFA_000706__
#define __split_interval_map_h_JOFA_000706__

#include <itl/interval_set.hpp>
#include <itl/interval_map_splitter.hpp>
#include <itl/interval_base_map.hpp>
#include <itl/split_interval_set.hpp>

namespace itl
{

    /// implements a map as a map of intervals - on insertion overlapping intervals are split and associated values are combined.
    /** 
        Template-class <b>split_interval_map</b>
        implements a map as a map of intervals - On insertion overlapping intervals are
        <b>split</b> and associated values are combined.
       
        Template parameter <b>DomainT</b>: Domain type of the map. Also type of the
        map's keys.

          Suitable as domain types are all datatypes that posess a partial order.
        In particular all discrete atomic datatypes like <tt>int, short, long</tt> and
        atomic pseudo-continuous datatypes <tt>float, double</tt> may be instantiated.

        Datatypes for the codomain parameter have to <b>implement</b> operations
        <tt>+=</tt>, <tt>-=</tt>, <tt>==</tt> (equality) and <tt>CodomainT()</tt> (default constructor).

        The default constructor <tt>CodomainT()</tt> has to contruct a neutral element
        such that the following holds:

          If <tt>x = y; y += CodomainT();</tt> then <tt>x==y;</tt> and
          If <tt>x = y; y -= CodomainT();</tt> then <tt>x==y;</tt>

          Template parameter <b>Interval=itl::interval</b>: Template type of interval used
        to implement the map. The default <b>itl::interval</b> uses the
        interval class template that comes with this library. Own implementation of interval
        classes are possible (but not trivial).

        <b>split_interval_map</b> implements a map <tt>map<DomainT, CodomainT></tt> as a map
        of intervals <tt>map<interval<DomainT>, CodomainT, ExclusiveLessT<Interval> ></tt>

        Interval maps <tt>split_interval_map<DomainT,CodomainT></tt> can be used similar (and in many
        aspects exactly like) common stl-maps. Unlike to stl-maps where you store
        a value for every key an interval map stores a contents value for an interval of
        keys. In it's degenerated form the key intervals contain a single element
        only. Then the interval map works like a normal stl-map. But if you work in problem
        domains where you associate values to large contiguous intervals, interval maps
        are very useful and efficient.


        Class <tt>interval_base_map</tt>  yields the following benefits:
      
        <ul>
            <li> A set of intervals is conceived as the domain set of the map. 
                The complexity involved with
                operations on intervals maps is encapsulated. The user of the class who
                wants to perform operations on interval maps is no more concerned
                with questions of overlapping, joining and bordering intervals.
            <li>
                <b>split_interval_map</b> gives an efficient implementation of maps consisting
                of larger contiguous chunks. Very large, even uncountably infinite maps
                can be represented in a compact way and handled efficiently.
            <li>
                <b>split_interval_map</b> serves as a overlay- or collision-computer.
        </ul>

        <b>split_interval_map as overlay computer</b>

        An object <tt>split_interval_map<int,int> overlays;</tt> computes the overlays or 
        collisions of intervalls which have been inserted into it, if they are
        associated with the <tt>int</tt>-value <tt>1</tt> as the codommain value.

        If a newly inserted interval overlaps with intervals which are already in the
        map, the interval is split up at the borders of the collisions. The assiciated
        values of the overlapping intervals are incremented by 1, thus counting
        the numbers of overlaps.

        If sets are used as codomain types, interval_maps will compute unions of
        associated maps in case of interval collisions.

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
        typename DomainT,
        typename CodomainT,
        template<class>class Interval = itl::interval,
        template<class>class Compare  = std::less,
        template<class>class Alloc    = std::allocator
    >
    class split_interval_map:
		public interval_base_map<itl::interval_map_splitter, DomainT,CodomainT,Interval,Compare,Alloc>
    {
    public:
        typedef split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc> type;
        typedef interval_base_map <itl::interval_map_splitter, 
                                   DomainT,CodomainT,Interval,Compare,Alloc> base_type;

        //PORT: The following types should be intereted from the base class
        // which does work with mscv++ but gcc complaines
        typedef Interval<DomainT> interval_type;
        typedef typename base_type::iterator iterator;
        typedef typename base_type::value_type value_type;
        typedef typename base_type::ImplMapT ImplMapT;

        typedef interval_set<DomainT,Interval,Compare,Alloc> interval_set_type;

        /// Default constructor for the empty map 
        split_interval_map(): base_type() {}
        /// Copy constructor
        split_interval_map(const split_interval_map& src): base_type(src) {}
    } ;


    template <class KeyT, class DataT>
    class type<itl::split_interval_map<KeyT,DataT> >
    {
    public:
        static std::string to_string()
        { 
            return "splt_itv_map<"+ type<KeyT>::to_string() + "," + 
                                    type<DataT>::to_string() +">"; 
        }
    };

} // namespace itl

#endif


