#!/bin/bash

if [ "$#" -lt 3 ]; then
        echo
        echo "Usage: $0 NUM_BITS STRATEGY NUM_RUNS [SORTED]"
        echo
        exit
fi

prefix="synthetic_d128M_n10M_a1.2_s1M"
ifile="inputs/synthetic/${prefix}.inp"
b=$(echo $2 | tr a-z A-Z)

for qn in 5 10 50 100
do
        qfile="queries/synthetic/synthetic_qe0.1%_qn${qn}K.qry"
        if [ "$#" -eq 4 ]; then
                ofile="outputs/synthetic/${prefix}_qe0.1%_qn${qn}K_oSUBS+SORT+SS+CM_m$1_qGOVERLAPS_b${b}.out"
                echo "./query_batch.exec -m $1 -b $b -r $3 $ifile $qfile > $ofile"
                ./query_batch.exec -m $1 -b $2 -r $3 $ifile $qfile > $ofile
        elif [ "$#" -eq 5 ] && ([ $5 = "SORTED" ] || [ $5 = "sorted" ]); then
                ofile="outputs/synthetic/${prefix}_qe0.1%_qn${qn}K_oSUBS+SORT+SS+CM_m$1_qGOVERLAPS_b${b}_withSORT.out"
                echo "./query_batch.exec -m $1 -b $b -r $3 -s $ifile $qfile > $ofile"
                ./query_batch.exec -m $1 -b $2 -r $3 -s $ifile $qfile > $ofile
        fi
done

