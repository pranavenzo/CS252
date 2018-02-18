#!/bin/bash

set -e

testcase=${0%\.*}

http="bin/http"
if [[ "$1" = "-e" ]]; then
    http="$2"
    shift
    shift
fi

verbose="$1"

$(dirname $0)/run-valgrind-test.sh $testcase $http -L $verbose