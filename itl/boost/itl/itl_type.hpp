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


/*-----------------------------------------------------------------------------
Function-templates for discrete Datatypes like int, unsigned or
    any class that provides a ++ operator c.f. iterators
-----------------------------------------------------------------------------*/

#ifndef __itl_TYPE_JOFA_080416_H__
#define __itl_TYPE_JOFA_080416_H__

#include <stdio.h>
#include <string>
#include <sstream>

#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/mpl/if.hpp>

namespace itl
{
	/*CL
    template <class IncrementableT>
    inline static IncrementableT succ(IncrementableT x) { return ++x; }

    template <class DecrementableT>
    inline static DecrementableT pred(DecrementableT x) { return --x; }
	*/

    // ------------------------------------------------------------------------
    template<class Type>
    struct type
    {
        /** Null or empty value of a datatype.

            Neutral element of a datatype with respect to a basic operation 
            <tt>+</tt>.    This commonly is a null or empty value which is best 
            associated with the default constructor in most cases.
        */
        static Type neutron();

        /** One-value or singleton-value of a datatype. 

            Neutral element of a datatype with respect to an operation <tt>*</tt>.
            This is a value that denotes 'oneness'. For many c++ classes such
            a value is not available as denotatin. We need the <tt>type<T>::unon()</tt> to
            denote an empty Interval independent of it's domain-type <tt>T</tt> as
            <tt>IntervalT<T>( type<T>::unon(), type<T>::neutron() )</tt>

            Using <tt>type<T>::unon()</tt> one can assign one-values to a
            template parameter T.

            \code
            template <class T> class A {
                void f() { T t; t = type<T>::unon(); -- dependent on the instance of T
                                                     -- t=1; t=1.0; t=SetT(x);
            }
            \endcode
        */
        //CL static Type unon();

        /** Is this type atomic (built in)? <tt>(bool, char, int, etc.)</tt>

            E.g.: <tt>if(type<T>::is_atomic()) -- T is atomic</tt>
        */
        //Cl static bool is_atomic();

        /** Is this type continuous ? <tt>(float, double, etc.)</tt> 

            E.g.: <tt>if(type<T>::is_continuous()) -- T is continuous</tt>
        */
        static bool is_continuous();

        static bool is_set();

        static bool is_interval_container();

        static bool is_interval_splitter();

        static bool is_neutron_emitter();

        static bool is_neutron_absorber();


        /** Represent the type by a string which is it's typename */
        static std::string to_string();
    };

    template <class Type>
    inline Type type<Type>::neutron() 
    {
        return Type(); 
    }

	//JODO THINK: 1.0 can always be expressed via ++T(), can't it?
    //template<> inline float  type<float>::unon()  { return 1.0; }
    //template<> inline double type<double>::unon() { return 1.0; }
    //template<> inline std::string type<std::string>::unon() { return std::string(" "); }


    //CL template <class Type>
    //inline Type type<Type>::unon() 
    //{ 
    //    return succ(type<Type>::neutron()); 
    //}


    //template<> inline bool type<bool>::is_atomic() { return true; }
    //template<> inline bool type<char>::is_atomic() { return true; }

    //template<> inline bool type<short>::is_atomic() { return true; }
    //template<> inline bool type<int>::is_atomic() { return true; }
    //template<> inline bool type<long>::is_atomic() { return true; }

    //template<> inline bool type<unsigned char>::is_atomic() { return true; }
    //template<> inline bool type<unsigned short>::is_atomic() { return true; }
    //template<> inline bool type<unsigned int>::is_atomic() { return true; }
    //template<> inline bool type<unsigned long>::is_atomic() { return true; }

    //template<> inline bool type<float>::is_atomic() { return true; }
    //template<> inline bool type<double>::is_atomic() { return true; }

    //template<> inline bool type<char*>::is_atomic() { return true; }

    //template <class Type>
    //inline bool type<Type>::is_atomic() { return false; }


    template<> inline bool type<float>::is_continuous() { return true; }
    template<> inline bool type<double>::is_continuous() { return true; }
    template<> inline bool type<std::string>::is_continuous() { return true; }

    template <class Type>
    inline bool type<Type>::is_continuous() { return false; }

    //-------------------------------------------------------------------------

    template <class Type>
    inline bool type<Type>::is_set() { return false; }

    //-------------------------------------------------------------------------
    template <class Type>
    inline bool type<Type>::is_interval_container() { return false; }

    template <class Type>
    inline bool type<Type>::is_interval_splitter() { return false; }

    template <class Type>
    inline bool type<Type>::is_neutron_emitter() { return false; }

    template <class Type>
    inline bool type<Type>::is_neutron_absorber() { return false; }

    //-------------------------------------------------------------------------
    template<>
    inline std::string type<int>::to_string() { return "int"; }
    template<>
    inline std::string type<double>::to_string() { return "double"; }
    template<>
    inline std::string type<std::string>::to_string() { return "string"; }

    //-------------------------------------------------------------------------
    template<template<class> class Templ>
    struct unary_template
    {
        static std::string to_string();
    };

    template <template<class>class Unary, class Type>
    struct type<Unary<Type> >
    {
        static std::string to_string()
        { 
            return unary_template<Unary>::to_string()+"<"+type<Type>::to_string()+">"; 
        }
    };

    // ---------------------------------------------------------------------------
    template<template<class,class>class Templ>
    struct binary_template
    {
        static std::string to_string();
    };

    template <template<class Type1, class Type2>class Binary, class Type1, class Type2>
    struct type<Binary<Type1, Type2> >
    {
        static std::string to_string()
        { 
            return binary_template<Binary>::to_string()+
                "<"+type<Type1>::to_string()+","+type<Type2>::to_string()+">"; 
        }
    };

    // ---------------------------------------------------------------------------
    //KEEP only currently unused
    //template<template<class,class,class>class Templ>
    //struct ternary_template
    //{
    //    static std::string to_string();
    //};

    //template <template<class Type1, class Type2, class Type3>class Ternary, 
    //          class Type1, class Type2, class Type3>
    //struct type<Ternary<Type1, Type2, Type3> >
    //{
    //    static std::string to_string()
    //    { 
    //        return ternary_template<Ternary>::to_string()+
    //            "<"+type<Type1>::to_string()+","+type<Type2>::to_string()+","+type<Type3>::to_string()+">"; 
    //    }
    //};

} // namespace itl

#endif


