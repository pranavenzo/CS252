#!/bin/bash

# fail if any commands here fail
set -e

# we want to run all tests from the root of the repo
cd "$(git rev-parse --show-toplevel)"

export PATH="./:${PATH}"

if ! ps -C "http" -o pid,uname | grep $USER > /dev/null 2>&1; then
    make server
fi

ansi_red="\x1b[31m"
ansi_green="\x1b[32m"
ansi_clear="\x1b[0m"

# stop exiting on failure
set +e

if [[ ! -f test/test$1.sh ]]; then
    echo "Invalid test number $1"
    exit 1
fi

printf "Running test$1..."

./test/test$1.sh $2 $3 $4 $5

if [[ "$?" = "0" ]]; then
    echo -e "$ansi_green OK$ansi_clear"
    exit 0
else
    echo -e "$ansi_red FAILED$ansi_clear see $testcase.cmp for full diff"
    exit 1
fi
