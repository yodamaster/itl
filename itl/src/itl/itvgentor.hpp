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
class ItvGentorT
--------------------------------------------------------------------*/
#ifndef __ITVGENTOR_H_JOFA_000714__
#define __ITVGENTOR_H_JOFA_000714__


#include <itl/gentorit.hpp>
#include <itl/numbergentor.hpp>
#include <itl/seqgentor.hpp>

namespace itl
{

template <class ItvDomTV, class ItvTV=interval<ItvDomTV> > 
class ItvGentorT: public RandomGentorAT<ItvTV>
{
public:
    virtual void some(ItvTV& x);

    void setValueRange(ItvDomTV low, ItvDomTV up)
    { m_valueRange.set(low,up, ItvTV::RIGHT_OPEN); }

    void setMaxIntervalLength(ItvDomTV len) { m_maxIntervalLength=len; }
    void setProbDerivation();


private:
    NumberGentorT<ItvDomTV> m_ItvDomTVGentor;

    interval<ItvDomTV> m_valueRange;
    ItvDomTV              m_maxIntervalLength;
};


template <class ItvDomTV, class ItvTV>
void ItvGentorT<ItvDomTV,ItvTV>::some(ItvTV& x)
{
    NumberGentorT<unsigned> NATGentor;
    ItvDomTV x1   = m_ItvDomTVGentor(m_valueRange);
    ITV_BOUNDTYPES bndTypes = NATGentor(4);
    unsigned upOrDown = NATGentor(1);
    unsigned decideEmpty = NATGentor(2);

    if(decideEmpty==0)
    {        
        ItvDomTV x2   = m_ItvDomTVGentor(m_valueRange);
        x.set(x1, x1-x2, bndTypes); //JODO this should be done smarter
    }
    else if(upOrDown==0) {
        ItvDomTV up 
            = m_ItvDomTVGentor(x1, static_cast<ItvDomTV>(std::min(m_valueRange.upper(), x1+m_maxIntervalLength)));
        x.set(x1, up, bndTypes);
    } else {
        ItvDomTV low 
            = m_ItvDomTVGentor(static_cast<ItvDomTV>(std::max(m_valueRange.lower(), x1-m_maxIntervalLength)), x1);
        x.set(low, x1, bndTypes);
    }
};

} // namespace itl

#endif


