# get the name and path of the current file except for the .sh extension
testcase=${0%\.*}
reqfile="$testcase.req"
resfile="$testcase.org.res"
outfile="$testcase.out.res"
cmpfile="$testcase.cmp"

host=$1
port=$2
verbose=$3

printf "GET /script/echo.sh HTTP/1.0\r\n" > $reqfile
printf "X-CS252-Testcase: test6-1\r\n" >> $reqfile
printf "Connection: close\r\n" >> $reqfile
printf "\r\n" >> $reqfile

printf "HTTP/1.0 200 OK\r\n" > $resfile
printf "Connection: close\r\n" >> $resfile
printf "Content-Length: 108\r\n" >> $resfile
printf "Content-Type: text/plain\r\n" >> $resfile
printf "\r\n" >> $resfile
printf "HTTP:HEADER:Connection=close\n" >> $resfile
printf "HTTP:HEADER:X-CS252-Testcase=test6-1\n" >> $resfile
printf "HTTP:METHOD=GET\n" >> $resfile
printf "HTTP:PATH=/script/echo.sh\n" >> $resfile

$(dirname $0)/run-http-test.sh $host $port $reqfile $resfile $outfile $cmpfile
success=$?

if [[ "$verbose" != "-v" ]]; then
    rm -f $reqfile $resfile $outfile
    if [[ "$success" = "0" ]]; then
        rm -f $cmpfile
    fi
fi

exit $success
