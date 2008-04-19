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

          Template parameter <b>interval_type=interval<DomainT></b>: Type of interval used
        to implement the set. The default <b>interval<DomainT></b> uses the
        interval class that comes with this library. Own implementation of interval
        classes are possible (but not trivial).

        <b>split_interval_map</b> implements a map <tt>MapT<DomainT, CodomainT></tt> as a map
        of intervals <tt>MapT<interval<DomainT>, CodomainT, ExclusiveLessT<IntervT> ></tt>

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
    class split_interval_map: public interval_base_map<DomainT,CodomainT,Interval,Compare,Alloc>
    {
    public:
        typedef split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc> type;
        typedef interval_base_map <DomainT,CodomainT,Interval,Compare,Alloc> base_type;

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

        virtual base_type* cons()const
        { return new split_interval_map(); }

        virtual bool contains(const value_type& x)const;

        virtual void insert(const value_type&);
        virtual void subtract(const value_type&);

        virtual void handle_neighbours(const iterator& it){}

        void toItvSetT(interval_set<DomainT,Interval,Compare,Alloc>& dst)const
        { dst.clear(); const_FORALL(typename ImplMapT, it, this->_map) dst.insert((*it).KEY_VALUE); }

        
        //TESTCODE
        void getResiduals(const interval_type& x_itv, interval_type& leftResid, interval_type& rightResid);

        //JODO make private after tests
        // If an associated value x is inserted into a SplitItvMap, the actual splitting takes place
        // only at the borders of the interval of x. All the other modifications of the SplitItvMap
        // can be done via inplace value modifications of associated intervals in the map or by simple
        // insertions of associated intervals.
        void splitFor(const interval_type& x_itv, iterator& lwb_it, iterator& upb_it);

    private:

        void niftyInsert(const value_type&);
        void niftySubtract(const value_type&);

        void recInsert(const value_type&);
        void recSubtract(const value_type&);

        void insertRest(const interval_type& x_itv, const CodomainT& x_val, iterator& it, iterator& end_it);
        void subtractRest(const interval_type& x_itv, const CodomainT& x_val, iterator& it, iterator& end_it);

    } ;




    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    bool split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::contains(const value_type& x_y)const
    {
        interval_type x = x_y.KEY_VALUE;
        if(x.empty()) return true;

        split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc> match_map;
        matchMap(match_map, x_y);

        if(match_map.interval_count() != 1) return false;

        iterator match_it = match_map._map.find(x);
        if(! x.contained_in((*match_it).KEY_VALUE) ) return false;

        return (*match_it).CONT_VALUE==x_y.CONT_VALUE; //CodomainT::OP ==
    }

    //----------------------------------------------------------------------
    // Splits *this split_interval_map M at the borders of interval x as follows.
    //
    // M: ... [--------)  ...  [------) ...
    //        vi               vj
    // x:          [---------------)
    // -->
    // M':... [---)[---)  ...  [---)[-) ...
    //        vi   vi     ...  vj   vj
    //----------------------------------------------------------------------
    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    void split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::splitFor(const interval_type& x_itv, iterator& fst_it, iterator& end_it)
    {
        fst_it = this->_map.lower_bound(x_itv);
        end_it = this->_map.upper_bound(x_itv);
        iterator lst_it;

        interval_type fst_itv, lst_itv;
        interval_type outerLeft, outerRight;   // Partial intervals of M which are outside x
        interval_type innerLeft, innerRight;   // Partial intervals of M which are inside x
        CodomainT leftVal, rightVal;      // Associated values vi and vj

        if(fst_it!=end_it)
        {
            fst_itv = (*fst_it).KEY_VALUE;
            fst_itv.left_surplus(outerLeft, x_itv);

            lst_it = end_it; lst_it--;
            lst_itv = (*lst_it).KEY_VALUE;
            lst_itv.right_surplus(outerRight, x_itv);
        }

        if(!outerLeft.empty())
        {
            // Split the associated Interval Mi into M'i1 and M'i2
            fst_itv.intersect(innerLeft, x_itv);
            leftVal = (*fst_it).CONT_VALUE;
            this->_map.erase(fst_it);
            this->_map.insert(value_type(outerLeft, leftVal));
            this->_map.insert(value_type(innerLeft, leftVal));

            // refresh lower_bound
            fst_it = this->_map.lower_bound(x_itv);
        }
        if(!outerRight.empty())
        {
            // Split the associated Interval Mj into M'j1 and M'j2
            lst_itv.intersect(innerRight, x_itv);

            if(!(innerLeft == innerRight))
            {
                rightVal = (*lst_it).CONT_VALUE;
                this->_map.erase(lst_it);
                this->_map.insert(value_type(innerRight, rightVal));
                this->_map.insert(value_type(outerRight, rightVal));
            }
            else
            {
                this->_map.insert(value_type(outerRight, leftVal));
            }

            // refresh upper bound
            end_it = this->_map.upper_bound(x_itv);
        }

        // After associated values are saved, we may erase the splitted intervals from the map
        // CAUTION Will erase fst_it and insert outerLeft, innerLeft invalidate lst_it and vice versa?
        // Not it should not, according to docu    
    }


    //MEMO INFO -------------------------------------------------------------------
    // niftyInsert and niftySubtract are iterative versions of recursive functions
    // recInsert and recSubtract. Since recInsert and recSubtract are one of the
    // most expensive functions (according to profile run) I expected the
    // iterative versions to speed up performance. As it turned out this is not
    // so, the iterative niftyVersions were even slightly slower.
    //   niftyInsert/Subtract should still be kept. (1) Their implementation is
    // more concise and better to understand. (2) May be it could be speeded up
    // by another attempt JODO 
    //-----------------------------------------------------------------------------
    // After splitFor(x, l, u) we have the following
    //
    // x:           [-----------------------------------)
    //               w
    // M:  ... [---)[---)     [--------)       ... [----)[--) ...
    //          vi   vi     ...v(i+1)               vj    vj
    // -->
    //     ... [---)[---)[===)[--------)[====) ... [----)[--) ...
    //          vi   vi+w w    v(i+1)+w  w      ... vj+w  vj
    //
    // The intersection intervals of x and M all get a value of v(i+k)+w
    // The complementary intervals within the range of x are getting value w
    // All the other intervals of M are of course unchanged
    //-----------------------------------------------------------------------------
    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    void split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::niftyInsert(const value_type& x)
    {
        typedef split_interval_set<DomainT,Interval,Compare,Alloc> AuxSetTD;
        const interval_type&   x_itv = x.KEY_VALUE;
        // JODO Must be a copy ??
        const CodomainT& x_val = x.CONT_VALUE; //CodomainT::OP =

        if(x_itv.empty()) return;
        if(x_val==CodomainT()) return; //CodomainT::OP CodomainT; CodomainT::OP ==

        std::pair<typename ImplMapT::iterator,bool> insertion = this->_map.insert(x);

        if(!insertion.WAS_SUCCESSFUL)
        {
            iterator lwb_, upb_, shared_, victim_;
            splitFor(x_itv, lwb_, upb_);

            AuxSetTD intersect, complement(x_itv);

            shared_=lwb_; 
            while(shared_ != upb_)
            {
                intersect.insert((*shared_).KEY_VALUE);
                if(CodomainT() == ((*shared_).CONT_VALUE += x_val))
                {
                    victim_ = shared_;
                    shared_++;
                    this->_map.erase(victim_);
                }
                else
                    shared_++;
            }

            complement -= intersect;

            FORALL(typename AuxSetTD, cmpl_, complement)
            {
                this->_map.insert(value_type(*cmpl_, x_val));
            }
        }
    }

    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    void split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::niftySubtract(const value_type& x)
    {
        typedef split_interval_set<DomainT,Interval,Compare,Alloc> AuxSetTD;
        const interval_type&   x_itv = x.KEY_VALUE;
        const CodomainT& x_val = x.CONT_VALUE; //CodomainT::OP =

        if(x_itv.empty()) return;
        if(x_val==CodomainT()) return; //CodomainT::OP CodomainT; CodomainT::OP ==

        std::pair<typename ImplMapT::iterator,bool> insertion = this->_map.insert(x);

        if(!insertion.WAS_SUCCESSFUL)
        {
            iterator lwb_, upb_, shared_, victim_;
            splitFor(x_itv, lwb_, upb_);

            AuxSetTD intersect, complement(x_itv);

            shared_=lwb_; 
            while(shared_ != upb_)
            {
                intersect.insert((*shared_).KEY_VALUE);
                if(CodomainT() == ((*shared_).CONT_VALUE -= x_val))
                {
                    victim_ = shared_;
                    shared_++;
                    this->_map.erase(victim_);
                }
                else
                    shared_++;
            }

            complement -= intersect;

            CodomainT insert_val = CodomainT();
            insert_val -= x_val;
            FORALL(typename AuxSetTD, cmpl_, complement)
            {
                this->_map.insert(value_type(*cmpl_, insert_val));
            }
        }
    }


    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    void split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::getResiduals(const interval_type& x_itv, interval_type& leftResid, interval_type& rightResid)
    {
        iterator fst_it = this->_map.lower_bound(x_itv);
        iterator end_it = this->_map.upper_bound(x_itv);
        
        if(fst_it==end_it)
        {
            leftResid.clear();
            rightResid.clear();
            return;
        }

        (*fst_it).KEY_VALUE.left_surplus(leftResid, x_itv);
        iterator lst_it = fst_it; lst_it++;

        if(lst_it==end_it)
        {
            rightResid.clear();
            return;
        }

        lst_it=end_it; lst_it--;

        (*lst_it).KEY_VALUE.right_surplus(rightResid, x_itv);
    }


    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    void split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::insert(const value_type& x)
    //{ niftyInsert(x); }
    { recInsert(x); }


    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    void split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::recInsert(const value_type& x)
    {
        const interval_type& x_itv = x.KEY_VALUE;
         // must be a copy
        CodomainT      x_val = x.CONT_VALUE; //CodomainT::OP =

        if(x_itv.empty()) return;
        if(x_val==CodomainT()) return; //CodomainT::OP CodomainT; CodomainT::OP ==

        std::pair<typename ImplMapT::iterator,bool> insertion = this->_map.insert(x);

        if(!insertion.WAS_SUCCESSFUL)
        {

            iterator fst_it = this->_map.lower_bound(x_itv);
            iterator end_it = this->_map.upper_bound(x_itv);

            iterator cur_it = fst_it ;
            interval_type cur_itv   = (*cur_it).KEY_VALUE ;
            CodomainT cur_val = (*cur_it).CONT_VALUE ; //CodomainT::OP =


            interval_type leadGap; x_itv.left_surplus(leadGap, cur_itv);
            // this is a new Interval that is a gap in the current map
            recInsert(value_type(leadGap, x_val));

            // only for the first there can be a leftResid: a part of *it left of x
            interval_type leftResid;  cur_itv.left_surplus(leftResid, x_itv);

            // handle special case for first

            interval_type interSec;
            cur_itv.intersect(interSec, x_itv);

            CodomainT cmb_val = cur_val;
            cmb_val += x_val;

            iterator snd_it = fst_it; snd_it++;
            if(snd_it == end_it) 
            {
                // first == last

                interval_type endGap; x_itv.right_surplus(endGap, cur_itv);
                // this is a new Interval that is a gap in the current map
                recInsert(value_type(endGap, x_val));

                // only for the last there can be a rightResid: a part of *it right of x
                interval_type rightResid;  (*cur_it).KEY_VALUE.right_surplus(rightResid, x_itv);

                this->_map.erase(cur_it);
                recInsert(value_type(leftResid,  cur_val));
                recInsert(value_type(interSec,   cmb_val));
                recInsert(value_type(rightResid, cur_val));
            }
            else
            {
                this->_map.erase(cur_it);
                recInsert(value_type(leftResid, cur_val));
                recInsert(value_type(interSec,  cmb_val));

                // shrink interval
                interval_type x_rest(x_itv);
                x_rest.left_subtract(cur_itv);

                insertRest(x_rest, x_val, snd_it, end_it);
            }
        }
    }


    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    void split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::insertRest(const interval_type& x_itv, const CodomainT& x_val, iterator& it, iterator& end_it)
    {
        iterator nxt_it = it; nxt_it++;

        interval_type   cur_itv = (*it).KEY_VALUE ;
        CodomainT cur_val = (*it).CONT_VALUE ;
        
        interval_type newGap; x_itv.left_surplus(newGap, cur_itv);
        // this is a new Interval that is a gap in the current map
        recInsert(value_type(newGap, x_val));

        interval_type interSec;
        cur_itv.intersect(interSec, x_itv);

        CodomainT cmb_val = cur_val;
        cmb_val += x_val;

        if(nxt_it==end_it)
        {
            interval_type endGap; x_itv.right_surplus(endGap, cur_itv);
            // this is a new Interval that is a gap in the current map
            recInsert(value_type(endGap, x_val));

            // only for the last there can be a rightResid: a part of *it right of x
            interval_type rightResid;  cur_itv.right_surplus(rightResid, x_itv);

            this->_map.erase(it);
            recInsert(value_type(interSec,   cmb_val));
            recInsert(value_type(rightResid, cur_val));
        }
        else
        {        
            this->_map.erase(it);
            recInsert(value_type(interSec,   cmb_val));

            // shrink interval
            interval_type x_rest(x_itv);
            x_rest.left_subtract(cur_itv);

            insertRest(x_rest, x_val, nxt_it, end_it);
        }
    }



    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    void split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::subtract(const value_type& x)
    {
        //niftySubtract(x);
        recSubtract(x);
    }



    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    void split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::recSubtract(const value_type& x)
    {
        const interval_type&   x_itv = x.KEY_VALUE;
        const CodomainT& x_val = x.CONT_VALUE;

        if(x_itv.empty()) return;
        if(x_val==CodomainT()) return;  //CodomainT::OP CodomainT; CodomainT::OP ==

        iterator fst_it = this->_map.lower_bound(x_itv);
        if(fst_it==this->_map.end()) return;
        iterator end_it = this->_map.upper_bound(x_itv);
        if(fst_it==end_it) return;

        iterator cur_it = fst_it ;
        interval_type cur_itv   = (*cur_it).KEY_VALUE ;
        // must be copies because cur_it will be erased
        CodomainT cur_val = (*cur_it).CONT_VALUE ; //CodomainT::OP = 

        // only for the first there can be a leftResid: a part of *it left of x
        interval_type leftResid;  cur_itv.left_surplus(leftResid, x_itv);

        // handle special case for first

        interval_type interSec;
        cur_itv.intersect(interSec, x_itv);

        CodomainT cmb_val = cur_val; //CodomainT::OP =
        cmb_val -= x_val;          //CodomainT::OP -=

        iterator snd_it = fst_it; snd_it++;
        if(snd_it == end_it) 
        {
            // first == last
            // only for the last there can be a rightResid: a part of *it right of x
            interval_type rightResid;  (*cur_it).KEY_VALUE.right_surplus(rightResid, x_itv);

            this->_map.erase(cur_it);
            insert(value_type(leftResid,  cur_val));
            if(!(cmb_val==CodomainT())) //CodomainT::OP CodomainT; CodomainT::OP ==
                insert(value_type(interSec, cmb_val));
            insert(value_type(rightResid, cur_val));
        }
        else
        {
            // first AND NOT last
            this->_map.erase(cur_it);
            insert(value_type(leftResid, cur_val));
            if(!(cmb_val==CodomainT())) //CodomainT::OP CodomainT; CodomainT::OP ==
                insert(value_type(interSec, cmb_val));

            subtractRest(x_itv, x_val, snd_it, end_it);
        }
    }



    template <typename DomainT, typename CodomainT, template<class>class Interval, template<class>class Compare, template<class>class Alloc>
    void split_interval_map<DomainT,CodomainT,Interval,Compare,Alloc>::subtractRest(const interval_type& x_itv, const CodomainT& x_val, iterator& snd_it, iterator& end_it)
    {
        iterator it=snd_it, nxt_it=snd_it; nxt_it++;

        while(nxt_it!=end_it)
        {
            CodomainT& cur_val = (*it).CONT_VALUE ;

            cur_val -= x_val; //CodomainT::OP -=

            if(cur_val==CodomainT()) //CodomainT::OP CodomainT; CodomainT::OP == 
            { iterator victim; victim=it; it++; this->_map.erase(victim); }
            else it++;
            nxt_it=it; nxt_it++;
        }

        // it refers the last overlaying intervals of x_itv
        const interval_type&  cur_itv = (*it).KEY_VALUE ;

        interval_type rightResid; cur_itv.right_surplus(rightResid, x_itv);

        if(rightResid.empty())
        {
            CodomainT& cur_val = (*it).CONT_VALUE ;
            cur_val -= x_val;      //CodomainT::OP -=
            if(cur_val==CodomainT()) //CodomainT::OP CodomainT; CodomainT::OP ==
                this->_map.erase(it);
        }
        else
        {
            CodomainT cur_val = (*it).CONT_VALUE ;
            CodomainT cmb_val = cur_val ; //CodomainT::OP =
            cmb_val -= x_val;           //CodomainT::OP -=
            interval_type interSec; cur_itv.intersect(interSec, x_itv);

            this->_map.erase(it);
            if(!(cmb_val==CodomainT())) //CodomainT::OP CodomainT; CodomainT::OP ==
                insert(value_type(interSec, cmb_val));
            insert(value_type(rightResid, cur_val));
        }

    }

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


