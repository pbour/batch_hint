#!/bin/bash

if [ "$#" -lt 3 ]; then
        echo
        echo "Usage: $0 NUM_BITS STRATEGY NUM_RUNS [SORTED]"
        echo
        exit
fi

b=$(echo $2 | tr a-z A-Z)
qfile="queries/synthetic/synthetic_d128M_qe0.1%_qn1K.qry"

for a in 1.01 1.1 1.4 1.8
do
        prefix="synthetic_d128M_n10M_a${a}_s1M"
        ifile="inputs/synthetic/${prefix}.inp"
        if [ "$#" -eq 3 ]; then
                ofile="outputs/synthetic/${prefix}_qe0.1%_qn1K_oSUBS+SORT+SS+CM_m$1_qGOVERLAPS_b${b}.out"
                echo "./query_batch.exec -m $1 -b $b -r $3 $ifile $qfile > $ofile"
                ./query_batch.exec -m $1 -b $2 -r $3 $ifile $qfile > $ofile
        elif [ "$#" -eq 4 ] && ([ $4 = "SORTED" ] || [ $4 = "sorted" ]); then
                ofile="outputs/synthetic/${prefix}_qe0.1%_qn1K_oSUBS+SORT+SS+CM_m$1_qGOVERLAPS_b${b}_withSORT.out"
                echo "./query_batch.exec -m $1 -b $b -r $3 -s $ifile $qfile > $ofile"
                ./query_batch.exec -m $1 -b $2 -r $3 -s $ifile $qfile > $ofile
        fi
done

