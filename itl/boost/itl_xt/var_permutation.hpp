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
#ifndef __var_permutation_JOFA_040621_H__
#define __var_permutation_JOFA_040621_H__

#include <string.h>
#include <itl_xt/fixtupelconst.hpp>
#include <itl_xt/itl_list.hpp>

namespace itl
{

    template <int varCountV>
    class var_permutation
    {
    public:
        typedef itl::list<VarEnumTD> ListTD;
    public:
        var_permutation(): m_Size(0) {}
        var_permutation(const var_permutation&);
        var_permutation& operator = (const var_permutation&);

        /// Hinten anfuegen
        var_permutation& add(VarEnumTD var);
        /// Von hinten 'count' Elemente loeschen
        var_permutation& del(int count = 1);

        int size()const { return m_Size; }
        void setIdentity();
        var_permutation& clear() { m_Size = 0; return *this; }

        /** Liefert zum Index 'permIdx' die unabh�ngige Variable var=permutation[permIdx].
        Da es sich um eine schnelle Primitive handelt, wird der G�ltigkeitsbereich des 
        Arguments 0 <= permIdx <m_Size nicht gepr�ft.        */
        VarEnumTD operator[] (VarEnumTD permIdx)const { return m_Permutation[permIdx]; }

        /** Liefert zum Index 'permIdx' die unabh�ngige Variable var=permutation[permIdx].
        Liefert -1, wenn permIdx kein g�ltiger Index ist. */
        VarEnumTD getVar(VarEnumTD permIdx)
        { return (0 <= permIdx && permIdx < m_Size) ? m_Permutation[permIdx] : UNDEFINED_INDEX; }

        var_permutation operator() (VarEnumTD fst, VarEnumTD lst)const;

        /** Index zur Variable 'var' finden. Liefert -1, wenn 'var' nicht gefunden. 
        index=find(var) ist hierbei der Index 0 <= index < m_Size der Permutation die den
        Wert der unabh�ngigen Variable 'var' enth�lt. */
        int find(VarEnumTD var)const;

        /// Ist 'var' enthalten?
        bool contains(VarEnumTD var)const { return 0 <= find(var); }

        /// Fuege 'var' an der Stelle 'pos' ein. Liefert false, wenn insert scheitert.
        bool insert(VarEnumTD var, int pos);

        /// Das Element mit dem Wert 'val' loeschen
        var_permutation& remove(VarEnumTD val);

        /** 'perm' ist eine Permutation von 'seq', so dass perm eine Untersequenz 
        von *this ist, in der keine Vertauschungen der Reihenfolge von *this vorkommen. 
        'perm' ist vertauschungsfreie Untersequenz von *this.
        Voraussetzung ist, dass die Elemente von 'seq' in *this enthalten sind.    */
        void consequent_permutation(var_permutation& perm, const var_permutation& seq)const;

        /** Liefert den reduzierten Aggregationsgrad einer Untersequenz 'subSeq'.
        'subSeq' hat natuerlich weniger Aggregationsgrade, als *this. Wir tun so,
        als waeren alle Elemente von *this ausgeblendet. Jedes Element, das in
        *this ausgebelendet ist, wird von grade abgezogen.
        */
        int gradeReduct(int grade, const var_permutation& subSeq)const; 


        ListTD asList()const;
        std::string as_string()const;

    private:
        VarEnumTD m_Permutation[varCountV];
        int          m_Size;
    };

    template <int varCountV>
    var_permutation<varCountV>::var_permutation (const var_permutation<varCountV>& src)
    {
        m_Size = src.m_Size;
        FOREACH_VAR_TO(idx, m_Size)
            m_Permutation[idx] = src.m_Permutation[idx];
    }

    template <int varCountV>
    var_permutation<varCountV>& var_permutation<varCountV>::operator = (const var_permutation<varCountV>& src)
    {
        if(&src != this)
        {
            m_Size = src.m_Size;
            FOREACH_VAR_TO(idx, m_Size)
                m_Permutation[idx] = src.m_Permutation[idx];
        }
        return *this;
    }


    template <int varCountV>
    void var_permutation<varCountV>::setIdentity()
    {
        FOREACH_VAR(idx)
            m_Permutation[idx] = idx;
        m_Size = varCountV;
    }

    template <int varCountV>
    var_permutation<varCountV>& var_permutation<varCountV>::add(VarEnumTD var) 
    {
        if(m_Size < varCountV)
        {
            m_Permutation[m_Size] = var; 
            m_Size++; 
        }
        return *this;
    }

    template <int varCountV>
    var_permutation<varCountV>& var_permutation<varCountV>::del(int count) // = 1 default 
    {
        int back = std::min(count, m_Size);
        m_Size -= back;
        return *this;
    }

    template <int varCountV>
    var_permutation<varCountV> var_permutation<varCountV>::operator() (VarEnumTD fst, VarEnumTD lst)const
    {
        var_permutation perm;
        for(VarEnumTD idx = fst; idx < lst; idx++)
            perm.add((*this)[idx]);

        return perm;
    }

    template <int varCountV>
    var_permutation<varCountV>& var_permutation<varCountV>::remove(VarEnumTD val)
    {
        int doomedIdx = find(val);
        if(doomedIdx == UNDEFINED_INDEX)
            return *this;

        for(int idx=doomedIdx; idx < (m_Size-1); idx++)
            m_Permutation[idx] = m_Permutation[idx+1];

        m_Size--;

        return *this;
    }

    template <int varCountV>
    int var_permutation<varCountV>::find(VarEnumTD val)const
    {
        int hit = UNDEFINED_INDEX;
        for(int idx=0; idx<m_Size; idx++)
            if(m_Permutation[idx]==val)
                return idx;

        return hit;
    }


    template <int varCountV>
    bool var_permutation<varCountV>::insert(VarEnumTD var, int pos)
    {
        //JODO URG untested
        J_ASSERT2(!contains(var), "var_permutation has to be unique");
        if(varCountV <= var || varCountV == m_Size)
            return false;

        // Alle nach rechts schaufeln
        for(int idx=pos; idx < m_Size; idx++)
            m_Permutation[idx+1] = m_Permutation[idx];

        m_Permutation[pos] = var;
    }

    template <int varCountV>
    std::string var_permutation<varCountV>::as_string()const
    {
        std::string repr = "[";
        int idx = 0;
        if(m_Size>0)
            repr += value<VarEnumTD>::to_string(m_Permutation[idx++]);

        while(idx<m_Size)
            repr += value<VarEnumTD>::to_string(m_Permutation[idx++]);

        repr += "]";

        return repr;
    }


    template <int varCountV>
    typename itl::var_permutation<varCountV>::ListTD var_permutation<varCountV>::asList()const
    {
        ListTD seq;
        int idx = 0;
        while(idx < m_Size)
            seq.push_back(m_Permutation[idx++]);
        return seq;
    }


    template <int varCountV>
    void var_permutation<varCountV>::consequent_permutation(var_permutation& perm, const var_permutation& seq)const
    {
        ListTD master = asList(),
            conseq, 
            unseq = seq.asList(); 

        master.consequent_permutation(conseq, unseq);

        perm.clear();
        const_FORALL(ListTD, it_, conseq)
            perm.add(*it_);
    }

    template <int varCountV>
    int var_permutation<varCountV>::gradeReduct(int grade, const var_permutation& subSeq)const
    {
        // subSeq ist echte Untersequenz von *this.
        if(grade==0)
            return 0;
        int subIdx = 0;
        for(int varIdx = 0; varIdx < size(); varIdx++)
        {
            if(subSeq[subIdx] == (*this)[varIdx])
                subIdx++;
            if(varIdx+1 == grade)
                return subIdx;
        }
    }


} // namespace itl

#endif // __var_permutation_JOFA_040621_H__


