#!/bin/bash

testcase=${0%\.*}
expect="$testcase.org"
actual="$testcase.out"
cmpfile="$testcase.cmp"

retval="0"

http="bin/http"
if [[ "$1" = "-e" ]]; then
    http="$2"
    shift
    shift
fi

verbose="$1"

$http --unknown > $actual 2>&1 &
server_pid=$!

sleep 0.2

if ps | grep "[h]ttp" > /dev/null 2>&1; then
    kill $server_pid
    wait $server_pid 2> /dev/null
    echo "Server does not exit on invalid option" > $cmpfile
    retval="1"
fi

if grep "^Configuration error: Unknown option" > /dev/null < $actual; then
    retval="0"
else
    echo "Server prints incorrect error message" > $cmpfile
    echo "Line should start with 'Configuration error: Unknown option'" >> $cmpfile
    retval="1"
fi

if [[ "$verbose" != "-v" ]]; then
    rm -f $expect $actual
    if [[ "$retval" = "0" ]]; then
        rm -f $cmpfile
    fi
fi

exit $retval
