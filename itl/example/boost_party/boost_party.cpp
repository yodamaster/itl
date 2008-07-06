/*----------------------------------------------------------------------------+
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2008: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#include <stdio.h>
#include <iostream>
#include <itl/itl_value.hpp>
#include <itl/string_set.hpp>
#include <itl/split_interval_map.hpp>

// The next line includes <boost/date_time/posix_time/posix_time.hpp>
// and a few lines of adapter code.
#include <itl/itl_ptime.hpp> 

using namespace std;
using namespace boost::posix_time;
using namespace itl;

/** Example boost_party.cpp \file boost_party.cpp

    Party.cpp demonstrates the possibilities of a split interval map (split_interval_map).
	As time parameter boost::posix_time::ptime is used. 
    A split_interval_map maps intervals to a given content. In this case the content 
    is a set of party guests represented by their name strings.

    As time goes by, groups of people join the party and leave later in the evening.
    So we add a time interval and a name set to the split_interval_map for the attendance
    of each group of people, that come together and leave together.

    On every overlap of intervals, the corresponding name sets are accumulated. At
    the points of overlap the intervals are split. The accumulation of content on
    overlap of intervals is always done via an operator += that has to be implemented
    for the content parameter of the split_interval_map.

    Finally the split_interval_map contains the history of attendance and all points in
    time, where the group of party guests changed.

    Party.cpp demonstrates a principle that we call 
    <b><em>aggregate on overlap (aggovering;)</em></b>:
    On insertion a value associated to the interval is aggrgated (added) to those
    values in the split_interval_map that overlap with the inserted value.

    There are two behavioral aspects to <b>aggovering</b>: a <em>decompositional
    behavior</em> and a <em>accumulative behavior</em>.

    The <em>decompositional behavior</em> splits up intervals on the time dimension of the 
    split_interval_map so that the intervals change whenever associated values
    change.

    The <em>accumulative behavior</em> accumulates associated values on every overlap of
    an insertion for the associated values.

    \include party/boost_party.cpp
*/

// Type itl::set<string> collects the names of party guests. Therefore it needs to
// implement operator += that performs a set union on overlap of intervals.
typedef itl::set<string> GuestSetT;

// boost::posix_time::ptime is the domain type the the split_interval_map. 
// It's key values are therefore time intervals: interval<ptime>. The content
// is the set of names: GuestSetT.
typedef split_interval_map<ptime, GuestSetT> BoostPartyAttendenceHistoryT;

// A party's height shall be defined as the maximum height of all guests ;-)
typedef interval_map<ptime, int> BoostPartyHeightHistoryT;

typedef interval_map<ptime, GuestSetT> Party2T;

void boost_party()
{
    GuestSetT mary_harry; 
    mary_harry.insert("Mary");
    mary_harry.insert("Harry");

    GuestSetT diana_susan; 
    diana_susan.insert("Diana");
    diana_susan.insert("Susan");

    GuestSetT peter; 
    peter.insert("Peter");

    GuestSetT john; 
    peter.insert("John");

    BoostPartyAttendenceHistoryT party;

	party.add( // add and element
	  make_pair( 
		rightopen_interval<ptime>(
		  time_from_string("2008-05-20 19:30"), 
		  time_from_string("2008-05-20 23:00")), 
		  mary_harry));

	party += // element addition can also be done via operator +=
	  make_pair( 
		rightopen_interval<ptime>(
		  time_from_string("2008-05-20 20:10"), 
		  time_from_string("2008-05-21 00:00")), 
		  diana_susan);
	party +=
	  make_pair( 
		rightopen_interval<ptime>(
		  time_from_string("2008-05-20 22:15"), 
		  time_from_string("2008-05-21 00:30")), 
		  peter);

	//-------------------------------------------------------------------------
	Party2T party2;

	party2 += 
	  make_pair( 
		rightopen_interval<ptime>(
		  time_from_string("2008-05-20 19:00"), 
		  time_from_string("2008-05-21 20:00")), 
		  diana_susan);
	party2 +=
	  make_pair( 
		rightopen_interval<ptime>(
		  time_from_string("2008-05-20 19:30"), 
		  time_from_string("2008-05-21 22:30")), 
		  john);

	party += party2;

    BoostPartyHeightHistoryT tallest_guest;

	// adding an element can be done wrt. simple aggregate functions
	// like e.g. min, max etc. in their 'inplace' or op= incarnation
	tallest_guest.add<inplace_max>(
	  make_pair( 
		rightopen_interval<ptime>(
		  time_from_string("2008-05-20 19:30"), 
		  time_from_string("2008-05-20 23:00")), 
		  180));

	tallest_guest.add<inplace_max>(
	  make_pair( 
		rightopen_interval<ptime>(
		  time_from_string("2008-05-20 20:10"), 
		  time_from_string("2008-05-21 00:00")), 
		  170));

	tallest_guest.add<inplace_max>(
	  make_pair( 
		rightopen_interval<ptime>(
		  time_from_string("2008-05-20 22:15"), 
		  time_from_string("2008-05-21 00:30")), 
		  200));


    BoostPartyAttendenceHistoryT::iterator it = party.begin();
	cout << "----- Histrory of party guests ------------------------\n";
    while(it != party.end())
    {
        interval<ptime> when = (*it).first;
        // Who is at the party within the time interval 'when' ?
        GuestSetT who = (*it++).second;
        cout << "[" << when.first() << " - " << when.upper() << ")"
			 << ": " << who.as_string() << endl;
    }

    BoostPartyHeightHistoryT::iterator height_ = tallest_guest.begin();
	cout << "----- Histrory of maximum guest hight -----------------\n";
    while(height_ != tallest_guest.end())
    {
        interval<ptime> when = height_->first;
        // Who is at the party within the time interval 'when' ?
        int height = (*height_++).second;
        cout << "[" << when.first() << " - " << when.upper() << ")"
			 << ": " << height <<" cm = " << height/30.48 << " ft" << endl;
    }
}


int main()
{
    cout << ">> Interval Template Library: Sample boost_party.cpp <<\n";
    cout << "-------------------------------------------------------\n";
    boost_party();
    return 0;
}

// Program output:
/*-----------------------------------------------------------------------------
>> Interval Template Library: Sample boost_party.cpp <<
-------------------------------------------------------
----- Histrory of party guests ------------------------
[2008-May-20 19:30:00 - 2008-May-20 20:10:00): Harry Mary
[2008-May-20 20:10:00 - 2008-May-20 22:15:00): Diana Harry Mary Susan
[2008-May-20 22:15:00 - 2008-May-20 23:00:00): Diana Harry Mary Peter Susan
[2008-May-20 23:00:00 - 2008-May-21 00:00:00): Diana Peter Susan
[2008-May-21 00:00:00 - 2008-May-21 00:30:00): Peter
----- Histrory of maximum guest hight -----------------
[2008-May-20 19:30:00 - 2008-May-20 22:15:00): 180 cm = 5.90551 ft
[2008-May-20 22:15:00 - 2008-May-21 00:30:00): 200 cm = 6.56168 ft
-----------------------------------------------------------------------------*/
