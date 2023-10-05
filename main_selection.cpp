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

#include "getopt.h"
#include "def_global.h"
#include "./containers/relation.h"
#include "./indices/hint_m.h"



void usage()
{
    cerr << endl;
    cerr << "PROJECT" << endl;
    cerr << "       Querying Interval Data on Steroids" << endl << endl;
    cerr << "USAGE" << endl;
    cerr << "       ./query_selection.exec [OPTION]... [DATA] [QUERIES]" << endl << endl;
    cerr << "DESCRIPTION" << endl;
    cerr << "       -? or -h" << endl;
    cerr << "              display this help message and exit" << endl;
    cerr << "       -m bits" << endl;
    cerr << "              set number of bits" << endl;
    cerr << endl;
    cerr << "       -r runs" << endl;
    cerr << "              set number of runs per query; by default 1" << endl;
    cerr << endl;
    cerr << "       -b strategy" << endl;
    cerr << "              select query strategy for batch processing: \"query\" or \"level\" or \"partition\"; by default \"query\"" << endl;
    cerr << "       -s sort queries" << endl;
    cerr << endl;
    cerr << "EXAMPLES" << endl;
    cerr << "       TODO" << endl;
    cerr << "       ./query_batch.exec -m 10 -r 10 -b QUERY inputs/BOOKS.inp queries/BOOKS_qe0.1%_qn10K.qry" << endl;
    cerr << "       ./query_batch.exec -m 10 -r 10 -b QUERY -s inputs/BOOKS.inp queries/BOOKS_qe0.1%_qn10K.qry" << endl;
    cerr << "       ./query_batch.exec -m 10 -r 10 -b LEVEL -s inputs/BOOKS.inp queries/BOOKS_qe0.1%_qn10K.qry" << endl;
    cerr << "       ./query_batch.exec -m 10 -r 10 -b PARTITION -s inputs/BOOKS.inp queries/BOOKS_qe0.1%_qn10K.qry" << endl;
    cerr << endl;
}


int main(int argc, char **argv)
{
    Timer tim;
    Relation R;
    HINT_M_SubsSort_SS_CM *idxR;
    size_t totalResult = 0, numQueries = 0;
    double totalSortTime = 0, totalIndexTime = 0, totalQueryTime = 0;
    Timestamp qstart, qend;
    RunSettings settings;
    char c;
    double vmDQ = 0, rssDQ = 0, vmI = 0, rssI = 0;
    string /*strHINT_M_Variant = "", strPredicate = "", */strStrategy = "";
    vector<RangeQuery> Q;
    size_t sumQ = 0;
    size_t *result;


    // Parse command line input
    settings.init();
    settings.query_type = QUERY_SELECTION;
    while ((c = getopt(argc, argv, "?hm:r:q:b:s")) != -1)
    {
        switch (c)
        {
            case '?':
            case 'h':
                usage();
                return 0;
                

            // HINT^m parameters
            case 'm':
                settings.hintm_numBits = atoi(optarg);
                break;
                
            
            // General query parameters
            case 'r':
                settings.query_numRuns = atoi(optarg);
                break;
                
                
            // Evaluation method parameters
            case 'b':
                strStrategy = toUpperCase((char*)optarg);
                break;
                
            case 's':
                settings.selection_sortQueries = true;
                break;
                
            default:
                cerr << endl << "Error - unknown option '" << c << "'" << endl << endl;
                usage();
                return 1;
        }
    }
    
    
    // Sanity check
    if (argc-optind != 2)
    {
        usage();
        return 1;
    }
    if (settings.hintm_numBits <= 0)
    {
        cerr << endl << "Error - invalid number of bits \"" << settings.hintm_numBits << "\" (option 'm')" << endl << endl;
        usage();
        return 1;
    }
    if (!checkSelectionStrategy(strStrategy, settings))
    {
        cerr << endl << "Error - unknown batch strategy \"" << strStrategy << "\" (option 'b')" << endl << endl;
        usage();
        return 1;
    }
    if ((!settings.selection_sortQueries) && ((settings.selection_strategy == SELECTION_BATCH_PERLEVEL) ||
        (settings.selection_strategy == SELECTION_BATCH_PERPARTITION)))
    {
        cerr << endl << "Error - sort queries (option '-s') for batch strategy \"" << strStrategy << "\"" << endl << endl;
        usage();
        return 1;
    }
    
    
    // Load intervals
    settings.input_fileR = argv[optind];
    R.load(settings.input_fileR);
    settings.hintm_maxBits = int(log2(R.gend-R.gstart)+1);
    
    
    // Load and initialize queries
    settings.query_file = argv[optind+1];
    ifstream fQ(settings.query_file);
    if (!fQ)
    {
        cerr << endl << "Error - cannot open query file \"" << settings.query_file << "\"" << endl << endl;
        return 1;
    }

    while (fQ >> qstart >> qend)
    {
        sumQ += qend-qstart;
        Q.emplace_back(numQueries, qstart, qend);
        numQueries++;
    }
    fQ.close();
    
    if (settings.selection_sortQueries)
    {
        tim.start();
        sort(Q.begin(), Q.end());
        totalSortTime = tim.stop();
    }

    result = new size_t[numQueries];

    process_mem_usage(vmDQ, rssDQ);
    
    // Build index
    tim.start();
    idxR = new HINT_M_SubsSort_SS_CM(R, settings.hintm_numBits, settings.hintm_maxBits);
    totalIndexTime = tim.stop();
    process_mem_usage(vmI, rssI);

    
    // Execute queries
    for (auto r = 0; r < settings.query_numRuns; r++)
    {
        // Reset results
        memset(result, 0, numQueries*sizeof(size_t));

        switch (settings.selection_strategy)
        {
            case SELECTION_BATCH_PERQUERY:
                tim.start();
                idxR->executeBottomUp_gOverlaps_PerQuery(Q, result);
                totalQueryTime += tim.stop();
                break;
                
            case SELECTION_BATCH_PERLEVEL:
                tim.start();
                idxR->executeBottomUp_gOverlaps_PerLevel(Q, result);
                totalQueryTime += tim.stop();
                break;

            case SELECTION_BATCH_PERPARTITION:
                tim.start();
                idxR->executeBottomUp_gOverlaps_PerPartition(Q, result);
                totalQueryTime += tim.stop();
                break;
        }
        
        // Check result
        for (auto q = 0; q < numQueries; q++)
        {
            totalResult += result[q];
        }
    }
    
    
    // Report
    idxR->getStats();
    cout << endl;
    cout << "Batch Processing Selection Queries" << endl;
    cout << "==================================" << endl << endl;
    cout << "Input info" << endl;
    cout << "File                      : " << settings.input_fileR << endl;
    cout << "Num of intervals          : " << R.size() << endl;
    cout << "Domain size               : " << (R.gend-R.gstart) << endl;
    cout << "Avg interval extent [%]   : "; printf("%f\n", R.avgRecordExtent*100/(R.gend-R.gstart));
    cout << endl;
    cout << "HINT^m info & stats" << endl;
    cout << "Variant                   : subs+sort+SS+CM" << endl;
    cout << "Num of bits               : " << settings.hintm_numBits << endl;
    cout << "Num of partitions         : " << idxR->numPartitions << endl;
    cout << "Num of Originals (In)     : " << idxR->numOriginalsIn << endl;
    cout << "Num of Originals (Aft)    : " << idxR->numOriginalsAft << endl;
    cout << "Num of replicas (In)      : " << idxR->numReplicasIn << endl;
    cout << "Num of replicas (Aft)     : " << idxR->numReplicasAft << endl;
    cout << "Num of empty partitions   : " << idxR->numEmptyPartitions << endl;
    printf( "Avg partition size        : %f\n", idxR->avgPartitionSize);
    printf( "Read VM [Bytes]           : %ld\n", (size_t)(vmI-vmDQ)*1024);
    printf( "Read RSS [Bytes]          : %ld\n", (size_t)(rssI-rssDQ)*1024);
    printf( "Indexing time [secs]      : %f\n", totalIndexTime);
    cout << endl;
    cout << "Queries info" << endl;
    cout << "File                      : " << settings.query_file << endl;
    cout << "Num of queries            : " << numQueries << endl;
    cout << "Avg query extent [%]      : "; printf("%f\n", (((float)sumQ/numQueries)*100)/(R.gend-R.gstart));
    cout << "Predicate type            : gOVERLAPS" << endl;
    cout << "Sorting time [secs]       : " << totalSortTime << endl;
    cout << endl;
    cout << "Evaluation report" << endl;
    cout << "Index traversing approach : bottom-up" << endl;
    cout << "Batch strategy            : " << strStrategy << "-based" << endl;
    cout << "Queries sorted            : " << ((settings.selection_sortQueries) ? "yes": "no") << endl;
    cout << "Num of runs per query     : " << settings.query_numRuns << endl;
    cout << "Total result [";
#ifdef WORKLOAD_COUNT
    cout << "CNT]        : ";
#elif defined(WORKLOAD_XOR)
    cout << "XOR]        : ";
#endif
    cout << totalResult/settings.query_numRuns << endl;
    printf( "Total querying time [secs]: %f\n", totalQueryTime/settings.query_numRuns);
    cout << endl;

    delete idxR;
    delete[] result;


    return 0;
}
