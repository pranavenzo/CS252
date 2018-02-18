#ifndef CS252_SOCKETERROR_H
#define CS252_SOCKETERROR_H

#include <stdexcept>
#include <string>
#include <errno.h>
#include <cstring> // for strerror

/**
 * This error should be thrown whenever one of the socket functions fails
 * You should provide the function name that failed instead of a generic message,
 * as this class will format the error response similar to perror()
**/
class SocketError : public std::runtime_error 
{
private:
    std::string m_what;
public:
    explicit SocketError(std::string const& func_name) :
        std::runtime_error(func_name)
    {
        m_what = func_name + "(): " + strerror(errno);
    }

    explicit SocketError(char const* func_name) :
        std::runtime_error(func_name)
    {
        m_what = std::string(func_name) + "(): " + strerror(errno);
    }

    virtual char const* what() const noexcept override { return m_what.c_str(); }
};

#endif