#include <string>

#include "Config.hpp"
#include "controller/Controller.hpp"
#include "controller/TextController.hpp"
#include "server/Request.hpp"
#include "server/Response.hpp"
#include "http/HttpStatus.hpp"

TextController::TextController(Config const& config, std::string const& response) :
    Controller(config),
    m_response(response)
{

}

void TextController::run(Request const& req, Response& res) const
{
    res.set_status(HttpStatus::Ok);
    res.set_header("Content-Type", "text/plain");
    res.set_header("Content-Length", std::to_string(m_response.size()));

    res.send(m_response.c_str(), m_response.size());
}
