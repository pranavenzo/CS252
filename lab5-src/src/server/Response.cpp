#include <string>
#include <cstring>
#include <map>
#include <unistd.h>

#include "server/Response.hpp"
#include "server/TcpConnection.hpp"
#include "http/HttpStatus.hpp"
#include "error/ResponseError.hpp"
#include "error/TodoError.hpp"
#include "Config.hpp"
using namespace std;
Response::Response(Config const& config, TcpConnection& conn) :
    m_config(config),
    m_conn(conn),
    m_headers_sent(false)
{
    // We want every response to have this header
    // It tells browsers that we want separate connections per request
    m_headers["Connection"] = "close";
}

void Response::send(void const* buf, size_t bufsize, bool raw)
{
    string outp("");
    outp = outp + "HTTP/1.0 " + m_status_text + "\r\n";
    m_conn.putbuf(outp.c_str(),outp.size());
    send_headers();
    string out("\r\n");
    m_conn.putbuf(out.c_str(),out.size());
    m_conn.putbuf(buf,bufsize);
    //throw TodoError("2", "You need to implement sending responses");
}

void Response::send_headers()
{
    string out("");
    for(auto it = m_headers.begin();it != m_headers.end();it++)
	out = out + it->first + ": " + it->second +"\r\n";

    m_conn.puts(out);

    //throw TodoError("2", "You need to implement sending headers");
}

void Response::set_header(std::string const& key, std::string const& value)
{
    m_headers[key] = value;
    //throw TodoError("2", "You need to implement controllers setting headers");
}

void Response::set_status(HttpStatus const& status)
{
    m_status_text = status.to_string();
}
