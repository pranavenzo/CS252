# get the name and path of the current file except for the .sh extension
testcase=${0%\.*}
reqfile="$testcase.req"
resfile="$testcase.org.res"
outfile="$testcase.out.res"
cmpfile="$testcase.cmp"

host=$1
port=$2
verbose=$3

printf "GET /script/echo.sh?a=b&key=value&course=cs252 HTTP/1.0\r\n" > $reqfile
printf "\r\n" >> $reqfile

printf "HTTP/1.0 200 OK\r\n" > $resfile
printf "Connection: close\r\n" >> $resfile
printf "Content-Length: 102\r\n" >> $resfile
printf "Content-Type: text/plain\r\n" >> $resfile
printf "\r\n" >> $resfile
printf "HTTP:METHOD=GET\n" >> $resfile
printf "HTTP:PATH=/script/echo.sh\n" >> $resfile
printf "HTTP:QUERY:a=b\n" >> $resfile
printf "HTTP:QUERY:course=cs252\n" >> $resfile
printf "HTTP:QUERY:key=value\n" >> $resfile

$(dirname $0)/run-http-test.sh $host $port $reqfile $resfile $outfile $cmpfile
success=$?

if [[ "$verbose" != "-v" ]]; then
    rm -f $reqfile $resfile $outfile
    if [[ "$success" = "0" ]]; then
        rm -f $cmpfile
    fi
fi

exit $success
