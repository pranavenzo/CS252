# get the name and path of the current file except for the .sh extension
testcase=${0%\.*}
reqfile="$testcase.req"
resfile="$testcase.org.res"
outfile="$testcase.out.res"
cmpfile="$testcase.cmp"

host=$1
port=$2
verbose=$3

printf "POST /script/echo.sh?k1=v1&k2=v2&k3=v3 HTTP/1.0\r\n" > $reqfile
printf "Content-Length: 26\r\n" >> $reqfile
printf "Content-Type: application/x-www-form-urlencoded\r\n" >> $reqfile
printf "X-CS252-Test: test6-5\r\n" >> $reqfile
printf "\r\n" >> $reqfile
printf "a=b&key=value&course=cs252" >> $reqfile

printf "HTTP/1.0 200 OK\r\n" > $resfile
printf "Connection: close\r\n" >> $resfile
printf "Content-Length: 273\r\n" >> $resfile
printf "Content-Type: text/plain\r\n" >> $resfile
printf "\r\n" >> $resfile
printf "HTTP:BODY:a=b\n" >> $resfile
printf "HTTP:BODY:course=cs252\n" >> $resfile
printf "HTTP:BODY:key=value\n" >> $resfile
printf "HTTP:HEADER:Content-Length=26\n" >> $resfile
printf "HTTP:HEADER:Content-Type=application/x-www-form-urlencoded\n" >> $resfile
printf "HTTP:HEADER:X-CS252-Test=test6-5\n" >> $resfile
printf "HTTP:METHOD=POST\n" >> $resfile
printf "HTTP:PATH=/script/echo.sh\n" >> $resfile
printf "HTTP:QUERY:k1=v1\n" >> $resfile
printf "HTTP:QUERY:k2=v2\n" >> $resfile
printf "HTTP:QUERY:k3=v3\n" >> $resfile

$(dirname $0)/run-http-test.sh $host $port $reqfile $resfile $outfile $cmpfile
success=$?

if [[ "$verbose" != "-v" ]]; then
    rm -f $reqfile $resfile $outfile
    if [[ "$success" = "0" ]]; then
        rm -f $cmpfile
    fi
fi

exit $success
