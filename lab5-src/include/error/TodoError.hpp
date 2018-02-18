#ifndef CS252_TODOERROR_H
#define CS252_TODOERROR_H

#include <stdexcept>
#include <string>

/**
 * TodoErrors represent some feature that you have yet to implement
**/
class TodoError : public std::runtime_error {
private:
    std::string m_what;
public:
    explicit TodoError(std::string const& step, std::string const& description) :
        std::runtime_error(description),
        m_what("Step ")
    {
        m_what += step + ": " + description;
    }

    virtual char const* what() const noexcept override { return m_what.c_str(); }
};

#endif