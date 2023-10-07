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

for qe in 0.01 0.05 0.5 1
do
        qfile="queries/synthetic/synthetic_d128M_qe${qe}%_qn1K.qry"
        if [ "$#" -eq 3 ]; then
                ofile="outputs/synthetic/${prefix}_qe${qe}%_qn1K_oSUBS+SORT+SS+CM_m$1_qGOVERLAPS_b${b}.out"
                echo "./query_batch.exec -m $1 -b $b -r $3 $ifile $qfile > $ofile"
                ./query_batch.exec -m $1 -b $2 -r $3 $ifile $qfile > $ofile
        elif [ "$#" -eq 4 ] && ([ $4 = "SORTED" ] || [ $4 = "sorted" ]); then
                ofile="outputs/synthetic/${prefix}_qe${qe}%_qn1K_oSUBS+SORT+SS+CM_m$1_qGOVERLAPS_b${b}_withSORT.out"
                echo "./query_batch.exec -m $1 -b $b -r $3 -s $ifile $qfile > $ofile"
                ./query_batch.exec -m $1 -b $2 -r $3 -s $ifile $qfile > $ofile
        fi
done

