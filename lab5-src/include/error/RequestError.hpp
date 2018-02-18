#ifndef CS252_REQUESTERROR_H
#define CS252_REQUESTERROR_H

#include <stdexcept>
#include <string>

#include "http/HttpStatus.hpp"

/**
 * This error is thrown by the Request class when it can not properly parse an incoming request.
 * It takes an additional HttpStatus argument because different parsing errors require
 * different response statuses.
**/
class RequestError : public std::runtime_error 
{
public:
    HttpStatus const& status;

    explicit RequestError(HttpStatus const& status, std::string const& message) : 
        std::runtime_error(message), 
        status(status)
    {
        
    }
};

#endif