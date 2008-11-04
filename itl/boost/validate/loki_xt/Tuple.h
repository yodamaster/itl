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
// This code is based on and inspired by designs of:
// The Loki Library by Andrei Alexandrescu et al.
// See:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// and:
// www.sourceforge.net/projects/loki
//
// Parts of the Loki Library have been used for the Itl Library.
// Those parts from the path <itl/loki> can also be included from the 
// original Loki Library sources.

#ifndef __Loki_tuple_h_JOFA_070604__
#define __Loki_tuple_h_JOFA_070604__

#include <boost/validate/loki/Typelist.h>
#include <boost/validate/loki/TypeTraits.h>
#include <boost/validate/loki/EmptyType.h>
#include <boost/validate/loki_xt/TypelistGentor.h>


namespace Loki
{
#if defined(_MSC_VER) && _MSC_VER >= 1300
#pragma warning( push ) 
 // 'class1' : base-class 'class2' is already a base-class of 'class3'
#pragma warning( disable : 4584 )
#endif // _MSC_VER

    // forward
    template <class TargetT, class SrcT> struct Sizer;

    namespace tup
    {
        template <typename FstT, typename SndT> struct TypePair
        {
            typedef FstT FirstT;
            typedef SndT SecondT;
        };

        // --------------------------------------------------------------------------
        // template class TupleElement
        // --------------------------------------------------------------------------
        template <typename Type, unsigned int index>
        struct TupleElement 
        {
            typedef Type ValueType;
            enum { position = index };
            Type _value;
            operator Type&() { return _value; }
            operator const Type&() const { return _value; }
            void set(Type& value){ _value = value; }
        };

        // --------------------------------------------------------------------------
        // template class TupleGentor
        // --------------------------------------------------------------------------
        template <class TList, unsigned int index>
        struct TupleGentor;

        template <class HeadT, class TailT, unsigned int index>
        struct TupleGentor<Typelist<HeadT, TailT>, index>
            : public TupleElement<HeadT, index>
            , public TupleGentor<TailT, index+1>
        {
            typedef Typelist<HeadT, TailT>         TList;
            typedef TupleElement<HeadT, index>  HeadClass;
            typedef TupleGentor<TailT, index+1> TailClass;
        };

        template <unsigned int size>
        struct TupleGentor<NullType, size>
        {
            typedef NullType TList;
        };
        // --------------------------------------------------------------------------

        // ---------------------------------------------------------------------------
        // template class RefGetter
        // ---------------------------------------------------------------------------
        template <class TupleT, unsigned int idx> struct RefGetter;

        template <class TupleT>
        struct RefGetter<TupleT, 0>
        {
            typedef typename TupleT::HeadClass HeadClass;
            typedef typename HeadClass::ValueType ValueType;

            static ValueType& Do(TupleT& obj)
            {
                HeadClass& elem = obj;
                return elem;
            }
        };

        template <class TupleT, unsigned int idx>
        struct RefGetter
        {
            typedef typename TupleT::HeadClass HeadClass;
            typedef typename TupleT::TailClass TailClass;
            typedef typename TL::TypeAt<typename TupleT::TList, idx>::Result ValueType;

            static ValueType& Do(TupleT& obj)
            {
                TailClass& rightBase = obj;
                return RefGetter<TailClass, idx-1>::Do(rightBase);
            }
        };

        template <int i, class TupleT>
        typename RefGetter<TupleT, i>::ValueType& refer(TupleT& obj)
        {
            return RefGetter<TupleT, i>::Do(obj);
        }
        // ---------------------------------------------------------------------------

        // ---------------------------------------------------------------------------
        // template class ValueGetter
        // ---------------------------------------------------------------------------
        template <class TupleT, unsigned int idx> struct ValueGetter;

        template <class TupleT>
        struct ValueGetter<TupleT, 0>
        {
            typedef typename TupleT::HeadClass HeadClass;
            typedef typename HeadClass::ValueType ValueType;

            static ValueType Do(const TupleT& obj)
            {
                const HeadClass& elem = obj;
                return elem;
            }
        };

        template <class TupleT, unsigned int idx>
        struct ValueGetter
        {
            typedef typename TupleT::HeadClass HeadClass;
            typedef typename TupleT::TailClass TailClass;
            typedef typename TL::TypeAt<typename TupleT::TList, idx>::Result ValueType;

            static ValueType Do(const TupleT& obj)
            {
                const TailClass& rightBase = obj;
                return ValueGetter<TailClass, idx-1>::Do(rightBase);
            }
        };

        template <int idx, class TupleT>
        typename ValueGetter<TupleT, idx>::ValueType get(const TupleT& obj)
        {
            return ValueGetter<TupleT, idx>::Do(obj);
        }
        // ---------------------------------------------------------------------------

        
        
        // ---------------------------------------------------------------------------
        // template class Mapper
        // ---------------------------------------------------------------------------
        template <template<typename,typename>class Function, class TrgTupleT, class SrcTupleT>
        struct Mapper;

        template <template<typename,typename>class Function, typename TrgType, typename SrcType, unsigned int index>
        struct Mapper<Function, 
                      TupleGentor<Typelist<TrgType, NullType>, index>,
                      TupleGentor<Typelist<SrcType, NullType>, index> >
        {
            typedef TupleGentor<Typelist<TrgType, NullType>, index> TrgTupleT;
            typedef TupleGentor<Typelist<SrcType, NullType>, index> SrcTupleT;

            static void Do(TrgTupleT& trg, const SrcTupleT& src)
            {
                TrgType trgVal = Function<TrgType, SrcType>::apply(src);
                trg.set(trgVal);
            }
        };


        template <template<typename,typename>class Function, class TrgTupleT, class SrcTupleT>
        struct Mapper
        {
            typedef typename TrgTupleT::HeadClass TrgHeadClass;
            typedef typename SrcTupleT::HeadClass SrcHeadClass;
            typedef typename TrgTupleT::TailClass TrgTailClass;
            typedef typename SrcTupleT::TailClass SrcTailClass;
            typedef typename TrgHeadClass::ValueType TrgType;
            typedef typename SrcHeadClass::ValueType SrcType;

            static void Do(TrgTupleT& trg, const SrcTupleT& src)
            {
                TrgHeadClass& trgHead        = trg;
                const SrcHeadClass& srcHead = src;
                TrgTailClass& trgTail        = trg;
                const SrcTailClass& srcTail = src;
                TrgType trgVal                = Function<TrgType, SrcType>::apply(srcHead);
                trgHead.set(trgVal);

                Mapper<Function, TrgTailClass, SrcTailClass>::Do(trgTail, srcTail);
            }
        };

        template <template<typename,typename>class Fun, class TrgTupleT, class SrcTupelT>
        TrgTupleT Transform(const SrcTupelT& src)
        {
            TrgTupleT target;
            Mapper<Fun, TrgTupleT, SrcTupelT>::Do(target, src);
            return target;
        }

        // ---------------------------------------------------------------------------
        // template class RefMapper
        // ---------------------------------------------------------------------------
        template <template<typename,typename>class Function, class TrgTupleT, class SrcTupleT>
        struct RefMapper;

        template <template<typename,typename>class Function, typename TrgType, typename SrcType, unsigned int index>
        struct RefMapper<Function, 
            TupleGentor<Typelist<TrgType, NullType>, index>,
            TupleGentor<Typelist<SrcType, NullType>, index> >
        {
            typedef TupleGentor<Typelist<TrgType, NullType>, index> TrgTupleT;
            typedef TupleGentor<Typelist<SrcType, NullType>, index> SrcTupleT;

            static void Do(TrgTupleT& trg, const SrcTupleT& src)
            {
                TrgType trgVal;
                Function<TrgType, SrcType>::apply(trgVal, src);
                trg.set(trgVal);
            }
        };

        template <template<typename,typename>class Function, class TrgTupleT, class SrcTupleT>
        struct RefMapper
        {
            typedef typename TrgTupleT::HeadClass TrgHeadClass;
            typedef typename SrcTupleT::HeadClass SrcHeadClass;
            typedef typename TrgTupleT::TailClass TrgTailClass;
            typedef typename SrcTupleT::TailClass SrcTailClass;
            typedef typename TrgHeadClass::ValueType TrgType;
            typedef typename SrcHeadClass::ValueType SrcType;

            static void Do(TrgTupleT& trg, const SrcTupleT& src)
            {
                TrgHeadClass& trgHead        = trg;
                const SrcHeadClass& srcHead = src;
                TrgTailClass& trgTail        = trg;
                const SrcTailClass& srcTail = src;
                TrgType trgVal;
                Function<TrgType, SrcType>::apply(trgVal, srcHead);
                trgHead.set(trgVal);

                RefMapper<Function, TrgTailClass, SrcTailClass>::Do(trgTail, srcTail);
            }
        };

        // ---------------------------------------------------------------------------
        // template class TemplateMapper
        // ---------------------------------------------------------------------------
        template <template<class>class UnaryTpl, 
                  template<typename,template<class>class> class Function, 
                  class TrgTupleT, class SrcTupleT>
        struct TemplateMapper;


        template <template<class>class UnaryTpl, template<typename,template<class>class> class Function, 
            unsigned int index>
        struct TemplateMapper<UnaryTpl, Function, 
            TupleGentor<NullType, index>,
            TupleGentor<NullType, index> >
        {
            typedef TupleGentor<NullType, index> TrgTupleT;
            typedef TupleGentor<NullType, index> SrcTupleT;
            static void Do(TrgTupleT& trg, SrcTupleT& src){}
        };


        //JODO does not work for empty tuples!
        template <template<class>class UnaryTpl, template<typename,template<class>class> class Function, 
            typename TrgType, unsigned int index>
        struct TemplateMapper<UnaryTpl, Function, 
            TupleGentor<Typelist<TrgType, NullType>, index>,
            TupleGentor<Typelist<UnaryTpl<TrgType>, NullType>, index> >
        {
            typedef TupleGentor<Typelist<TrgType, NullType>, index> TrgTupleT;
            typedef TupleGentor<Typelist<UnaryTpl<TrgType>, NullType>, index> SrcTupleT;

            static void Do(TrgTupleT& trg, SrcTupleT& src)
            {
                TrgType trgVal;
                Function<TrgType, UnaryTpl>::apply(trgVal, src);
                trg.set(trgVal);
            }
        };

        template <template<class>class UnaryTpl,
            template<typename,template<class>class>class Function, 
        class TrgTupleT, class SrcTupleT>
        struct TemplateMapper
        {
            typedef typename TrgTupleT::HeadClass TrgHeadClass;
            typedef typename SrcTupleT::HeadClass SrcHeadClass;
            typedef typename TrgTupleT::TailClass TrgTailClass;
            typedef typename SrcTupleT::TailClass SrcTailClass;
            typedef typename TrgHeadClass::ValueType TrgType;
            typedef typename SrcHeadClass::ValueType SrcType;

            static void Do(TrgTupleT& trg, SrcTupleT& src)
            {
                TrgHeadClass& trgHead = trg;
                SrcHeadClass& srcHead = src;
                TrgTailClass& trgTail = trg;
                SrcTailClass& srcTail = src;
                TrgType trgVal;
                Function<TrgType, UnaryTpl>::apply(trgVal, srcHead);
                trgHead.set(trgVal);

                TemplateMapper<UnaryTpl, Function, TrgTailClass, SrcTailClass>::Do(trgTail, srcTail);
            }
        };
        // ---------------------------------------------------------------------------


        
        ////////////////////////////////////////////////////////////////////////////////
        // helper class template Reductor
        // See reduce below
        ////////////////////////////////////////////////////////////////////////////////

        template <class TupleT, unsigned int i> struct Reductor;

        template <class TupleT>
        struct Reductor<TupleT, 0>
        {
            typedef typename TupleT::TList::Head HeadType;

            static HeadType Do(const TupleT& obj)
            {
                return get<0>(obj);
            }
        };

        template <class TupleT, unsigned int i>
        struct Reductor
        {
            typedef typename TupleT::TList::Head HeadType;
            typedef typename TupleT::TailClass TailClass;

            static HeadType Do(const TupleT& obj)
            {
                // Which is: first(obj) + reduce(tail(obj))
                return get<0>(obj) + Reductor<TailClass, i-1>::Do(obj);
            }
        };
        
        template <class TupleT>
        typename Reductor<TupleT, 0>::HeadType reduce(const TupleT& obj)
        {
            return Reductor<typename TupleT::BaseClass, 
                            TL::Length<typename TupleT::TList>::value-1>::Do(obj);
        }

        // --------------------------------------------------------------------------

        template <typename TrgT, class TupleT>
        TrgT perduce(const TupleT& obj);

        template <template<typename>class Combi, 
                  template<typename>class Embracer, 
                  typename TrgT, class TupleT>
        TrgT perduce2(const TupleT&);

        
        
        // ---------------------------------------------------------------------------
        // template class Apply
        // ---------------------------------------------------------------------------
        template <template<class>class UnaryTpl, 
                  template<typename,template<class>class> class Function, 
                  class TrgTupleT, class SrcTupleT>
        struct Apply;



        template <template<class>class UnaryTpl, template<typename,template<class>class> class Function, 
            unsigned int index>
        struct Apply<UnaryTpl, Function, 
            TupleGentor<NullType, index>,
            TupleGentor<NullType, index> >
        {
            typedef TupleGentor<NullType, index> SrcTupleT;

            static void Do(SrcTupleT& src)
            {
                //CL? Function<TrgType, UnaryTpl>::apply(src);
            }
        };


        //JODO does not work for empty tuples!
        template <template<class>class UnaryTpl, template<typename,template<class>class> class Function, 
                  typename TrgType, unsigned int index>
        struct Apply<UnaryTpl, Function, 
                     TupleGentor<Typelist<TrgType, NullType>, index>,
                     TupleGentor<Typelist<UnaryTpl<TrgType>, NullType>, index> >
        {
            typedef TupleGentor<Typelist<TrgType, NullType>, index> TrgTupleT;
            typedef TupleGentor<Typelist<UnaryTpl<TrgType>, NullType>, index> SrcTupleT;

            static void Do(SrcTupleT& src)
            {
                Function<TrgType, UnaryTpl>::apply(src);
            }
        };


        template <template<class>class UnaryTpl,
                  template<typename,template<class>class>class Function, 
                  class TrgTupleT, class SrcTupleT>
        struct Apply
        {
            typedef typename TrgTupleT::HeadClass TrgHeadClass;
            typedef typename SrcTupleT::HeadClass SrcHeadClass;
            typedef typename TrgTupleT::TailClass TrgTailClass;
            typedef typename SrcTupleT::TailClass SrcTailClass;
            typedef typename TrgHeadClass::ValueType TrgType;
            typedef typename SrcHeadClass::ValueType SrcType;

            static void Do(SrcTupleT& src)
            {
                SrcHeadClass& srcHead = src;
                SrcTailClass& srcTail = src;

                Function<TrgType, UnaryTpl>::apply(srcHead);

                Apply<UnaryTpl, Function, TrgTailClass, SrcTailClass>::Do(srcTail);
            }
        };

        template <class TupleT, unsigned int i> struct Stringer;

        template <class TupleT>
        std::string Stringify(const TupleT& obj);
        
    } // namespace tup

    
    // ---------------------------------------------------------------------------
    // template class tuple
    // ---------------------------------------------------------------------------
    template <typename Types> class tuple 
        : public tup::TupleGentor<Types, 0>
    {
    public:
        typedef tuple<Types>                        SelfType;
        typedef typename tup::TupleGentor<Types, 0> BaseClass;
        typedef NullType                            HeadType;
        
        enum { Size = TL::Length<Types>::value };

        int size()const { return Size; }

        template <unsigned int index> 
        typename TL::TypeAt<Types, index>::Result get()const
        { return Loki::tup::get<index>(*this); }

        std::string as_string()const { return Stringify(*this); }

        //JODO Does not work for empty tuples
        HeadType reduce()const { return tup::reduce(*this); }

        template <typename TrgT>
        TrgT perduce()const { return tup::perduce<TrgT>(*this); }

        template <template<typename>class Combi, 
                  template<typename>class Embracer, typename TrgT>
        TrgT perduce2()const { return tup::perduce2<Combi, Embracer, TrgT>(*this); }

        template <template<typename,typename>class Fun, class TrgTupleT>
        TrgTupleT map()const
        {
            TrgTupleT target;
            tup::Mapper<Fun, TrgTupleT, SelfType>::Do(target, *this);
            return target;
        }

        template <template<typename,typename>class Fun, class TrgTupleT>
        void map(TrgTupleT& target)const
        {
            tup::RefMapper<Fun, TrgTupleT, SelfType>::Do(target, *this);
        }

        // Need example here to reduce abstraction pain
        //        A value generator template     A generator-function-type: value generating function  value-tuple-type        
        //                              GentorT                                   e.g.  SomeValue      to be generated
        template <template<class>class UnaryTpl, template<class TrgTupleT,template<class>class>class Fun, class TrgTupleT>
        void map_template(TrgTupleT& target)
        {
            tup::TemplateMapper<UnaryTpl, Fun, TrgTupleT, SelfType>::Do(target, *this);
        }

        // --------------------------------------------------------------------
        /*
        template 
        <
            template<class>class ConsumerTpl,    // GentorT<TypeT> 
            template<class>class ArgumentTpl,    // GentorProfileT<TypeT> 
            template<template<class>class,        // to consumer GentorT<TypeT>
                     template<class>class>        // apply GentorProfileT<TypeT>
                     class Consume,                // using function Calibrate
            class ArgumentTupleT,                // tuple type for profile tuple value
            class BaseTupleT                    // Basis value tuple type
        >
        void map_consumer_template(const ArgumentTupleT& arguments)
        {
            tup::ConsumerTemplateMapper
            <
                ConsumerTpl, 
                ArgumentTpl, 
                Consume,
                ArgumentTupleT,
                BaseTupleT, 
                SelfType
            >
            ::Do
            (    
                *this,        // SelfType       e.g. Tuple<GentorT       <T>> 
                arguments   // ArgumentTupleT e.g. Tuple<GentorProfileT<T>>
            );
        }
        */
        // --------------------------------------------------------------------

        template <template<class>class UnaryTpl, template<class TrgTupleT,template<class>class>class Fun, class TrgTupleT>
        void apply()
        {
            tup::Apply<UnaryTpl, Fun, TrgTupleT, SelfType>::Do(*this);
        }

        /*
        template <typename TargetType
                  template<typename,typename>class Translator,
                  template<typename>class Combinator,
                  template<typename>class Embracer>
        TargetType transform
        */

        // This leads to a compiler breakdown with ms vc8 !!!
        // template <unsigned int index> 
        // void set(const TL::TypeAt<Types, index>::Result& value)const
        // { return Loki::tup::refer<index>(const_cast<tuple<Types>&> (*this)) = value; }
    };

    namespace tup
    {
        template<typename T0, typename T1>
        tuple<LOKI_TYPELIST_2(T0, T1)> le(const T0& val0, const T1& val1)
        {
            tuple<LOKI_TYPELIST_2(T0, T1)> tup;
            refer<0>(tup)=val0;
            refer<1>(tup)=val1;
            return tup;
        }
        
        //---------------------------------------------------------------------------
        // template class Perductor
        //---------------------------------------------------------------------------
        template <typename TrgT, typename SrcT> struct Perducer;
        
        template <typename TrgT, class TupleT, unsigned int i> struct Perductor;

        template <typename TrgT, class TupleT>
        struct Perductor<TrgT, TupleT, 0>
        {
            typedef typename TupleT::TList::Head HeadType;

            static TrgT Do(const TupleT& obj)
            {
                return Perducer<TrgT, HeadType>::Do(get<0>(obj));
            }
        };

        template <typename TrgT, class TupleT, unsigned int i>
        struct Perductor
        {
            typedef typename TupleT::TList::Head HeadType;
            typedef typename TupleT::TailClass TailClass;

            static TrgT Do(const TupleT& obj)
            {
                // Which is: first(obj) + reduce(tail(obj))
                return Perducer<TrgT, HeadType>::Do(get<0>(obj)) + Perductor<TrgT, TailClass, i-1>::Do(obj);
            }
        };


        template <typename TrgT, class Types>
        struct Perducer<TrgT, tuple<Types> >
        {
            static TrgT Do(const tuple<Types>& obj)
            {
                return perduce<TrgT>(obj);
            }
        };

        template <typename TrgT, typename SrcT> 
        struct Perducer
        {
            static TrgT Do(const SrcT& value) { return value; }
        };

        template <typename TrgT, class TupleT>
        TrgT perduce(const TupleT& obj)
        {
            return Perductor<TrgT, typename TupleT::BaseClass, 
                             TL::Length<typename TupleT::TList>::value-1>::Do(obj);
        }

        //---------------------------------------------------------------------------
        // template class Perductor2
        // allows to specify a Combinator Combi istead of just operator+ to 
        // binary combine values for reduction 
        //---------------------------------------------------------------------------
        template <template<typename>class Combi, 
                  template<typename>class Embracer, 
                  typename TrgT, typename SrcT> struct Perducer2;

        template <template<typename>class Combi, 
                  template<typename>class Embracer, 
                  typename TrgT, class TupleT, unsigned int i> 
        struct Perductor2;

        template <template<typename>class Combi, 
                  template<typename>class Embracer, 
                  typename TrgT, class TupleT>
        struct Perductor2<Combi, Embracer, TrgT, TupleT, 0>
        {
            typedef typename TupleT::TList::Head HeadType;

            static TrgT Do(const TupleT& obj)
            {
                return Perducer2<Combi, Embracer, TrgT, HeadType>::Do(get<0>(obj));
            }
        };

        template <template<typename>class Combi, 
                  template<typename>class Embracer, 
                  typename TrgT, class TupleT, unsigned int i>
        struct Perductor2
        {
            typedef typename TupleT::TList::Head HeadType;
            typedef typename TupleT::TailClass TailClass;

            static TrgT Do(const TupleT& obj)
            {
                // Which is: first(obj) + reduce(tail(obj))
                return
                    Combi<TrgT>::Do(
                        Perducer2<Combi, Embracer, TrgT, HeadType>::Do(get<0>(obj)), 
                        Perductor2<Combi, Embracer, TrgT, TailClass, i-1>::Do(obj)
                    );
            }
        };

        template <template<typename>class Combi, 
                  template<typename>class Embracer, 
                  typename TrgT, class Types>
        struct Perducer2<Combi, Embracer, TrgT, tuple<Types> >
        {
            static TrgT Do(const tuple<Types>& obj)
            {
                //return perduce2<Combi, Embracer, TrgT>(obj);
                return Embracer<TrgT>::Do(perduce2<Combi, Embracer, TrgT>(obj));
            }
        };

        template <template<typename>class Combi, 
                  template<typename>class Embracer, 
                  typename TrgT, typename SrcT> 
        struct Perducer2
        {
            static TrgT Do(const SrcT& value) { return value; }
        };

        template <template<typename>class Combi, 
                  template<typename>class Embracer, 
                  typename TrgT, class TupleT>
        TrgT perduce2(const TupleT& obj)
        {
            return Perductor2<Combi, Embracer, TrgT, typename TupleT::BaseClass, 
                              TL::Length<typename TupleT::TList>::value-1>::Do(obj);
        }

        
        ////////////////////////////////////////////////////////////////////////////////
        // helper class template Stringer
        // See Stringify below
        ////////////////////////////////////////////////////////////////////////////////

        template <unsigned int i>
        struct Stringer<tuple<NullType>, i>
        {
            static std::string Do(const tuple<NullType>& obj)
            {
                return std::string();
            }
        };

        template <class TupleT>
        struct Stringer<TupleT, 0>
        {
            typedef typename TupleT::TList::Head HeadType;

            static std::string Do(const TupleT& obj)
            {
                return to_string<HeadType>::apply(get<0>(obj));
            }
        };

        template <class TupleT, unsigned int i>
        struct Stringer
        {
            typedef typename TupleT::TList::Head HeadType;
            typedef typename TupleT::TailClass TailClass;

            static std::string Do(const TupleT& obj)
            {
                return to_string<HeadType>::apply(get<0>(obj)) 
                       + ", " + Stringer<TailClass, i-1>::Do(obj);
            }
        };

        template <class TupleT>
        std::string Stringify(const TupleT& obj)
        {
            return 
                "("+
                    Stringer<TupleT, TL::Length<typename TupleT::TList>::value-1>::Do(obj)
                +")";
        }



        ////////////////////////////////////////////////////////////////////////////////
        // helper class template Mapper
        // See Map below 
        ////////////////////////////////////////////////////////////////////////////////

        template <typename TargetT, typename SourceT> struct UnaryFunction
        {
            typedef TargetT (*Type)(SourceT);
        };

        template <typename TargetT, typename SourceT> struct TwiceFun
        {
            static TargetT apply(const SourceT& src); 
        };
        template <> 
        struct TwiceFun<int,int>
        {
            static int apply(const int& src) 
            {
                return 2*src;
            }
        };
        template <> 
        struct TwiceFun<int,double>
        {
            static int apply(const double& src) 
            {
                return static_cast<int>(2.0*src);
            }
        };
        template <> 
        struct TwiceFun<double,double>
        {
            static double apply(const double& src) 
            {
                return (2.0*src);
            }
        };

        // --- Referential version -------------------------------------
        template <typename TargetT, typename SourceT> struct RefTwiceFun
        {
            static void apply(TargetT& trg, const SourceT& src); 
        };
        template <> 
        struct RefTwiceFun<int,int>
        {
            static void apply(int& trg, const int& src) 
            {
                trg = 2*src;
            }
        };


        // ---------------------------------------------------------------------------
        // template class ToString
        // ---------------------------------------------------------------------------
        template <typename TargetT, typename SourceT> struct ToString
        { static TargetT apply(const SourceT& src); };

        template <typename SourceT> struct ToString<std::string, SourceT>
        {
            static std::string apply(const SourceT& src)
            {
                return itl::to_string<SourceT>::apply(src);
            }
        };
        // ---------------------------------------------------------------------------



    } //namespace tup        

    
#if defined(_MSC_VER) && _MSC_VER >= 1300
#pragma warning( pop ) 
#endif
}   // namespace Loki

#endif // __Loki_tuple_h_JOFA_070604__

