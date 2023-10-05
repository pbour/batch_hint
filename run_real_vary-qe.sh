#!/bin/bash

if [ "$#" -lt 4 ]; then
        echo
        echo "Usage: $0 DATASET_PREFIX NUM_BITS STRATEGY NUM_RUNS [SORTED]"
        echo
        exit
fi

ifile="inputs/real/$1.inp"
b=$(echo $3 | tr a-z A-Z)

for qe in 0.01% 0.05% 0.1% 0.5% 1%
do
        qfile="queries/real/$1_qe${qe}_qn10K.qry"
        if [ "$#" -eq 4 ]; then
                ofile="outputs/real/$1_qe${qe}_qn10K_oSUBS+SORT+SS+CM_m$2_qGOVERLAPS_b${b}.out"
                echo "./query_batch.exec -m $2 -b $b -r $4 $ifile $qfile > $ofile"
                ./query_batch.exec -m $2 -b $3 -r $4 $ifile $qfile > $ofile
        elif [ "$#" -eq 5 ] && ([ $5 = "SORTED" ] || [ $5 = "sorted" ]); then
                ofile="outputs/real/$1_qe${qe}_qn10K_oSUBS+SORT+SS+CM_m$2_qGOVERLAPS_b${b}_withSORT.out"
                echo "./query_batch.exec -m $2 -b $b -r $4 -s $ifile $qfile > $ofile"
                ./query_batch.exec -m $2 -b $3 -r $4 -s $ifile $qfile > $ofile
        fi
done

