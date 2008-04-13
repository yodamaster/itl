/**----------------------------------------------------------------------------+
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

using namespace std;
using namespace itl;


/** Example overlap_counter.cpp \file overlap_counter.cpp

    The most basic application of a split_interval_map is a counter counting
    the number of overlaps of intervals inserted into it.

    On could call a split_interval_map an aggregate on overlap machine. A very basic
    aggregation is summation of an integer. A split_interval_map<int,int> maps
    intervals of int to ints. 

    If we insert a value pair (Interval<int>(2,6), 1) into the SpliItvMap, it
    increases the content of all value pairs in the map by 1, if their interval
    part overlaps with Interval<int>(2,6).

    \include overlap_counter/overlap_counter.cpp
*/

/*  A split_interval_map<int, int> is a map<Interval<int>, int> that maps intervals
    to int values. On overlap of intervals it summs up the associated int 
    values.    */
typedef itl::split_interval_map<int, int> OverlapCounterTD;

void print_overlaps(const OverlapCounterTD& counter)
{
    for(OverlapCounterTD::const_iterator it = counter.begin(); it != counter.end(); it++)
    {
        interval<int> itv = (*it).first;
        int overlaps_count = (*it).second;
        if(overlaps_count == 1)
            cout << "in interval " << itv.asString() << " intervals do not overlap" << endl;
        else
            cout << "in interval " << itv.asString() << ": "<< overlaps_count << " intervals overlap" << endl;
    }
}

void overlap_counter()
{
    OverlapCounterTD overlap_counter;
    interval<int> itv;

    itv = rightopen_interval(4,8);
    cout << "-- inserting " << itv.asString() << " -------------------------------------------" << endl;
    overlap_counter.insert(OverlapCounterTD::value_type(itv, 1));
    print_overlaps(overlap_counter);
    cout << "--------------------------------------------------------------" << endl;

    itv = rightopen_interval(6,9);
    cout << "-- inserting " << itv.asString() << " -------------------------------------------" << endl;
    overlap_counter.insert(OverlapCounterTD::value_type(itv, 1));
    print_overlaps(overlap_counter);
    cout << "--------------------------------------------------------------" << endl;

    itv = rightopen_interval(1,9);
    cout << "-- inserting " << itv.asString() << " -------------------------------------------" << endl;
    overlap_counter.insert(OverlapCounterTD::value_type(itv, 1));
    print_overlaps(overlap_counter);
    cout << "--------------------------------------------------------------" << endl;
    
}

int main()
{
    cout << ">> Interval Template Library: Sample overlap_counter.cpp <<\n";
    cout << "-----------------------------------------------------------\n";
    overlap_counter();
    return 0;
}

// Program output:

// >> Interval Template Library: Sample overlap_counter.cpp <<
// -----------------------------------------------------------
// -- inserting [4,8) -------------------------------------------
// in interval [4,8) intervals do not overlap
// --------------------------------------------------------------
// -- inserting [6,9) -------------------------------------------
// in interval [4,6) intervals do not overlap
// in interval [6,8): 2 intervals overlap
// in interval [8,9) intervals do not overlap
// --------------------------------------------------------------
// -- inserting [1,9) -------------------------------------------
// in interval [1,4) intervals do not overlap
// in interval [4,6): 2 intervals overlap
// in interval [6,8): 3 intervals overlap
// in interval [8,9): 2 intervals overlap
// --------------------------------------------------------------
