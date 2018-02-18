#ifndef CS252_CONTROLLER_H
#define CS252_CONTROLLER_H

#include <string>

#include "Config.hpp"
#include "server/Request.hpp"
#include "server/Response.hpp"
#include "server/TcpConnection.hpp"
#include "http/HttpStatus.hpp"

class Controller
{
protected:
    Config const& m_config;
public:
    /**
     * Initializes the base Controller class with the given config.
    **/
    Controller(Config const& config);

    /**
     * Declares the destructor as virtual.
     * Why would we want to do this?
    **/
    virtual ~Controller();

    /**
     * Declares that the run method for controllers is virtual, but what does the "= 0" mean?
    **/
    virtual void run(Request const& req, Response& res) const = 0;

    /**
     * :: TODO ::
     * Takes in a path from a request, a base directory to look for that path in, and a reference to
     * a string to store the final result in.
     * This can be used for both the SendFileController and for the ExecScriptController.
     * Make sure you use realpath() properly to make sure that requests do not traverse across your filesystem!
    **/
    bool resolve_requested_path(std::string const& requested, std::string const& basedir, std::string& resolved) const noexcept;

    /**
     * These static methods are conveniences that can be used whenever we need to send a basic error response.
     * The first method is for when we already have a response object -- such as in a controller.
     * The second method is for when we do not already have a response object -- 
     * such as when we catch an error in Server::handle()
    **/
    static void send_error_response(Response& res, HttpStatus const& status, std::string response_text);
    static void send_error_response(Config const& config, TcpConnection* conn, HttpStatus const& status, std::string response_text);
};

#endif