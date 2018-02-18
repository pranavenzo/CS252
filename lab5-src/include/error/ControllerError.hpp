#ifndef CS252_CONTROLLERERROR_H
#define CS252_CONTROLLERERROR_H

#include <stdexcept>
#include <string>

/**
 * Whenever your Controller enters a state that it does not know how to proceed in,
 * this can be thrown so that Server::handle() can catch it and issue an error response.
**/
class ControllerError : public std::runtime_error 
{
public:
    explicit ControllerError(std::string const& what) : std::runtime_error(what) { }
    explicit ControllerError(char const* what) : std::runtime_error(what) { }
};

#endif