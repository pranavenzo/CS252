#!/bin/bash

testcase=${0%\.*}
serverout="$testcase.server.out"
telnetout="$testcase.telnet.out"
cmpfile="$testcase.cmp"

http="bin/http"
if [[ "$1" = "-e" ]]; then
    http="$2"
    shift
    shift
fi

verbose="$1"

# kill any lingering background http processes
ps -C $(basename $http) -o pid= | xargs kill -SIGKILL > /dev/null 2>&1
ps -C nc -o pid= | xargs kill -SIGKILL > /dev/null 2>&1

$http -R > $serverout 2>&1 &
server_pid=$!

sleep 0.2

if ! ps -p $server_pid > /dev/null 2>&1; then
    echo "Could not start server" > $cmpfile
    exit 1
fi

port=$(get-port.sh $server_pid)

function make_netcat {
    isalive=$(ps -C $(basename $http) -o pid,uname | grep $USER | wc -l)

    if [[ $isalive != "1" ]]; then
        echo "Server crashed before netcat $1 could start"
        exit 1
    fi

    nc 127.0.0.1 $port &
    eval nc$1=$!
}

# start netcats
make_netcat 1
make_netcat 2
make_netcat 3
make_netcat 4

sleep 0.5

# count processes and threads
proccount=$(ps -C $(basename $http) -o pid,uname | grep $USER | wc -l)
threadcount=$(ps -C $(basename $http) -o nlwp,uname | grep $USER | awk '{print $1}')
ret=0

if [[ $proccount != "1" || $threadcount != "5" ]]; then
    echo "Expected 1 process and 5 threads; got $proccount processes and $threadcount threads" > $cmpfile
    ret=1
fi

for pid in $nc1 $nc2 $nc3 $nc4 $server_pid; do
    if ! ps -fu $USER | grep $pid | grep -v grep > /dev/null 2>&1; then
        echo "PID $pid crashed before we could kill it"
        exit 1
    fi

    kill -SIGKILL $pid
    wait $pid 2> /dev/null
    sleep 0.1
done

if [[ "$verbose" != "-v" ]]; then
    rm -f $serverout
    if [[ "$ret" = "0" ]]; then
        rm -f $cmpfile
    fi
fi

exit $ret
