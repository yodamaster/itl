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
#ifndef __itl_MAPALGO_H_JOFA_990225__
#define __itl_MAPALGO_H_JOFA_990225__

#include <itl/notate.hpp>
//CL #include <itl/itl_algo.hpp>
#include <itl/set_algo.hpp>

namespace itl
{
	namespace Map 
	{
		template<class mapT>
		bool contained_in(const mapT& sub, const mapT& super)
		{
			if(&super == &sub)                   return true;
			if(sub.empty())                      return true;
			if(super.empty())                    return false;
			if(super.size()    < sub.size()    ) return false;
			if(*sub.begin()    < *super.begin()) return false;
			if(*super.rbegin() < *sub.rbegin() ) return false;

			typename mapT::const_iterator common_lwb_;
			typename mapT::const_iterator common_upb_;
			if(!Set::common_range(common_lwb_, common_upb_, sub, super))
				return false;

			typename mapT::const_iterator sub_ = sub.begin(), super_;
			while(sub_ != sub.end())
			{
				super_ = super.find((*sub_).KEY_VALUE);
				if(super_ == super.end()) 
					return false;
				else if((*sub_).CONT_VALUE != (*super_).CONT_VALUE)
					return false;
				sub_++;
			}
			return true;
		}

		//CL
		//template<class mapT>
		//void inject(mapT& result, const mapT& x2)
		//{
		//	if(&result != &x2)
		//		const_FORALL(mapT, x2_, x2)
		//			result.inject(*x2_);
		//	else
		//		FORALL(mapT, it, result)	
		//			(*it).CONT_VALUE += (*it).CONT_VALUE;
		//}

		//JODO map_intersection: es gibt mehrer M�glichkeiten der Implementierung
		// map/insert; accu_map/inject=insert
		template<class mapT>
		void intersection(mapT& y, const mapT& x1, const mapT& x2)
		{
			mapT tmp;
			typename mapT::const_iterator i1 = x1.begin(), i2;

			while(i1 != x1.end())
			{
				i2 = x2.find(i1->first);
				if(i2 != x2.end())
				{
					//JODO this is for collector and counter
					tmp.insert(*i1);
					tmp.inject(*i2); //JODO insert/inject
				}
				i1++;
			}
			tmp.swap(y);
		}

		// optimized version
		template<class mapT>
		void intersect(mapT& result, const mapT& x1, const mapT& x2)
		{
			typename mapT::const_iterator common_lwb_;
			typename mapT::const_iterator common_upb_;

			result.clear();
			if(!Set::common_range(common_lwb_, common_upb_, x1, x2))
				return;

			typename mapT::const_iterator x1_ = common_lwb_, x2_;

			while(x1_ != common_upb_)
			{
				x2_ = x2.find((*x1_).KEY_VALUE);
				if(x2_ != x2.end())
				{
					result.insert(*x1_);
					result.inject(*x2_);
				}
				x1_++;
			}
		}

		template<class mapT>
		void intersect(mapT& result, const mapT& x2)
		{
			// result = result * x2;
			mapT tmp;
			intersect(tmp, result, x2);
			tmp.swap(result);
		}

		// optimized version
		template<class mapT, class setT>
		void intersect(mapT& result, const mapT& x1, const setT& x2)
		{
			typename mapT::const_iterator common_lwb_;
			typename mapT::const_iterator common_upb_;

			result.clear();
			if(!Set::common_range(common_lwb_, common_upb_, x1, x2))
				return;

			typename mapT::const_iterator x1_ = common_lwb_;
			typename setT::const_iterator common_;

			while(x1_ != common_upb_)
			{
				common_ = x2.find((*x1_).KEY_VALUE);
				if(common_ != x2.end())
					result.insert(*x1_);

				x1_++;
			}
		}

		template<class mapT, class setT>
		void intersect(mapT& result, const setT& x2) //JODO TEST
		{
			// result = result * x2;
			mapT tmp;
			intersect(tmp, result, x2);
			tmp.swap(result);
		}

	} // namespace Map

} // namespace itl

#endif
