/*----------------------------------------------------------------------------+
Copyright (c) 2007-2008: Joachim Faulhaber
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
#ifndef __itl_MAPALGO_H_JOFA_080225__
#define __itl_MAPALGO_H_JOFA_080225__

#include <itl/notate.hpp>
#include <itl/set_algo.hpp>

namespace itl
{
    namespace Map 
    {
        template<class MapType>
        bool contained_in(const MapType& sub, const MapType& super)
        {
            if(&super == &sub)                   return true;
            if(sub.empty())                      return true;
            if(super.empty())                    return false;
            if(super.size()    < sub.size()    ) return false;
            if(*sub.begin()    < *super.begin()) return false;
            if(*super.rbegin() < *sub.rbegin() ) return false;

            typename MapType::const_iterator common_lwb_;
            typename MapType::const_iterator common_upb_;
            if(!Set::common_range(common_lwb_, common_upb_, sub, super))
                return false;

            typename MapType::const_iterator sub_ = sub.begin(), super_;
            while(sub_ != sub.end())
            {
                super_ = super.find((*sub_).KEY_VALUE);
                if(super_ == super.end()) 
                    return false;
                else if(!(sub_->CONT_VALUE == super_->CONT_VALUE))
                    return false;
                sub_++;
            }
            return true;
        }

        template<class MapType>
        void intersection(MapType& y, const MapType& x1, const MapType& x2)
        {
            MapType tmp;
            typename MapType::const_iterator i1 = x1.begin(), i2;

            while(i1 != x1.end())
            {
                i2 = x2.find(i1->first);
                if(i2 != x2.end())
                {
                    tmp += *i1; 
                    if(is_set<typename MapType::codomain_type>::value)
                        tmp *= *i2;
                    else
                        tmp += *i2;
                }
                i1++;
            }
            tmp.swap(y);
        }

        // optimized version
        template<class MapType>
        void intersect(MapType& result, const MapType& x1, const MapType& x2)
        {
            typename MapType::const_iterator common_lwb_;
            typename MapType::const_iterator common_upb_;

            result.clear();
            if(!Set::common_range(common_lwb_, common_upb_, x1, x2))
                return;

            typename MapType::const_iterator x1_ = common_lwb_, x2_;

            while(x1_ != common_upb_)
            {
                x2_ = x2.find((*x1_).KEY_VALUE);
                if(x2_ != x2.end())
                {
                    result.insert(*x1_);
                    if(is_set<typename MapType::data_type>::value)
                        result.template add<inplace_star>(*x2_); //MEMO template cast for gcc
                    else
                        result.template add<inplace_plus>(*x2_);
                        //result.template add<inplace_identity>(*x2_);
                }
                x1_++;
            }
        }

        template<class MapType>
        void intersect(MapType& result, const MapType& x2)
        {
            // result = result * x2;
            MapType tmp;
            intersect(tmp, result, x2);
            tmp.swap(result);
        }

        // optimized version
        template<class MapType, class SetType>
        void intersect(MapType& result, const MapType& x1, const SetType& x2)
        {
            typename MapType::const_iterator common_lwb_;
            typename MapType::const_iterator common_upb_;

            result.clear();
            if(!Set::common_range(common_lwb_, common_upb_, x1, x2))
                return;

            typename MapType::const_iterator x1_ = common_lwb_;
            typename SetType::const_iterator common_;

            while(x1_ != common_upb_)
            {
                common_ = x2.find((*x1_).KEY_VALUE);
                if(common_ != x2.end())
                    result.insert(*x1_);

                x1_++;
            }
        }

        template<class MapType, class SetType>
        void intersect(MapType& result, const SetType& x2) //JODO TEST
        {
            // result = result * x2;
            MapType tmp;
            intersect(tmp, result, x2);
            tmp.swap(result);
        }

    } // namespace Map

} // namespace itl

#endif

