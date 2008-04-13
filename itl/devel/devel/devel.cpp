// devel.cpp : Definiert den Einstiegspunkt f�r die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>
#include <itl/notate.hpp>
#include <itl/itl_set.hpp>
#include <itl/itl_map.hpp>
//#include <itl/itl_list.hpp>
//#include <itl/string_list.hpp>
#include <itl/interval_set.hpp>
#include <itl/split_interval_map.hpp>
//#include <lokixt/Tuple.h>

using namespace std;
using namespace itl;

//void test_itl_map()
//{
//    typedef itl::map<int,int> ItlMap;
//    //JODO typedef std::map<int,int> StlMap;
//    typedef itl::map<int,int> StlMap;
//    itl::map<int,int> itlMap;
//
//    itlMap.insert(ItlMap::value_type(1, 42));
//    itlMap.insert(ItlMap::value_type(2, 43));
//    itlMap[2] = 44;
//
//    FORALL(ItlMap, it_, itlMap)
//    {
//        cout << (*it_).first << "->" << (*it_).second << endl;
//    }
//
//    StlMap stlMap(itlMap);
//    ItlMap itlMap2;
//    itlMap2 = itlMap;
//    
//    cout << "---------------\n";
//    FORALL(ItlMap, it2_, itlMap2)
//    {
//        cout << (*it2_).first << "->" << (*it2_).second << endl;
//    }
//
//    if(itlMap == itlMap2)
//        cout << "maps are equal\n" ;
//    else
//        cout << "maps are NOT equal\n" ;
//
//    itlMap2.insert(make_pair(5,25));
//    itlMap2.insert(make_pair(6,36));
//
//    cout << "---------------\n";
//    FORALL(ItlMap, it2_, itlMap2)
//    {
//        cout << (*it2_).first << "->" << (*it2_).second << endl;
//    }
//
//    itlMap2 -= itlMap;
//
//    {
//        cout << "---------------\n";
//        FORALL(ItlMap, it_, itlMap)
//        {
//            cout << (*it_).first << "->" << (*it_).second << endl;
//        }
//    }
//
//    itlMap += itlMap2;
//
//    {
//        cout << "---------------\n";
//        FORALL(ItlMap, it_, itlMap)
//        {
//            cout << (*it_).first << "->" << (*it_).second << endl;
//        }
//    }
//}
//
//void test_itl_set()
//{
//    typedef itl::set<string> ItlSetT;
//
//    ItlSetT residents, people;
//    residents.insert("Joachim");
//    residents.insert("Christina");
//    ItlSetT guests;
//    guests.insert("Dreas");
//    guests.insert("Katja");
//
//    people = residents;
//    people += guests;
//
//    cout << people.asString() << endl;
//}
//
//
//void sample_list_interlacing()
//{
//    //JODO This is a basic example for list interlacing. This has to be worked
//    // out a little more for a new sample project.
//    typedef itl::list<string> ItlListT;
//
//    ItlListT ball, big, shine;
//    ball.add("My").add("ball").add("is").add("blue");
//    big.add("My").add("big").add("is").add("light");
//    shine.add("big").add("light").add("and").add("shines");
//
//    cout << ball.asString() << endl;
//    cout << big.asString() << endl;
//    cout << endl;
//
//    ball.interlace(big);
//    cout << ball.asString() << endl;
//
//    cout << endl;
//    cout << shine.asString() << endl;
//    ball.interlace(shine);
//
//    cout << endl;
//    cout << ball.asString() << endl;
//}
//
//void test_itl_list()
//{
//    typedef itl::list<string> ItlListT;
//    string balls = "my  ball  is  blue";
//    string_list ball; ball.split(balls);
//    cout << ball.asString() << endl;
//}
//
//void test_tuple()
//{
//    using namespace Loki;
//    //tuple<LOKI_TYPELIST_1(int)> sgl_int;
//
//}
//
//void test_intersection()
//{
//    cout << "function: test_intersection()" << endl;
//
//    typedef itl::split_interval_map<int,int> SplipT;
//    typedef itl::interval_set<int> ItvSetT;
//
//    ItvSetT iset;
//    iset.insert(rightopen_interval(33,42));
//    SplipT splip;
//    splip.insert(SplipT::value_type(rightopen_interval(11,42), 1));
//
//    splip *= iset;
//
//    cout << splip.asString() << endl;
//
//}
//
//// --- TypeToString -----------------------------------------------------------
//template <typename Type>
//struct TypeToString{ static std::string it(); };
//
//template <typename Type>
//struct TypeToString2{ static std::string it(); };
//
//std::string TypeToString<int>::it(){ return "int"; }
//std::string TypeToString<double>::it(){ return "double"; }
//
//template <template<class>class UnaryTemplate>
//struct UnaryTemplateToString{ static std::string it(); };
//
//template <template<class,class>class BinaryTemplate>
//struct BinaryTemplateToString{ static std::string it(); };
//
//template <class T> struct MyUnary {};
//
//std::string UnaryTemplateToString<MyUnary>::it(){ return "MyUnary"; }
//
//template <class T, class U> struct MyBinary {};
//
//std::string BinaryTemplateToString<MyBinary>::it(){ return "MyBinary"; }
//
//
//template<template<class>class UnaryTemplate, typename T>
//struct TypeToString<UnaryTemplate<T> >
//{
//    static std::string it()
//    {
//        return UnaryTemplateToString<UnaryTemplate>::it() + "<" + TypeToString<T>::it() + ">";
//    }
//};
//
//template <template<int, template<class>class>class UnaryTemplate2>
//struct UnaryTemplate2ToString{ static std::string it(); };
//
//template <int, template<class>class Tpl> struct MyUnary2 {};
//
//std::string UnaryTemplate2ToString<MyUnary2>::it(){ return "MyUnary2"; }
//
//template<template<int, template<class>class>class UnaryTemplate2, template<class>class Tpl>
//struct TypeToString<UnaryTemplate2<1, Tpl> >
//{
//    static std::string it()
//    {
//        return UnaryTemplate2ToString<UnaryTemplate2>::it() + "<" + UnaryTemplateToString<Tpl>::it() + ">";
//    }
//};
//
//
//template<template<int, template<class>class>class BinaryTemplate2, template<class>class Tpl>
//struct TypeToString<BinaryTemplate2<2, Tpl> >
//{
//    static std::string it()
//    {
//        return UnaryTemplate2ToString<BinaryTemplate2>::it() + "<1*" + " some " + ">";
//    }
//};
//// --- TypeToString -----------------------------------------------------------
//
//
//void test_TypeToString()
//{
//    cout << TypeToString<MyUnary<int> >::it() << endl;
//    cout << TypeToString<MyUnary2<1, MyUnary> >::it() << endl;
//    cout << TypeToString<MyUnary2<2, MyUnary> >::it() << endl;
//}

void tmp_test_map()
{
    itl::set<int> y;
    y.insert(3);
    y.insert(5);
    cout << y.asString() << endl;

    itl::map<int,int> my;
    my.insert(make_pair(3,3));
    my.insert(make_pair(5,3));
    cout << my.asString() << endl;

    itl::interval_set<int> is;
    is.insert(rightopen_interval(1,3));
    is.insert(rightopen_interval(3,5));
    cout << is.asString() << endl;

    itl::split_interval_map<int,int> im;
    im.insert(make_pair(rightopen_interval(1,6), 1));
    im.insert(make_pair(rightopen_interval(4,8), 1));
    cout << im.asString() << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
    //sample_list_interlacing();
    //test_itl_list();
    //test_tuple();
    //test_intersection();
    //test_TypeToString();
    tmp_test_map();
    return 0;
}

