#ifndef CS252_TCPCONNECTION_H
#define CS252_TCPCONNECTION_H

#include <string>

#include "Config.hpp"

class TcpConnection
{
private:
    Config const& m_config;
    int m_master;
    int m_conn;
    bool m_shutdown;
public:
    /**
     * :: TODO ::
     * TcpConnection() calls accept() to acquire a child connection
     * which should be saved in m_conn
    **/
    TcpConnection(Config const& config, int master_fd);

    /**
     * Destructors should release resources acquired in the constructor,
     * so we should close m_conn hre
    **/
    ~TcpConnection() noexcept;

    /**
     * Call shutdown on the connection - see `man 2 shutdown`.
     * Think about why we don't do this in the destructor.
    **/
    void shutdown();

    /**
     * :: TODO ::
     * Gets a single byte from m_conn and stores it in *c.
     * If read() says there are no bytes left to be read, return false.
    **/
    bool getc(unsigned char* c);

    /**
     * :: TODO ::
     * Writes a single byte to m_conn.
    **/
    void putc(unsigned char c);

    /**
     * :: TODO ::
     * Convenience method to write an entire string to the connection.
    **/
    void puts(std::string const& str);

    /**
     * :: TODO ::
     * Convenience method to write an entire buffer to the connection.
    **/
    void putbuf(void const* buf, size_t bufsize);
};

#endif