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
#include <stdio.h>
#include <iostream>
#include <itl/ctxreprbase.hpp>
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
	So we insert a time interval and a name set to the split_interval_map for the attendance
	of each group of people, that come together and leave together.

	On every overlap of intervals, the corresponding name sets are accumulated. At
	the points of overlap the intervals are split. The accumulation of content on
	overlap of intervals is always done via an operator += that has to be implemented
	for the content parameter of the SpliItvMapT.

	Finally the split_interval_map contains the history of attendance and all points in
	time, where the group of party guests changed.

    \include party/party.cpp
*/

// Type string_set<> collects the names of party guests. Therefore it needs to
// implement operator += that performs a set union on overlap of intervals.
typedef string_set<> GuestsNameSetTD;

// Time is the domain type the the split_interval_map. It's key values are therefore
// time intervals: interval<Time>. The content is the set of names: GuestsNameSetTD.
typedef split_interval_map<Time, GuestsNameSetTD> PartyAttendenceHistoryTD;

void party()
{
	GuestsNameSetTD mary_harry; 
	mary_harry.insert("Mary");
	mary_harry.insert("Harry");

	GuestsNameSetTD diana_susan; 
	diana_susan.insert("Diana");
	diana_susan.insert("Susan");

	GuestsNameSetTD peter; 
	peter.insert("Peter");

	PartyAttendenceHistoryTD party;

	party.insert(make_pair( rightopen_interval<Time>(Time(19,30), Time(23,00)), mary_harry) );
	party.insert(make_pair( rightopen_interval<Time>(Time(20,10), Time(monday,0,0)), diana_susan) );
	party.insert(make_pair( rightopen_interval<Time>(Time(22,15), Time(monday,0,30)), peter) );

	PartyAttendenceHistoryTD::iterator it = party.begin();
	while(it != party.end())
	{
		interval<Time> when = (*it).first;
		// Who is at the party within the time interval 'when' ?
		GuestsNameSetTD who = (*it++).second;
		cout << when.asString() << ": " << who.asString() << endl;
	}
}


int main()
{
	cout << ">> Interval Template Library: Sample party.cpp <<\n";
	cout << "-------------------------------------------------\n";
	party();
	system("pause");
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