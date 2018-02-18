#ifndef CS252_SENDFILECONTROLLER_H
#define CS252_SENDFILECONTROLLER_H

#include <string>
#include <fstream>

#include "Config.hpp"
#include "controller/Controller.hpp"
#include "server/Request.hpp"
#include "server/Response.hpp"

class SendFileController : public Controller
{
private:
    /**
     * :: TODO ::
     * Gets the content length of a filestream.
     * Whatever this returns can be used when setting the Content-Length header.
    **/
    int get_content_length(std::fstream& fs) const;

    /**
     * Gets the content type of a given filename.
     * Whatever this returns can be used when setting the Content-Type header.
     * In order to do this, you need to call the `xdg-mime` command by fork()ing,
     * exec()ing, and then reading its output through a pipe, similar to lab 3.
     * Luckily for you, we have given you this code.
     * Use it wisely.
    **/
    std::string get_content_type(std::string const& filename) const;
public:
    /**
     * Initializes the SendFileController with the given config.
    **/
    SendFileController(Config const& config);

    /**
     * :: TODO ::
     * Looks at the path in req and tries to open it, read its contents, and send
     * its contents using res.send()
     * To figure out exactly what file to open, consider implementing the logic in
     * Controller::resolve_requested_path() so that you can use the same logic for
     * the extra credit portion of the lab.
    **/
    void run(Request const& req, Response& res) const override;
};

#endif