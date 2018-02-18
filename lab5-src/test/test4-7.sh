#!/bin/bash

root=$(git rev-parse --show-toplevel)

# get the name and path of the current file except for the .sh extension
testcase=${0%\.*}
reqfile="$testcase.req"
resfile="$testcase.org.res"
outfile="$testcase.out.res"
cmpfile="$testcase.cmp"

host=$1
port=$2
verbose=$3

printf "GET /../src/controller/../../static/index.html HTTP/1.0\r\n" > $reqfile
printf "\r\n" >> $reqfile

printf "HTTP/1.0 200 OK\r\n" > $resfile
printf "Connection: close\r\n" >> $resfile
printf "Content-Length: 8162\r\n" >> $resfile
printf "Content-Type: text/html\r\n" >> $resfile
printf "\r\n" >> $resfile
cat $root/static/index.html >> $resfile

$(dirname $0)/run-http-test.sh $host $port $reqfile $resfile $outfile $cmpfile
success=$?

if [[ "$verbose" != "-v" ]]; then
    rm -f $reqfile $resfile $outfile
    if [[ "$success" = "0" ]]; then
        rm -f $cmpfile
    fi
fi

exit $success
