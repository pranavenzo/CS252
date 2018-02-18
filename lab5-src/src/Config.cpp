#include <iostream>
#include <getopt.h>
#include <sys/socket.h> // for SOMAXCONN

#include "Config.hpp"
#include "error/ConfigError.hpp"

void Config::set_opt(int shortopt, char const* optarg)
{
    switch (shortopt)
    {
    case 'v':
        verbose = true;
        break;
    case 'p':
        port = strtol(optarg, NULL, 10);
        if (port <= 1024 && port != 0)
        {
            throw ConfigError("Invalid port");
        }
        break;
    case 'e':
        exec_dir = optarg;
        break;
    case 's':
        static_dir = optarg;
        break;
    case 'q':
        queue_length = strtol(optarg, NULL, 10);
        if (queue_length < 1 || queue_length > SOMAXCONN)
        {
            throw ConfigError("Invalid queue length");
        }
        break;
    case 'P':
        threads = strtol(optarg, NULL, 10);
        if (threads < 1 || threads > 10)
        {
            throw ConfigError("Invalid number of threads for a thread pool");
        }

        // fall through
    case 'F':
    case 'R':
    case 'L':
        mode = (Config::Mode) shortopt;
        break;
    case '?':
        throw ConfigError(std::string("Unknown option ") + (char) optopt);
    default:
        throw ConfigError("Could not parse command-line options");
    }
}

Config::Config(int argc, char** argv)
{
    // turn off getopt's own error messages:
    opterr = 0;

    struct option cl_options[] = {
        {"verbose", no_argument, 0, 'v'},
        {"port", required_argument, 0, 'p'},
        {"exec-dir", required_argument, 0, 'e'},
        {"static-dir", required_argument, 0, 's'},
        {"queue-length", required_argument, 0, 'q'},
        {"fork", no_argument, 0, 'F'},
        {"pool-thread", required_argument, 0, 'P'},
        {"request-thread", no_argument, 0, 'R'},
        {"linear", no_argument, 0, 'L'},
        {0, 0, 0, 0}
    };
    int cl_option_index;
    int shortopt;

    char const* gopt_fmt = "vp:e:s:q:FP:RL";

    while ((shortopt = getopt_long(argc, argv, gopt_fmt, cl_options, &cl_option_index)) != -1)
    {
        if (shortopt == -1)
        {
            // no more arguments
            break;
        }
        else if (shortopt == 0)
        {
            set_opt(cl_options[cl_option_index].val, optarg);
        }
        else
        {
            set_opt(shortopt, optarg);
        }
    }
}

void Config::print() const
{
    std::cout << "Config:" << std::endl;
    if (port == 0)
    {
        std::cout << "\tPort: (dynamic)" << std::endl;
    }
    else
    {
        std::cout << "\tPort: " << port << std::endl;
    }
    std::cout << "\tExecutables directory: " << exec_dir << std::endl;
    std::cout << "\tStatic directory: " << static_dir << std::endl;
    std::cout << "\tVerbose: " << (verbose ? "true" : "false") << std::endl;
    std::cout << "\tQueue length: " << queue_length << std::endl;
    if (mode == SM_POOLTHREAD)
    {
        std::cout << "\tMode: pool of threads (" << threads << " total)" << std::endl;
    }
    else if (mode == SM_REQUESTTHREAD)
    {
        std::cout << "\tMode: thread-per-request" << std::endl;
    }
    else if (mode == SM_FORK)
    {
        std::cout << "\tMode: process-per-request" << std::endl;
    }
    else
    {
        std::cout << "\tMode: linear/iterative" << std::endl;
    }
}
