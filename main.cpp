#include "../include/ircBot.hpp"

uint16_t
valid_port(char *port_arg)
{
    size_t i;

    std::cout << port_arg << std::endl;
    for (i = 0; port_arg[i] ;i++)
    {
        if (!isdigit(port_arg[i]) || i > 5)
        {
            std::cerr << "Error: Port range between 1024 && 65535." << std::endl; 
            return 1;
        }
    }
    uint32_t port = atoi(port_arg);
    if (1024 > port || 65535 < port)
    {
        std::cerr << "Error: Port range between 1024 && 65535." << std::endl; 
        return 1;
    }
    return port;
}

uint16_t
arg_checker(int ac, char **av)
{
    switch (ac)
    {
        case 2 :
            std::cerr <<   "ERROR: "  "IRC Bot missing argument <password>" << std::endl;
            return 1;
        case 3 :
            uint16_t port;

            if (!(port = valid_port(av[1])))
                return 1;
            return port;
        default :
            std::cerr << "ERROR: "  "IRC Bot requare 2 arguments. \n"  "try ./bot <port> <password>" << std::endl;
            return 1;
    }
}

int main (int ac, char **av) {

    uint16_t port;

    if ((port = arg_checker(ac ,av)) != 1)
    {
        try {
            Bot agent; // exeption

            agent.startBot(port);
            agent.authenticate(av[2]);
            agent.listenForCommand();

        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return -10; // e number
        }
        // std::cout << "am here ma nigga" << std::endl;
        //start
    }
    return port;
}