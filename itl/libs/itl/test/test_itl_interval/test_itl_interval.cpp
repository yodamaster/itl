/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::interval unit test
#include <string>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

// interval instance types
//#include <itl/itl_gregorian_date.hpp> 
#include <itl/itl_ptime.hpp> 

#include <itl/itl_interval.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace itl;


typedef ::boost::mpl::list<
	unsigned short, unsigned int, unsigned long  
	,short, int, long
	,float, double
	,boost::posix_time::ptime
> bicrementable_types;

typedef ::boost::mpl::list<
	unsigned short, unsigned int, unsigned long  
	,short, int, long
> integral_types;

typedef ::boost::mpl::list<
	unsigned short, unsigned int, unsigned long  
	,short, int, long
	,boost::posix_time::ptime
> discrete_types;

typedef ::boost::mpl::list<
	float, double
	,std::string
> continuous_types;

typedef ::boost::mpl::list<
	unsigned short, unsigned int, unsigned long  
	,short, int, long
	,float, double
	,std::string
	,boost::posix_time::ptime
> ordered_types;



BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_bicrementability, T, bicrementable_types)
{
	BOOST_CHECK_EQUAL(T(), pred(succ(T())));
	BOOST_CHECK_EQUAL(itl::type<T>::neutron(), pred(succ(itl::type<T>::neutron())));
	BOOST_CHECK_EQUAL(itl::type<T>::unon(), succ(itl::type<T>::neutron()));
	BOOST_CHECK_EQUAL(interval<T>().length(), itl::type<difference<T>::type>::neutron());

	T v1 = itl::type<T>::unon(); 
	T v2 = v1; ++v2;
	T v3 = v2; ++v3;
	T v4 = v3; ++v4;
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_constructors, T, ordered_types)
{
	// An empty interval is defined as the closed interval [1,0]
	BOOST_CHECK_EQUAL(interval<T>().empty(), true);
	BOOST_CHECK_EQUAL(interval<T>().cardinality(), itl::type<itl::size<T>::type>::neutron());
	BOOST_CHECK_EQUAL(interval<T>().lower(), itl::type<T>::unon());
	BOOST_CHECK_EQUAL(interval<T>().upper(), itl::type<T>::neutron());

	BOOST_CHECK_EQUAL(interval<T>(), interval<T>());
	BOOST_CHECK_EQUAL(interval<T>(), interval<T>(itl::type<T>::unon(), itl::type<T>::neutron()));
	BOOST_CHECK_EQUAL(interval<T>(), interval<T>(itl::type<T>::unon(), itl::type<T>::neutron(), interval<T>::CLOSED));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_constructors_integral, T, integral_types)
{
	BOOST_CHECK_EQUAL(interval<T>().first(), itl::type<T>::unon());
	BOOST_CHECK_EQUAL(interval<T>().last(), itl::type<T>::neutron());
	BOOST_CHECK_EQUAL(interval<T>().length(), 0);
}

BOOST_AUTO_TEST_CASE(test_itl_interval_constructors_discrete_hudel)
{
	BOOST_CHECK_EQUAL(interval<double>().continuous_length(), 0.0);
}


