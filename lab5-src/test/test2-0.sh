#!/bin/bash

# get the name and path of the current file except for the .sh extension
testcase=${0%\.*}
reqfile="$testcase.req"
resfile="$testcase.org.res"
outfile="$testcase.out.res"
cmpfile="$testcase.cmp"

host=$1
port=$2
verbose=$3

nc $host $port > /dev/null 2>&1 &
ncpid=$!

sleep 0.2

if ps -u $USER | grep $ncpid > /dev/null 2>&1; then
    kill -SIGKILL $ncpid
    wait $ncpid 2> /dev/null
    rm -f $cmpfile
    exit 0
else
    echo "nc could not connect to your server on port $port" > $cmpfile
    exit 1
fi
