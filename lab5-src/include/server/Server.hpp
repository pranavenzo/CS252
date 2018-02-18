#ifndef CS252_SERVER_H
#define CS252_SERVER_H

#include <string>
#include <thread>
#include <unordered_map>

#include "Config.hpp"
#include "server/TcpConnection.hpp"
#include "controller/Controller.hpp"
#include "http/HttpStatus.hpp"

class Server {
private:
    Config const& m_config;
    int m_master;

    /**
     * :: TODO ::
     * Each of the run* functions below use common logic for handling connected requests,
     * so you should split out that logic into its own function here.
    **/
    void handle(TcpConnection* conn) const;
public:
    /**
     * :: TODO ::
     * The server constructor should acquire and set up the resources it needs,
     * such as a connection to the internet.
    **/
    Server(Config const& config);

    /**
     * :: TODO ::
     * The server destructor should release resources acquired in the constructor.
    **/
    ~Server() noexcept;

    /**
     * :: TODO ::
     * Each of the following are used for their respective server modes
    **/
    void run_linear() const;
    void run_fork() const;
    void run_thread_pool() const;
    void run_thread_request() const;
};

#endif
