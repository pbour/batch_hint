/******************************************************************************
 * Project:  hint
 * Purpose:  Indexing interval data
 * Author:   Panagiotis Bouros, pbour@github.io
 ******************************************************************************
 * Copyright (c) 2020 - 2023
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

#pragma once
#ifndef _GLOBAL_DEF_H_
#define _GLOBAL_DEF_H_

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <tuple>
using namespace std;


// Workload type
//#define WORKLOAD_COUNT
#define WORKLOAD_XOR

//#define XOR_START


// Query type
#define QUERY_SELECTION     1
#define QUERY_JOIN          2

// Basic predicates of Allen's algebra
#define PREDICATE_EQUALS     1
#define PREDICATE_STARTS     2
#define PREDICATE_STARTED    3
#define PREDICATE_FINISHES   4
#define PREDICATE_FINISHED   5
#define PREDICATE_MEETS      6
#define PREDICATE_MET        7
#define PREDICATE_OVERLAPS   8
#define PREDICATE_OVERLAPPED 9
#define PREDICATE_CONTAINS   10
#define PREDICATE_CONTAINED  11
#define PREDICATE_PRECEDES   12
#define PREDICATE_PRECEDED   13

// Generalized predicates, ACM SIGMOD'22 gOverlaps
#define PREDICATE_GOVERLAPS  14


// Index
#define HINT_M_BASE                 0 // Base HINT_M
#define HINT_M_SUBS                 1
#define HINT_M_SUBS_SORT            2
#define HINT_M_SUBS_SOPT            3
#define HINT_M_SUBS_SORT_SOPT       4
#define HINT_M_SUBS_SORT_SOPT_SS    5
#define HINT_M_SUBS_SORT_SOPT_CM    6
#define HINT_M_SUBS_SORT_SS_CM      7
//#define HINT_M_ALL 8


// For selection queries
#define SELECTION_BATCH_PERQUERY           1
#define SELECTION_BATCH_PERLEVEL           2
#define SELECTION_BATCH_PERPARTITION       3


typedef int PartitionId;
typedef int RecordId;
typedef int Timestamp;


struct RunSettings
{
    // Input
	const char   *input_fileR;
    const char   *input_fileS;
    
    // Index/indices
    unsigned int hintm_numBits;
    unsigned int hintm_maxBits;
    unsigned int hintm_variant;

    // For queries, in general
    const char   *query_file;
    bool         query_verbose;
    unsigned int query_numRuns;
    unsigned int query_type;
    unsigned int query_predicate;

    // For selection queries
    bool         selection_topDown;
    unsigned int selection_strategy;
    bool         selection_sortQueries;

    void init()
    {
        // For queries, in general
        query_verbose	   = false;
        query_numRuns      = 1;
        query_predicate    = PREDICATE_GOVERLAPS;
        
        // For selection queries
        selection_topDown        = false;
        selection_strategy       = SELECTION_BATCH_PERQUERY;
        selection_sortQueries = false;
    };
};


struct StabbingQuery
{
	size_t id;
	Timestamp point;
    
    StabbingQuery()
    {
        
    };
    StabbingQuery(size_t i, Timestamp p)
    {
        id = i;
        point = p;
    };
};

struct RangeQuery
{
	size_t id;
	Timestamp start, end;

    RangeQuery()
    {
        
    };
    RangeQuery(size_t i, Timestamp s, Timestamp e)
    {
        id = i;
        start = s;
        end = e;
    };
    
    bool operator<(const RangeQuery &q)
    {
        if (start == q.start)
            return end < q.end;
        else
            return start < q.start;
    };
};


class Timer
{
private:
	using Clock = std::chrono::high_resolution_clock;
	Clock::time_point start_time, stop_time;
	
public:
	Timer()
	{
		start();
	}
	
	void start()
	{
		start_time = Clock::now();
	}
	
	
	double getElapsedTimeInSeconds()
	{
		return std::chrono::duration<double>(stop_time - start_time).count();
	}
	
	
	double stop()
	{
		stop_time = Clock::now();
		return getElapsedTimeInSeconds();
	}
};


// Imports from utils
void process_mem_usage(double& vm_usage, double& resident_set);
string toUpperCase(char *buf);
bool checkHINT_M_Variant(string strHINT_M_Variant, RunSettings &settings);
bool checkPredicate(string strPredicate, RunSettings &settings);
bool checkSelectionStrategy(string strStrategy, RunSettings &settings);
#endif // _GLOBAL_DEF_H_
