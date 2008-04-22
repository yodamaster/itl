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

#ifndef __var_tuple_JOFA_040614_H__
#define __var_tuple_JOFA_040614_H__

#include <itl/notate.hpp>
#include <itl/fixtupelconst.hpp>
#include <string>
#include <sstream>

namespace itl
{
	/// tuple of independent variables 
    /** class var_tuple: Die Tupel unabh�ngiger Variablen. Beschreiben
        die Zellen eines n-dimensionalen Z�hlw�rfels

        Zust�ndigkeit: Zellen eines n-dimensionalen Z�hlw�rfels beschreiben. Als
        Index f�r Merkmalskombinationsrechner dienen. (Um)Ordnen, Projizieren und
        Gruppieren unterst�tzen.

        Zusammenarbeit: Order, Projection, Grouping, Merkmalskombinationsrecher

        FastVarTupel ist schnell (Fast), da es als einfaches array[int] implemetiert wird
    */
    template <int varCountV>
    class var_tuple
    {
    public:
        enum { var_count = varCountV };
    public:
        /// Default Ctor. Achtung absichtlich keine Initialisierung!
        var_tuple(){};
        /// Ctor initialisiere alle Elemente des Tupels auf einen Wert
        var_tuple(StatVarTD);

        /// Copy Ctor
        var_tuple(const var_tuple&);

        /// Assignment
        var_tuple& operator = (const var_tuple&);

        /// Gr��e des Tupels
        int size()const { return varCountV; }

        /// Gleichheit
        bool operator == (const var_tuple&)const;

        /// Standard-Ordnung
        bool operator < (const var_tuple&)const;

        /// Lesender Zugriff auf Elemente
        StatVarTD  operator [] (int idx)const { return m_tupel[idx]; }
        /// Modifizierender Zugriff
        StatVarTD& operator [] (int idx) { return m_tupel[idx]; }

        ///
        std::string as_string()const;

    private:
        StatVarTD m_tupel[varCountV];
        
    };


    template <int varCountV>
    itl::var_tuple<varCountV>::var_tuple (StatVarTD val) // vor VC8: itl::var_tuple<varCountV>::var_tuple<varCountV>(StatVarTD val)
    {
        FOREACH_VAR(idx) 
            m_tupel[idx] = val;
    }

    template <int varCountV>
    itl::var_tuple<varCountV>::var_tuple (const var_tuple<varCountV>& src)
    {
        FOREACH_VAR(idx)
            m_tupel[idx] = src.m_tupel[idx];
    }

    template <int varCountV>
    var_tuple<varCountV>& var_tuple<varCountV>::operator = (const var_tuple<varCountV>& src)
    {
        if(&src != this)
        {
            FOREACH_VAR(idx)
                m_tupel[idx] = src.m_tupel[idx];            
        }    
        return *this;
    }


    template <int varCountV>
    bool var_tuple<varCountV>::operator == (const var_tuple<varCountV>& rhs)const
    {
        for(int idx=0; idx < varCountV; idx++)
            if(m_tupel[idx] != rhs.m_tupel[idx]) return false;

        return true;
    }


    template <int varCountV>
    bool var_tuple<varCountV>::operator < (const var_tuple<varCountV>& rhs)const
    {
        FOREACH_VAR(idx)
            if(m_tupel[idx] < rhs.m_tupel[idx]) return true;
            else if (m_tupel[idx] > rhs.m_tupel[idx]) return false;

        // because all are equal
        return false;
    }

    template <int varCountV>
    std::string var_tuple<varCountV>::as_string()const
    {
        std::stringstream repr;
        repr << "(";
        for(int idx = 0; idx < varCountV-1; idx++)
            repr << m_tupel[idx] << ",";

        if(varCountV==0) repr << ")";
        else repr << m_tupel[varCountV-1] << ")";

        return repr.str();
    }


} // namespace itl



#endif // __var_tuple_JOFA_040614_H__


