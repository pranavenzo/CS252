#!/bin/bash

# fail if any commands here fail
set -e

# we want to run all tests from the root of the repo
cd "$(git rev-parse --show-toplevel)"

ansi_red="\x1b[31m"
ansi_green="\x1b[32m"
ansi_clear="\x1b[0m"

total_cases="0"
total_pass="0"

host="127.0.0.1"
http="bin/http"
port="0"
attach="false"
old_path="$PATH"

export PATH="./:${PATH}"

if ! ps -fu $USER | grep "[b]in/http" > /dev/null 2>&1; then
    make server
fi

if [[ "$1" = "-h" || "$1" = "--host" ]]; then
    attach="true"
    host=$(echo $2 | cut -d : -f 1)
    port=$(echo $2 | cut -d : -f 2)
    echo "Using server on $host:$port for testing"
    shift
    shift
fi

if [[ "$1" = "-e" || "$1" = "--executable" ]]; then
    http="$2"
    echo "Using $2 instead of bin/http for testing"
    shift
    shift
fi

verbose=""
if [[ "$1" = "-v" || "$1" = "--verbose" ]]; then
    verbose="-v"
    shift
fi

# stop exiting on failure
set +e

function run_http_step {
    local step_total="step$1_cases"
    local step_pass="step$1_pass"
    local step_regex="test$1-*.sh"

    local local_total="0"
    local local_pass="0"

    echo "===== Running step $1 tests ====="

    local server_pid="0"
    local server_port="0"

    if [[ $attach = "true" ]]; then
        echo "Running against server on $host:$port"
        server_port=$port
    else
        $http --port $port --verbose > http-out 2>&1 &
        server_pid=$!

        sleep 0.5

        if ! ps -fu $USER | grep $server_pid | grep -v grep > /dev/null 2>&1; then
            echo "Could not start server!"
            exit 1
        fi

        server_port=$(./get-port.sh $server_pid)
        if [[ -z $server_port ]]; then
            echo "Server PID $server_pid is not listening on a port!"
            exit 1
        fi

        echo "Started server on port $server_port with pid $server_pid"
    fi

    for testscript in $(find test -name "$step_regex" | sort); do
        ((local_total += 1))
        ((total_cases += 1))

        testpath=${testscript%\.*}
        testcase=$(basename $testpath)

        printf "Running $testcase..."

        if ! ps -fu $USER | grep $server_pid | grep -v grep > /dev/null 2>&1; then
            echo -e "Server not running!$ansi_red FAILED$ansi_clear"
            continue
        fi

        if [[ -z $server_port ]]; then
            echo -e "Could not find port of server $server_pid!$ansi_red FAILED$ansi_clear"
            continue
        fi

        ./$testscript $host $server_port $verbose

        if [[ "$?" = "0" ]]; then
            echo -e "$ansi_green OK$ansi_clear"
            ((local_pass += 1))
            ((total_pass += 1))
        else
            echo -e "$ansi_red FAILED$ansi_clear see $testcase.cmp for full diff"
        fi
    done

    if [[ $attach = "false" && -n "$server_port" ]]; then
        kill $server_pid
        wait $server_pid 2> /dev/null
    fi

    # return using the variable names passed in
    eval $step_total="$local_total"
    eval $step_pass="$local_pass"
}

function run_step {
    local step_total="step$1_cases"
    local step_pass="step$1_pass"
    local step_regex="test$1-*.sh"

    local local_total="0"
    local local_pass="0"

    echo "===== Running step $1 tests ====="

    for testscript in $(find test -name "$step_regex" | sort); do
        ((local_total += 1))
        ((total_cases += 1))

        testpath=${testscript%\.*}
        testcase=$(basename $testpath)

        printf "Running $testcase..."

        ./$testscript -e $http $verbose

        if [[ "$?" = "0" ]]; then
            echo -e "$ansi_green OK$ansi_clear"
            ((local_pass += 1))
            ((total_pass += 1))
        else
            echo -e "$ansi_red FAILED$ansi_clear see $testcase.cmp for full diff"
        fi
    done

    # return using the variable names passed in
    eval $step_total="$local_total"
    eval $step_pass="$local_pass"
}

function run1 { run_step 1; }
function run2 { run_http_step 2; }
function run3 { run_step 3; }
function run4 { run_http_step 4; }
function run5 { run_step 5; }
function run6 { run_http_step 6; }

if [[ $# -gt 0 ]]; then
    while [[ $# -gt 0 ]]; do
        run$1
        shift
    done
else
    run1
    run2
    run3
    run4
    run5
    run6
fi


echo
echo "===== Results ====="
[[ -n "$step1_cases" ]] && echo "Step 1: $step1_pass/$step1_cases"
[[ -n "$step2_cases" ]] && echo "Step 2: $step2_pass/$step2_cases"
[[ -n "$step3_cases" ]] && echo "Step 3: $step3_pass/$step3_cases"
[[ -n "$step4_cases" ]] && echo "Step 4: $step4_pass/$step4_cases"
[[ -n "$step5_cases" ]] && echo "Step 5: $step5_pass/$step5_cases"
[[ -n "$step6_cases" ]] && echo "Step 6: $step6_pass/$step6_cases"
echo "Total: $total_pass/$total_cases"
echo

# reset path
export PATH=$old_path
# exit with error if not 100%
if (( $total_pass != $total_cases )); then
	exit 1
fi

