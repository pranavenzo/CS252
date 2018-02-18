#ifndef CS252_RESPONSE_H
#define CS252_RESPONSE_H

#include <string>
#include <cstring>
#include <map>

#include "server/TcpConnection.hpp"
#include "http/HttpStatus.hpp"
#include "Config.hpp"

class Response
{
private:
    Config const& m_config;
    TcpConnection& m_conn;
    bool m_headers_sent;
    std::string m_status_text;

    /**
     * We want to use a std::map here instead of a std::unordered_map,
     * which is what most of the rest of the skeleton code suggests using
     * This is so that we can maintain deterministic ordering for testing
    **/
    std::map<std::string, std::string> m_headers;

    void send_headers();
public:
    /**
     * The Response constructor doesn't do much of anything on its own.
    **/
    Response(Config const& config, TcpConnection& conn);

    /**
     * :: TODO ::
     * Called by controllers to set an arbitrary header on the response.
    **/
    void set_header(std::string const& key, std::string const& value);

    /**
     * Called by controllers to set the status of a response.
     * Controllers must call this before they can send a response.
    **/
    void set_status(HttpStatus const& status);

    /**
     * :: TODO ::
     * Sends the body of the response.
     * The body is a void const* because it could be text or binary data, this method
     * does not need to know or care.
     * Callers would only call with raw = true when they do not want to flush the
     * response's headers (such as for extra credit, where the scripts write their own headers)
    **/
    void send(void const* buf, size_t size, bool raw = false);
};

#endif
