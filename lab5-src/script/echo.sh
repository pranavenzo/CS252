printenv | grep "^HTTP" | sort > .echo.tmp

content_length=$(wc -c < .echo.tmp)

printf "HTTP/1.0 200 OK\r\n"
printf "Connection: close\r\n"
printf "Content-Length: $content_length\r\n"
printf "Content-Type: text/plain\r\n\r\n"
cat .echo.tmp
rm -f .echo.tmp
