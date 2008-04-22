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
#ifndef __itl_prefix_set_JOFA_040902_H__
#define __itl_prefix_set_JOFA_040902_H__

#include <itl/notate.hpp>
#include <itl/string_set.hpp>


namespace itl
{

    template <typename StringT>
    struct PrefixLess : public binary_function<StringT, StringT, bool>
    {
        // Wenn einer von lhs & rhs ein echter Prefix des anderen ist, ist er kleiner.
        // Ansonsten lexikografischer Vergleich
        bool operator() (const StringT& lhs, const StringT& rhs)const;
    };

	/// represents a set of strings by means of prefixes
    /** Ein prefix_set repraesentiert eine Menge von Strings mit Hilfe von
        Prefixen.

        Zum Beispiel repraesentiert das 
        prefix_set {Ha} die Menge {Hans, Hallo, Harald, ...} also alle Strings, 
        die mit 'Ha' beginnen.
        
        Entsprechend
        {Har, Hal}.contains(Haribo) == true
        {Har, Hal}.contains(Hallenbad) == true
        {Har, Hal}.contains(Hugo) == false

        Einfuegen eines gemeinsamen Praefix, einer PraefixSetT verkleinert es:
        {Har, Hal}.insert(Ha) == {Ha}
        
        PrefixSetTs sind sehr praktisch fuer die Behandlung unserer Gruppierungen
        von Kategoriestrings aus den Cfg-Files, die ja mittels Prefix-Listen 
        ausgedrueckt werden.

        Hierfuer gibt es noch die Klasse StringPatternSetT, die die ensprechenden
            prefix_set m_Includes;
            prefix_set m_Excludes;
        enthaelt.        
    */
    class prefix_set
    {
    public:
        typedef string_set<PrefixLess> PrefixSetTD;
        typedef PrefixSetTD::iterator iterator;
        typedef PrefixSetTD::const_iterator const_iterator;

    public:
        static int compareStrings(int& comSize, const char* s1, const char* s2);

    public:
        void insert(const std::string& val);

        const_iterator find(const std::string& val)const { return m_.find(val); }
        bool contains(const std::string& val)const { return m_.find(val)!=m_.end();    }

        std::string as_string()const { return m_.as_string(); }

    private:
        string_set<PrefixLess> m_;
    };



    template <typename StringT>
    bool PrefixLess<StringT>::operator() (const StringT& lhs, const StringT& rhs)const
    {
        int commonPrefixLength;
        int compareResult 
            = prefix_set::compareStrings(commonPrefixLength, lhs.c_str(), rhs.c_str());

        if(compareResult == 0)      // Gleichheit
            return false;
        if(commonPrefixLength == lhs.size())
            return true;            // lhs istEchterPrefix rhs
        // OTHERWISE
        return compareResult == -1; 
    }

}

#endif // __itl_prefix_set_JOFA_040902_H__

