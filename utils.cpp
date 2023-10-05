/******************************************************************************
 * Project:  hint
 * Purpose:  Indexing interval data
 * Author:   Panagiotis Bouros, pbour@github.io
 ******************************************************************************
 * Copyright (c) 2020 - 2022
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

#include "def_global.h"



string toUpperCase(char *buf)
{
    auto i = 0;
    while (buf[i])
    {
        buf[i] = toupper(buf[i]);
        i++;
    }
    
    return string(buf);
}


// Calculate memory usage: WORKS ONLY on Linux
void process_mem_usage(double& vm_usage, double& resident_set)
{
    vm_usage     = 0.0;
    resident_set = 0.0;
    
    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
        >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
        >> ignore >> ignore >> vsize >> rss;
    }
    
    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}


// Check input parameters
bool checkHINT_M_Variant(string strHINT_M_Variant, RunSettings &settings)
{
    if (strHINT_M_Variant == "")
    {
        settings.hintm_variant = HINT_M_BASE;
        return true;
    }
    else if (strHINT_M_Variant == "SUBS")
    {
        settings.hintm_variant = HINT_M_SUBS;
        return true;
    }
    else if (strHINT_M_Variant == "SUBS+SORT")
    {
        settings.hintm_variant = HINT_M_SUBS_SORT;
        return true;
    }
    else if (strHINT_M_Variant == "SUBS+SOPT")
    {
        settings.hintm_variant = HINT_M_SUBS_SOPT;
        return true;
    }
    else if (strHINT_M_Variant == "SUBS+SORT+SOPT")
    {
        settings.hintm_variant = HINT_M_SUBS_SORT_SOPT;
        return true;
    }
    else if (strHINT_M_Variant == "SUBS+SORT+SOPT+SS")
    {
        settings.hintm_variant = HINT_M_SUBS_SORT_SOPT_SS;
        return true;
    }
    else if (strHINT_M_Variant == "SUBS+SORT+SOPT+CM")
    {
        settings.hintm_variant = HINT_M_SUBS_SORT_SOPT_CM;
        return true;
    }
    else if (strHINT_M_Variant == "SUBS+SORT+SS+CM")
    {
        settings.hintm_variant = HINT_M_SUBS_SORT_SS_CM;
        return true;
    }
//    else if (strHINT_M_Variant == "ALL")
//    {
//        settings.hintm_variant = HINT_M_ALL;
//        return true;
//    }
    
    return false;
}
    


bool checkPredicate(string strPredicate, RunSettings &settings)
{
    if (strPredicate == "EQUALS")
    {
        settings.query_predicate = PREDICATE_EQUALS;
        return true;
    }
    else if (strPredicate == "STARTS")
    {
        settings.query_predicate = PREDICATE_STARTS;
        return true;
    }
    else if (strPredicate == "STARTED")
    {
        settings.query_predicate = PREDICATE_STARTED;
        return true;
    }
    else if (strPredicate == "FINISHES")
    {
        settings.query_predicate = PREDICATE_FINISHES;
        return true;
    }
    else if (strPredicate == "FINISHED")
    {
        settings.query_predicate = PREDICATE_FINISHED;
        return true;
    }
    else if (strPredicate == "MEETS")
    {
        settings.query_predicate = PREDICATE_MEETS;
        return true;
    }
    else if (strPredicate == "MET")
    {
        settings.query_predicate = PREDICATE_MET;
        return true;
    }
    else if (strPredicate == "OVERLAPS")
    {
        settings.query_predicate = PREDICATE_OVERLAPS;
        return true;
    }
    else if (strPredicate == "OVERLAPPED")
    {
        settings.query_predicate = PREDICATE_OVERLAPPED;
        return true;
    }
    else if (strPredicate == "CONTAINS")
    {
        settings.query_predicate = PREDICATE_CONTAINS;
        return true;
    }
    else if (strPredicate == "CONTAINED")
    {
        settings.query_predicate = PREDICATE_CONTAINED;
        return true;
    }
    else if (strPredicate == "BEFORE")
    {
        settings.query_predicate = PREDICATE_PRECEDES;
        return true;
    }
    else if (strPredicate == "AFTER")
    {
        settings.query_predicate = PREDICATE_PRECEDED;
        return true;
    }
    if (strPredicate == "GOVERLAPS")
    {
        settings.query_predicate = PREDICATE_GOVERLAPS;
        return true;
    }

    return false;
}


bool checkSelectionStrategy(string strStrategy, RunSettings &settings)
{
    if (strStrategy == "QUERY")
    {
        settings.selection_strategy = SELECTION_BATCH_PERQUERY;
        return true;
    }
    else if (strStrategy == "LEVEL")
    {
        settings.selection_strategy = SELECTION_BATCH_PERLEVEL;
        return true;
    }
//    else if (strStrategy == "PARTITION1")
//    {
//        settings.selection_strategy = SELECTION_BATCH_PERPARTITION1;
//        return true;
//    }
//    else if (strStrategy == "PARTITION1+")
//    {
//        settings.selection_strategy = SELECTION_BATCH_PERPARTITION1_PLUS;
//        return true;
//    }
//    else if (strStrategy == "PARTITION2")
//    {
//        settings.selection_strategy = SELECTION_BATCH_PERPARTITION2;
//        return true;
//    }
//    else if (strStrategy == "PARTITION2+")
//    {
//        settings.selection_strategy = SELECTION_BATCH_PERPARTITION2_PLUS;
//        return true;
//    }
    else if (strStrategy == "PARTITION")
    {
        settings.selection_strategy = SELECTION_BATCH_PERPARTITION;
        return true;
    }

    return false;
}
