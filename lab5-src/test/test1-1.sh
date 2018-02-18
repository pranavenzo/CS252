#!/bin/bash

testcase=${0%\.*}
expect="$testcase.org"
actual="$testcase.out"
cmpfile="$testcase.cmp"

http="bin/http"
if [[ "$1" = "-e" ]]; then
    http="$2"
    shift
    shift
fi

verbose="$1"

echo -e "Config:" > $expect
echo -e "\tPort: (dynamic)" >> $expect
echo -e "\tExecutables directory: script" >> $expect
echo -e "\tStatic directory: static" >> $expect
echo -e "\tVerbose: false" >> $expect
echo -e "\tQueue length: 5" >> $expect
echo -e "\tMode: linear/iterative" >> $expect

$http > $actual 2>&1 &
server_pid=$!

sleep 0.2

if [[ $(ps -u $USER | grep $server_pid | wc -l) = "1" ]]; then
    kill $server_pid
    wait $server_pid 2> /dev/null
fi

cat $actual | head -n $(wc -l $expect) > $actual

diff -u $expect $actual > $cmpfile
success=$?

if [[ "$verbose" != "-v" ]]; then
    rm -f $expect $actual
    if [[ "$success" = "0" ]]; then
        rm -f $cmpfile
    fi
fi

exit $success
