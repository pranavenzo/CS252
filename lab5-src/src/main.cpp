#include <iostream>
#include <signal.h>

#include "Config.hpp"
#include "server/Server.hpp"
#include "controller/SendFileController.hpp"
#include "controller/TextController.hpp"
#include "error/ConfigError.hpp"
#include "error/SocketError.hpp"
#include "error/ConnectionError.hpp"
#include "error/TodoError.hpp"

int main(int argc, char** argv)
{
    // You will get SIGPIPEs when working with browsers
    // we can safely ignore them.
    signal(SIGPIPE, SIG_IGN);

    try 
    {
        Config config(argc, argv);
        Server server(config);

        config.print();

        if (config.mode == Config::SM_LINEAR)
        {
            server.run_linear();
        }
        else if (config.mode == Config::SM_REQUESTTHREAD)
        {
            server.run_thread_request();
        }
        else if (config.mode == Config::SM_FORK)
        {
            server.run_fork();
        }
        else if (config.mode == Config::SM_POOLTHREAD)
        {
            server.run_thread_pool();
        }
        else
        {
            throw ConfigError("Unsupported server runtime option");
        }

        return 0;
    }
    catch (ConfigError const& e)
    {
        std::cerr << "Configuration error: " << e.what() << std::endl;
    }
    catch (SocketError const& e)
    {
        std::cerr << "Socket error: " << e.what() << std::endl;
    }
    catch (TodoError const& e)
    {
        std::cerr << "You missed a todo: " << e.what() << std::endl;
    }
    catch (std::runtime_error const& e)
    {
        std::cerr << "Unknown error: " << e.what() << std::endl;
    }

    return 1;
}
