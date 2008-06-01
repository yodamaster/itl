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
class MapGentorT
    A random generator for Maps.
    ContainerGentorT does not cover maps, because its
    value_type contains const parts so void some(value_type& x)
    can NOT be implemented 
--------------------------------------------------------------------*/
#ifndef __MAPGENTOR_H_JOFA_000724__
#define __MAPGENTOR_H_JOFA_000724__

#include <itl/itl_list.hpp>
#include <itl/gentorit.hpp>

namespace itl
{

template <class MapTV> 
class MapGentorT: public RandomGentorAT<MapTV>
{
public:
    typedef typename MapTV::value_type    ValueTypeTD;
    typedef typename MapTV::key_type    DomainTD;
    typedef typename MapTV::data_type    CodomainTD;
    typedef list<ValueTypeTD>    SampleTypeTD;

    MapGentorT(): p_domainGentor(NULL), p_codomainGentor(NULL) {}
    ~MapGentorT() { delete p_domainGentor; delete p_codomainGentor; }

    virtual void some(MapTV& x);
    void last(MapTV& x)const;
    void last_permuted(MapTV& x)const;

    void setDomainGentor(RandomGentorAT<DomainTD>* gentor)
    { 
        if(p_domainGentor)
            delete p_domainGentor;
        p_domainGentor = gentor; 
    }
    void setCodomainGentor(RandomGentorAT<CodomainTD>* gentor)
    { 
        if(p_codomainGentor)
            delete p_codomainGentor;
        p_codomainGentor = gentor; 
    }

    void setRangeOfSampleSize(int lwb, int upb)
    { m_sampleSizeRange = rightopen_interval(lwb,upb); }
    void setRangeOfSampleSize(const interval<int>& szRange)
    { J_ASSERT(szRange.is_rightopen()); m_sampleSizeRange = szRange; }

private:
    RandomGentorAT<DomainTD>*        p_domainGentor;
    RandomGentorAT<CodomainTD>*        p_codomainGentor;
    interval<int>                    m_sampleSizeRange;
    SampleTypeTD                    m_sample;
    int                                m_sampleSize;
};


template <class MapTV> 
void MapGentorT<MapTV>::some(MapTV& x)
{
    NumberGentorT<int> intGentor;
    x.clear();
    m_sample.clear();
    m_sampleSize = intGentor(m_sampleSizeRange);

    for(int i=0; i<m_sampleSize; i++)
    {
        DomainTD key;
        p_domainGentor->some(key);
        CodomainTD val;
        p_codomainGentor->some(val);
        x += ValueTypeTD(key,val);
        m_sample.push_back(ValueTypeTD(key,val));
    }
}


template <class MapTV> 
void MapGentorT<MapTV>::last(MapTV& x)const
{
    x.clear();
    const_FORALL(typename SampleTypeTD, it, m_sample) x.insert(*it);
}

template <class MapTV>
void MapGentorT<MapTV>::last_permuted(MapTV& x)const
{
    x.clear();

    SampleTypeTD perm;

    NumberGentorT<int> intGentor;
    const_FORALL(typename SampleTypeTD, it, m_sample)
    {
        if( 0==intGentor(2) ) perm.push_back(*it);
        else perm.push_front(*it);
    }

    const_FORALL(typename SampleTypeTD, pit, perm) x.insert(*pit);
}


/*
template <class MapTV> 
void MapGentorT<MapTV>::lastSample(SampleTypeTD& sam)const
{ sam = m_sample; }


template <class MapTV> 
void MapGentorT<MapTV>::lastSample_permuted(SampleTypeTD& sam)
{
    NumberGentorT<unsigned> intGentor;
    x.clear();
    int coin = intGentor.some(2); // gives 0 or 1
    const_FORALL(typename SampleTypeTD, it, m_sample)
    {
        if( 0==intGentor.some(2) ) sam.push_back(*it);
        else sam.push_front(*it);
    }
}
*/

} // namespace itl

#endif


