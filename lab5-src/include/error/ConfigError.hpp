#ifndef CS252_CONFIGERROR_H
#define CS252_CONFIGERROR_H

#include <stdexcept>
#include <string>

/**
 * ConfigErrors should be used to represent any kind of bad server configuration option
**/
class ConfigError : public std::runtime_error {
public:
    explicit ConfigError(std::string const& what) : std::runtime_error(what) { }
    explicit ConfigError(char const* what) : std::runtime_error(what) { }
};

#endif