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

printf "PUT /hello-world HTTP/1.0\r\n" > $reqfile
printf "\r\n" >> $reqfile

printf "HTTP/1.0 405 Method Not Allowed\r\n" > $resfile
printf "Connection: close\r\n" >> $resfile
printf "Content-Length: 28\r\n" >> $resfile
printf "Content-Type: text/plain\r\n" >> $resfile
printf "\r\n" >> $resfile
printf "Error while parsing request\n" >> $resfile

$(dirname $0)/run-http-test.sh $host $port $reqfile $resfile $outfile $cmpfile
success=$?

if [[ "$verbose" != "-v" ]]; then
    rm -f $reqfile $resfile $outfile
    if [[ "$success" = "0" ]]; then
        rm -f $cmpfile
    fi
fi

exit $success
