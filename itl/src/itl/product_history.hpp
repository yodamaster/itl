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
#ifndef __product_history_h_JOFA_011005_H__
#define __product_history_h_JOFA_011005_H__


#include <itl/split_interval_map.hpp>

namespace itl
{

/// composes many episodes of different types into a single history of episode_product s
/**    
    <b>Eine Klasse zur Verarbeitung von Episoden unterschiedlichen Typs in einer gemeinsamen Historie</b>

    Template-Parameter <b>ItvDomTV</b>: Domain-Typ der History, also Typ der
    Elemente des Definitionsbereichs (Schl�ssel), die die History enthalten kann
    (z.B. Tage, Sekunden, Monate, int u.�.).

    Template-Parameter <b>TypeDomTV</b>: TypeDomTV ist der Definitionsbereich (Domain)
    der Typen von Episoden, die in der Produkt-Historie verwendet werden k�nnen.
    
    TypeDomTV kapselt im wesentlichen einen Aufz�hlungstyp, der die Typen
    von Episoden definiert, die m�glich sind (Z.B. Aufenthalte, Kosten�bernahmen und
    Bettenbelegungen).

    product_history wird ohne weiere Umschweife direkt aus einem \ref split_interval_map
    abgeleitet.

    Die Werte dieser Historie sind eine (Auf)sammlung oder Produkt von Episoden
    verschiedenen Typs \ref episode_product. Die Historie solcher
    Episodensammlungen teilt sich immer dort auf, wo irgend ein Unterschied
    in den Episoden eintritt.

    Mit anderen Worten die ProduktHistorie enth�lt zusammenh�ngende Intervalle,
    wo die Anzahl und Werte gesammelter Episonden konstant sind.

    Dies ist ein universelles Werkzeug, um Mengen asynchroner Historien
    zusammenzufassen und die feinste Darstellung als eine Historie zu
    erhalten, die alle 'Zustands�nderungen' ihrer Teilhistorien repr�sentiert.

    { H<T1>, .. , H<Ti> }  -> H<{T1, .. , Ti}>
    
    @author  Joachim Faulhaber
*/
template <class ItvDomTV, class TypeDomTV>
class product_history : public split_interval_map<ItvDomTV, episode_product<ItvDomTV,TypeDomTV> >
{
public:
    typedef                episode_product<ItvDomTV,TypeDomTV> EpisodeProductTD;
    typedef                split_interval_map<ItvDomTV, EpisodeProductTD> BaseTypeTD;
    typedef    typename    BaseTypeTD::value_type value_type;
    typedef    typename    BaseTypeTD::interval_type IntervalTD;
    typedef    typename    BaseTypeTD::domain_type DomainTD;
    
    typedef    typename    BaseTypeTD::data_type data_type;
    typedef    typename    EpisodeProductTD::EpisodePTD EpisodePTD;
    
public:

    void insert(EpisodePTD epi)
    {
        EpisodeProductTD epiRec;
        epiRec.insert(epi);
        BaseTypeTD::insert(value_type(epi->interval(), epiRec));
    }

protected:
    
    void insert(const value_type& val)
    {
        BaseTypeTD::insert(val);
    }
};

} // namespace itl

#endif // __product_history_h_JOFA_011005_H__


