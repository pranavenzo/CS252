#!/bin/bash

pid=$1

# extracts the port number from netstat
netstat -ltp 2> /dev/null | grep "$pid/" | awk '{print $4}' | sed -e 's/.*://'
