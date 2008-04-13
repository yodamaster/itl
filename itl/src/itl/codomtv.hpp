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
Dieses File dient ausschlie�lich der Dokumentation von Template-Parametern
durch doc++. (Dokumentations-Typ)
Es soll und mu� niemals in Projekte includiert werden! 
--------------------------------------------------------------------*/

#pragma message(__LOC__"Don't include this file (it served to document template patameter only)!")

namespace itl
{

/// defines the signature for the template value parameter of SplitItvMapT
/**  
    class <b>CodomTV</b> defines the signature (functions and operations)
    that are demanded for a value class of the interval container SplitItvT.
    These are: Default-ctor, equality ==, += and -=.
    
    @author  Joachim Faulhaber
*/

class CodomTV
{
public:
    /// Liefert einen Wert; in der Regel einen Nullwert oder Leerwert
    CodomTV();

    /** Gleichheits Operator

        Die Operation <tt>==</tt> mu� korrekt implementiert werden. F�r atomare
        Datentypen und viele Klassen gibt bereits eine Implementierung dieser
        Operation.
    */
    bool operator == (const CodomTV& y, const CodomTV& y)const;

    /** Additionsoperator mit der Eigenschaft <tt>x + y - y == x</tt>

        Die Operation <tt>+</tt> mu� nicht explizit implementiert sein. 
        Sie mu�    nur in ihrer <tt>+=</tt> Variante zur Verf�gung stehen.
    */
    CodomTV& operator += (const CodomTV& y);

    /** Subtraktionsoperator mit der Eigenschaft <tt>x + y - y == x</tt>

        Die Operation <tt>-</tt> mu� nicht explizit implementiert sein.
        Sie mu�    nur in ihrer <tt>-=</tt> Variante zur Verf�gung stehen.
          Die Eigenschaften, die <tt>+= und -=</tt> erf�llen m�ssen, lauten genauer 
        
        <tt>x = x1; x+=y; x-=y; impliziert x==x1 (1)</tt>

        <tt>x = x1; x+=CodomTV(); impliziert x==x1 (2)</tt>

        <tt>x = x1; x-=CodomTV(); impliziert x==x1 (3)</tt>

        Die Umkehrung von (1) <tt>x - y + y == x</tt> ist nicht gefordert. Dies w�rde z.B.
        von sets nicht erf�llt. Der Defaultkonstruktor CodomTV() liefert
        ein neutrales Element bez�glich + und -
    */
    CodomTV& operator -= (const CodomTV& y);

} ;

} // namespace itl