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
#ifndef __itl_set_algo_H_JOFA_990225__
#define __itl_set_algo_H_JOFA_990225__

#include <itl/notate.hpp>
#include <itl/itl_type.hpp>
#include <itl/predicates.hpp>

/*
<b>SetAlgo </b>
SetAlgo is a collection of algorithms that work with sets but may also
used with other set-implementations like e.g. hash_set.
*/

namespace itl
{

    namespace Set
    {

        template<class ObjectT, class ConstObjectT, class IteratorT>
        bool common_range(IteratorT& lwb, IteratorT& upb, ObjectT& x1, const ConstObjectT& x2)
        {
            // lwb and upb are iterator of x1 marking the lower and upper bound of
            // the common range of x1 and x2.
            typedef typename ConstObjectT::const_iterator ConstObject_iterator;

            lwb = x1.end();
            upb = x1.end();

            if(x1.empty() || x2.empty()) 
                return false;

            IteratorT x1_fst_ = x1.begin();
            IteratorT x1_lst_ = x1.end(); x1_lst_--;

            ConstObject_iterator x2_fst_ = x2.begin();
            ConstObject_iterator x2_lst_ = x2.end(); x2_lst_--;

            typename ObjectT::key_compare key_less;

            if(key_less(ObjectT::key_value(x1_lst_), ConstObjectT::key_value(x2_fst_))) // {x1}   {x2}
                return false;
            if(key_less(ConstObjectT::key_value(x2_lst_), ObjectT::key_value(x1_fst_))) // {x2}   {x1} 
                return false;

            // We do have a common range
            lwb = x1.lower_bound(ConstObjectT::key_value(x2_fst_));
            upb = x1.upper_bound(ConstObjectT::key_value(x2_lst_));

            return true;
        }

        //JODO where to put common algorithms? namespace Collector, Ordered, Sorted, SortedObject

        template<class ObjectT>
        void inject(ObjectT& result, const ObjectT& x2)
        {
            typedef typename ObjectT::const_iterator Object_const_iterator;
            for(Object_const_iterator x2_ = x2.begin(); x2_ != x2.end(); x2_++)
                result.inject(*x2_);
        }

        template<class ObjectT, class CoObjectT>
        void subtract(ObjectT& result, const CoObjectT& x2) //JODO TEST
        {
            typename CoObjectT::const_iterator common_lwb_;
            typename CoObjectT::const_iterator common_upb_;
            if(!common_range(common_lwb_, common_upb_, x2, result))
                return;

            typename CoObjectT::const_iterator x2_ = common_lwb_;
            typename ObjectT::iterator common_;

            while(x2_ != common_upb_)
                result.subtract(*x2_++);
        }



        /** Function template <tt>contained_in</tt> implements the subset relation. 
        <tt>contained_in(sub, super)</tt> is true if <tt>sub</tt> is contained in <tt>super</tt> */
        template<class SetType>
        bool contained_in(const SetType& sub, const SetType& super)
        {
            if(&super == &sub)                   return true;
            if(sub.empty())                      return true;
            if(super.empty())                    return false;
            if(*sub.begin()    < *super.begin()) return false;
            if(*super.rbegin() < *sub.rbegin() ) return false;

            typename SetType::const_iterator common_lwb_;
            typename SetType::const_iterator common_upb_;
            if(!common_range(common_lwb_, common_upb_, sub, super))
                return false;

            typename SetType::const_iterator sub_ = common_lwb_, super_;
            while(sub_ != common_upb_)
            {
                super_ = super.find(*sub_++);
                if(super_ == super.end()) 
                    return false;
            }
            return true;
        }

        /** Function template <tt>lexicographical_equal</tt> implements 
        lexicographical equality. */
        template<class SetType>
        bool lexicographical_equal(const SetType& left, const SetType& right)
        {
            if(&left == &right)        return true;
            if(left.element_count() != right.element_count()) 
                return false;

            // so we have two sorted containers with equal element counts
            typename SetType::const_iterator left_  = left.begin();
            typename SetType::const_iterator right_ = right.begin();

            while(left_ != left.end())
            {
                if(!(*left_==*right_))
                    return false;
            
                ++left_; ++right_;
            }

            return true;
        }


        /** */
        template<class SetType>
        void intersect(SetType& result, const SetType& x1, const SetType& x2)
        {
            typename SetType::const_iterator common_lwb_;
            typename SetType::const_iterator common_upb_;

            result.clear();
            if(!common_range(common_lwb_, common_upb_, x1, x2))
                return;

            typename SetType::const_iterator x1_ = common_lwb_, x2_;

            while(x1_ != common_upb_)
            {
                x2_ = x2.find(*x1_++);
                if(x2_ != x2.end())
                    result.insert(*x2_);
            }
        }

        template<class SetType>
        void intersect(SetType& result, const SetType& x2)
        {
            SetType tmp;
            intersect(tmp, result, x2);
            tmp.swap(result);
        }


        template<class SetType>
        bool disjoint(const SetType& x1, const SetType& x2)
        {
            typename SetType::const_iterator common_lwb_;
            typename SetType::const_iterator common_upb_;

            if(!common_range(common_lwb_, common_upb_, x1, x2))
                return true;

            typename SetType::const_iterator x1_ = common_lwb_, x2_;

            while(x1_ != common_upb_)
            {
                x2_ = x2.find(*x1_++);
                if(x2_ != x2.end()) return false; // found a common element
            }
            // found no common element
            return true;    
        }

        template<class SetType>
        void subtract(SetType& result, const SetType& x1, const SetType& x2)
        {
            SetType temp;
            typename SetType::const_iterator x1_ = x1.begin(), x2_;

            if(&x1 != &x2)
                while(x1_ != x1.end())
                {
                    x2_ = x2.find(*x1_);
                    if(x2_ == x2.end())
                        temp.insert(*x1_);
                    ++x1_;
                }
                temp.swap(result);
        }

    } // namespace Set

} // namespace itl

#endif

