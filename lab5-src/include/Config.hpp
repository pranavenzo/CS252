#ifndef CS252_CONFIG_H
#define CS252_CONFIG_H

#include <string>
#include "Utils.hpp"

class Config {
private:
    /**
     * This is a helper function that takes in the return value of getopt_long
     * as well as the global optarg string.
     * This function is useful because 
    **/
    void set_opt(int shortopt, char const* optarg);
public:
    // Each mode is set to the same value as its flag, which can be useful while parsing arguments
    enum Mode { SM_FORK = 'F', SM_LINEAR = 'L', SM_POOLTHREAD = 'P', SM_REQUESTTHREAD = 'R' };

    enum Mode mode = SM_LINEAR;
    bool verbose = false;
    unsigned short port = 0;
    int threads = -1;
    unsigned short queue_length = 5;
    std::string exec_dir = "script";
    std::string static_dir = "static";

    /**
     * You should not have to change this method, as it prints out
     * config options in a standard way that we parse in the step 1 tests
    **/
    void print() const;

    /**
     * :: TODO ::
     * Create the config object by parsing argc and argv.
    **/
    Config(int argc, char** argv);
};
#endif
