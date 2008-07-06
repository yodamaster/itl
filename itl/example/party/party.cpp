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
#include "../toytime.h"

using namespace std;
using namespace itl;

/** Example party.cpp \file Party.cpp

    Party.cpp demonstrates the possibilities of a split interval map (split_interval_map).
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

    \include party/party.cpp
*/

// Type itl::set<string> collects the names of party guests. Therefore it needs to
// implement operator += that performs a set union on overlap of intervals.
typedef itl::set<string> GuestSetT;

// Time is the domain type the the split_interval_map. It's key values are therefore
// time intervals: interval<Time>. The content is the set of names: GuestSetT.

typedef split_interval_map<Time, GuestSetT> PartyAttendenceHistoryT;

void party()
{
    GuestSetT mary_harry; 
    mary_harry.insert("Mary");
    mary_harry.insert("Harry");

    GuestSetT diana_susan; 
    diana_susan.insert("Diana");
    diana_susan.insert("Susan");

    GuestSetT peter; 
    peter.insert("Peter");

    PartyAttendenceHistoryT party;

    party += make_pair( rightopen_interval<Time>(Time(19,30), Time(23,00)), mary_harry);
    party += make_pair( rightopen_interval<Time>(Time(20,10), Time(monday,0,0)), diana_susan);
    party += make_pair( rightopen_interval<Time>(Time(22,15), Time(monday,0,30)), peter);

    PartyAttendenceHistoryT::iterator it = party.begin();
    while(it != party.end())
    {
        interval<Time> when = (*it).first;
        // Who is at the party within the time interval 'when' ?
        GuestSetT who = (*it++).second;
        cout << when.as_string() << ": " << who.as_string() << endl;
    }
}

int main()
{
    cout << ">> Interval Template Library: Sample party.cpp       <<\n";
    cout << "-------------------------------------------------------\n";
    party();
    return 0;
}

// Program output:

// >> Interval Template Library: Sample party.cpp <<
// -------------------------------------------------
// [sun:19:30,sun:20:10): Harry Mary
// [sun:20:10,sun:22:15): Diana Harry Mary Susan
// [sun:22:15,sun:23:00): Diana Harry Mary Peter Susan
// [sun:23:00,mon:00:00): Diana Peter Susan
// [mon:00:00,mon:00:30): Peter
