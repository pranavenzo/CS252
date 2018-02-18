#ifndef CS252_TEXTCONTROLLER_H
#define CS252_TEXTCONTROLLER_H

#include <string>

#include "Config.hpp"
#include "controller/Controller.hpp"
#include "server/Request.hpp"
#include "server/Response.hpp"

class TextController : public Controller
{
private:
    std::string m_response;
public:
    /**
     * Initializes a TextController with the given config and response text
    **/
    TextController(Config const& config, std::string const& response);

    /**
     * TextController's run method will always return a 200 OK response
     * with a body of m_response and content type of text/plain
    **/
    void run(Request const& req, Response& res) const override;
};

#endif