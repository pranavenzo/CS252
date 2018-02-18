#ifndef CS252_RESPONSEERROR_H
#define CS252_RESPONSEERROR_H

#include <stdexcept>
#include <string>

/**
 * This error is thrown by the Response class when it can not properly format a response.
**/
class ResponseError : public std::runtime_error 
{
public:
    explicit ResponseError(std::string const& what) : std::runtime_error(what) { }
    explicit ResponseError(char const* what) : std::runtime_error(what) { }
};

#endif