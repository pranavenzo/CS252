#!/bin/bash

set -e

host=$1
port=$2
reqfile=$3
resfile=$4
outfile=$5
cmpfile=$6
cmpflag=$7

nc $host $port < $reqfile > $outfile 2>&1

if [[ "$cmpflag" = "--use-cmp" ]]; then
    cmp --print-bytes --verbose $outfile $resfile > $cmpfile 2>&1
else
    diff -u $outfile $resfile > $cmpfile 2>&1
fi

