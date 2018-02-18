#ifndef CS252_CONNECTIONERROR_H
#define CS252_CONNECTIONERROR_H

#include <stdexcept>
#include <string>

/**
 * ConnectionErrors represent any kind of network problem -- being unable to read or write
 * to a connection is usually when this will be thrown.
**/
class ConnectionError : public std::runtime_error {
public:
    explicit ConnectionError(std::string const& what) : std::runtime_error(what) { }
    explicit ConnectionError(char const* what) : std::runtime_error(what) { }
};

#endif