/******************************************************************************
 * Project:  hint
 * Purpose:  Indexing interval data
 * Author:   Panagiotis Bouros, pbour@github.io
 * Author:   Nikos Mamoulis
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

#ifndef _HINT_M_H_
#define _HINT_M_H_

#include "../def_global.h"
#include "../containers/relation.h"
#include "../containers/offsets.h"
#include "../containers/offsets_templates.cpp"
#include "../indices/hierarchicalindex.h"
#include <boost/dynamic_bitset.hpp>


// HINT^m with subs+sort, skewness & sparsity optimizations and cash misses activated, from VLDB Journal
class HINT_M_SubsSort_SS_CM : public HierarchicalIndex
{
private:
    Relation      *pOrgsInTmp;
    Relation      *pOrgsAftTmp;
    Relation      *pRepsInTmp;
    Relation      *pRepsAftTmp;
    
    RecordId      **pOrgsIn_sizes, **pOrgsAft_sizes;
    size_t        **pRepsIn_sizes, **pRepsAft_sizes;
    RecordId      **pOrgsIn_offsets, **pOrgsAft_offsets;
    size_t        **pRepsIn_offsets, **pRepsAft_offsets;

    
    // Construction
    inline void updateCounters(const Record &r);
    inline void updatePartitions(const Record &r);
    
    // Querying
    // Auxiliary functions to determine exactly how to scan a partition.
    inline bool getBounds(unsigned int level, Timestamp t, PartitionId &next_from, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, vector<pair<Timestamp, Timestamp> >::iterator &iterStart, vector<pair<Timestamp, Timestamp> >::iterator &iterEnd, RelationIdIterator &iterI);
    inline bool getBounds(unsigned int level, Timestamp t, PartitionId &next_from, Offsets_SS_CM *ioffsets, RelationId *ids, RelationIdIterator &iterIStart, RelationIdIterator &iterIEnd);
    inline bool getBoundsS(unsigned int level, Timestamp t, PartitionId &next_from, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, RelationIdIterator &iterIStart, RelationIdIterator &iterIEnd);
    inline bool getBoundsE(unsigned int level, Timestamp t, PartitionId &next_from, Offsets_SS_CM *ioffsets, RelationId *ids, vector<pair<Timestamp, Timestamp> > *timestamps, RelationIdIterator &iterIStart, RelationIdIterator &iterIEnd);
    inline bool getBounds(unsigned int level, Timestamp ts, Timestamp te, PartitionId &next_from, PartitionId &next_to, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, vector<pair<Timestamp, Timestamp> >::iterator &iterStart, vector<pair<Timestamp, Timestamp> >::iterator &iterEnd, RelationIdIterator &iterI);
    inline bool getBounds(unsigned int level, Timestamp ts, Timestamp te, PartitionId &next_from, PartitionId &next_to, Offsets_SS_CM *ioffsets, RelationId *ids, RelationIdIterator &iterIStart, RelationIdIterator &iterIEnd);
    
    inline bool getBounds_PerPartition(unsigned int level, Timestamp t, PartitionId &next_from, Offsets_SS_CM *ioffsets, RelationId *ids, RelationIdIterator &iterIBegin, RelationIdIterator &iterIEnd);
    inline bool getBounds_PerPartition(unsigned int level, Timestamp t, PartitionId &next_from, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, vector<pair<Timestamp, Timestamp> >::iterator &iterBegin, vector<pair<Timestamp, Timestamp> >::iterator &iterEnd, RelationIdIterator &iterI);
    inline bool getBounds_PerPartition(unsigned int level, Timestamp t, PartitionId &next_from, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, RelationId *ids, vector<pair<Timestamp, Timestamp> >::iterator &iterBegin, vector<pair<Timestamp, Timestamp> >::iterator &iterEnd, RelationIdIterator &iterIBegin, RelationIdIterator &iterIEnd);

    
    // Auxiliary functions to scan a partition.
    inline void scanPartition_CheckBothTimestamps_Equals(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckBothTimestamps_Starts(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckStart_Starts(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckBothTimestamps_Started(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckBothTimestamps_Finishes(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckBothTimestamps2_Finishes(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckEnd_Finishes(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckBothTimestamps_Finished(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckBothTimestamps2_Finished(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckEnd_Finished(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckEnd_Met(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckAllConditions_Overlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckTwoConditions1_Overlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckTwoConditions2_Overlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckOneCondition_Overlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamp, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckTwoConditions_Overlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckTwoConditions3_Overlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartitions_CheckOneCondition_Overlaps(unsigned int level, Timestamp ts, Timestamp te, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, Timestamp qt, PartitionId &next_from, PartitionId &next_to, size_t &result);
    inline void scanPartition_CheckOneCondition2_Overlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, Timestamp qt, PartitionId &next_from, size_t &result);
  inline void scanPartition_CheckAllConditions_Overlapped(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckTwoConditions1_Overlapped(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckTwoConditions2_Overlapped(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckTwoConditions3_Overlapped(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckTwoConditions4_Overlapped(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckOneCondition_Overlapped(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckOneCondition_Overlapped(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckEnd_Overlapped(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartitions_CheckOneCondition_Overlapped(unsigned int level, Timestamp ts, Timestamp te, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, Timestamp qt, PartitionId &next_from, PartitionId &next_to, size_t &result);
    inline void scanPartition_CheckBothTimestamps_Contains(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckStart_Contains(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartitions_CheckEnd_Contains(unsigned int level, Timestamp ts, Timestamp te, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, Timestamp qt, PartitionId &next_from, PartitionId &next_to, size_t &result);
    inline void scanPartition_CheckBothTimestamps_Contained(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckStart_Contained(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckEnd_Contained(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);

    inline void scanPartition_Precedes(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartitions_Precedes(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, PartitionId &next_from, size_t &result);
    inline void scanPartition_Preceded(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_Preceded(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartitions_Preceded(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, RelationId *ids, vector<pair<Timestamp, Timestamp> > *timestamps, PartitionId &next_from, size_t &result);

    inline void scanPartition_CheckBothTimestamps_gOverlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckEnd_gOverlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckEnd_gOverlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_CheckStart_gOverlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, size_t &result);
    inline void scanPartition_NoChecks_gOverlaps(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, RelationId *ids, PartitionId &next_from, size_t &result);
    inline void scanPartitions_NoChecks_gOverlaps(unsigned int level, Timestamp ts, Timestamp te, Offsets_SS_CM *ioffsets, RelationId *ids, PartitionId &next_from, PartitionId &next_to, size_t &result);

    // For batch processing
    inline void scanPartition_NoChecks_gOverlaps_PerPartition(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, RelationId *ids, PartitionId &next_from, vector<pair<Timestamp, Timestamp> >::iterator &iterBegin, vector<pair<Timestamp, Timestamp> >::iterator &iterEnd, RelationIdIterator &iterIBegin, RelationIdIterator &iterIEnd, size_t &result);
    inline void scanPartition_CheckBothTimestamps_gOverlaps_PerPartition(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, RelationId *ids, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qstart, Timestamp qend, PartitionId &next_from, vector<pair<Timestamp, Timestamp> >::iterator &iterBegin, vector<pair<Timestamp, Timestamp> >::iterator &iterEnd, RelationIdIterator &iterIBegin, RelationIdIterator &iterIEnd, size_t &result);
    inline void scanPartition_CheckStart_gOverlaps_PerPartition(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, RelationId *ids, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, vector<pair<Timestamp, Timestamp> >::iterator &iterBegin, vector<pair<Timestamp, Timestamp> >::iterator &iterEnd, RelationIdIterator &iterIBegin, RelationIdIterator &iterIEnd, size_t &result);
    inline void scanPartition_CheckEnd_gOverlaps_PerPartition(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, RelationId *ids, Timestamp qt, PartitionId &next_from, vector<pair<Timestamp, Timestamp> >::iterator &iterBegin, vector<pair<Timestamp, Timestamp> >::iterator &iterEnd, RelationIdIterator &iterIBegin, RelationIdIterator &iterIEnd, size_t &result);
    inline void scanPartition_CheckEnd_gOverlaps_PerPartition(unsigned int level, Timestamp t, Offsets_SS_CM *ioffsets, vector<pair<Timestamp, Timestamp> > *timestamps, RelationId *ids, bool (*compare)(const pair<Timestamp, Timestamp>&, const pair<Timestamp, Timestamp>&), Timestamp qt, PartitionId &next_from, vector<pair<Timestamp, Timestamp> >::iterator &iterBegin, vector<pair<Timestamp, Timestamp> >::iterator &iterEnd, RelationIdIterator &iterIBegin, RelationIdIterator &iterIEnd, size_t &result);


public:
    RelationId    *pOrgsInIds;
    vector<pair<Timestamp, Timestamp> > *pOrgsInTimestamps;
    RelationId    *pOrgsAftIds;
    vector<pair<Timestamp, Timestamp> > *pOrgsAftTimestamps;
    RelationId    *pRepsInIds;
    vector<pair<Timestamp, Timestamp> > *pRepsInTimestamps;
    RelationId    *pRepsAftIds;
    vector<pair<Timestamp, Timestamp> > *pRepsAftTimestamps;
    
    Offsets_SS_CM *pOrgsIn_ioffsets;
    Offsets_SS_CM *pOrgsAft_ioffsets;
    Offsets_SS_CM *pRepsIn_ioffsets;
    Offsets_SS_CM *pRepsAft_ioffsets;

    // Construction
    HINT_M_SubsSort_SS_CM(const Relation &R, const unsigned int numBits, const unsigned int maxBits);
    void getStats();
    void locate(const Record &r);
    ~HINT_M_SubsSort_SS_CM();
    
    // Querying
    size_t executeBottomUp_gOverlaps(RangeQuery Q);
    
    void executeBottomUp_gOverlaps_PerQuery(vector<RangeQuery> Q, size_t *result);
    void executeBottomUp_gOverlaps_PerLevel(vector<RangeQuery> Q, size_t *result);
    void executeBottomUp_gOverlaps_PerPartition(vector<RangeQuery> Q, size_t *result);
};


// Comparators
inline bool CompareTimestampPairsByStart(const pair<Timestamp, Timestamp> &lhs, const pair<Timestamp, Timestamp> &rhs)
{
    return (lhs.first < rhs.first);
}

inline bool CompareTimestampPairsByEnd(const pair<Timestamp, Timestamp> &lhs, const pair<Timestamp, Timestamp> &rhs)
{
    return (lhs.second < rhs.second);
}
#endif // _HINT_M_H_
