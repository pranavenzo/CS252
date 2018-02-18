#ifndef CS252_REQUEST_H
#define CS252_REQUEST_H

#include <unordered_map>
#include <string>

#include "Config.hpp"
#include "server/TcpConnection.hpp"

class Request
{
public:
    /**
     * The Request constructor kicks off the parsing of the incoming request.
    **/
    Request(Config const& config, TcpConnection& conn);

    /**
     * Request::print() is a convenience method that prints the method, path, and version
     * of a request in a consistent way.
     * You can use it wherever you see fit, such as every
     * time you parse a new request and want to log it to your server's stdout
    **/
    void print() const noexcept;

    /**
     * Convenience method to check if a header exists and store it in value if it does
    **/
    bool try_header(std::string const& key, std::string& value) const noexcept;

    /**
     * Accessor methods that all simply return the corresponding request member variable.
     * Return const references to avoid copying if possible
    **/
    std::string const& get_path() const noexcept;
    std::string const& get_method() const noexcept;
    std::string const& get_version() const noexcept;
    std::unordered_map<std::string, std::string> const& get_headers() const noexcept;
    std::unordered_map<std::string, std::string> const& get_query() const noexcept;
    std::unordered_map<std::string, std::string> const& get_body() const noexcept;
private:
    Config const& m_config;
    TcpConnection& m_conn;
    std::unordered_map<std::string, std::string> m_headers;
    std::unordered_map<std::string, std::string> m_query;
    std::unordered_map<std::string, std::string> m_body_data;
    std::string m_path;
    std::string m_method;
    std::string m_version;
    int const m_max_buf = 512;

    /**
     * We think its useful to break up the parsing of the method, path, version, and headers
     * into separate methods (also query and body data for the extra credit)
     * When you encounter an unexpected format, you should send a response with the
     * "400 Bad Request" status code except for the specific cases noted below.
    **/

    /**
     * :: TODO ::
     * parse_method() looks for "GET" at the start of the request_line (as well as "POST" 
     * for extra credit)
     * If request_line does not start with either, you will need to send a response with
     * the "405 Method Not Allowed" status text.
    **/
    void parse_method(std::string& request_line);

    /**
     * :: TODO ::
     * parse_route() looks for a string starting with a "/" at the beginning of the request_line.
     * If that does not exist, you will need to send a response with the "400 Bad Request" status.
     * If you do the extra credit, this is where you can call parse_querystring() with the
     * start of the parsed path after the first "?" to fill in the m_query map
    **/
    void parse_route(std::string& request_line);

    /**
     * :: TODO ::
     * This is also where you can parse both query parameters and body data 
     * encoded as x-www-form-urlencoded.
     * This method takes a map argument because its used for parsing both route query strings
     * as well as body data.
     * The correct map should be passed in by reference by the caller.
    **/
    void parse_querystring(std::string query, std::unordered_map<std::string, std::string>& parsed);

    /**
     * :: TODO ::
     * parse_version() takes the rest of request_line and looks for an exact match for
     * either "HTTP/1.0" or "HTTP/1.1".
     * If it does not find this, you should send a response with the "505 HTTP Version Not Supported"
     * status code
    **/
    void parse_version(std::string& request_line);

    /**
     * :: TODO ::
     * parse_headers() will keep reading lines until it finds a line of length 0 (if parse_raw_line()
     * cuts off the trailing \r\n) or of length 2 otherwise.
     * For each line it reads, it should parse the header in that line and add it to m_headers
    **/
    void parse_headers();

    /**
     * :: TODO ::
     * parse_body() only needs to be implemented for extra credit.
     * If the request is a POST request *and* if the Content-Type of the request is
     * "application/x-www-form-urlencoded", read in the entire rest of the request.
     * You can assume that requests will contain a valid Content-Length header,
     * which will tell you how large to make your buffer.
     * If the method is GET, there will never be a body, but this method should just no-op
     * If Content-Length is greater than 4096, you should send a response with the
     * "403 Forbidden" status code
     * If the Content-Type is not "application/x-www-form-urlencoded", you should send a
     * response with the "415 Unsupported Media Type" status code
    **/
    void parse_body();

    /**
     * :: TODO ::
     * Even if you do not use the parse_{method|path|version|headers} methods, we highly
     * recommend implementing this one.
     * This method should be the only place where m_conn.getc() should be called.
     * It can build up a buffer of up to m_max_buf characters until it finds a \r
     * followed by an \n.
     * When it finds this or hits m_max_buf-1 characters read, it returns a std::string
     * version of the buffer for use by other internal methods.
     * You may choose to either return a string that contains the trailing \r\n or not.
     * If you choose to do so, make sure you account for it in the rest of your code!
    **/
    std::string parse_raw_line();
};

#endif
