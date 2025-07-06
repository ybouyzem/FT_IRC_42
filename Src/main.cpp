#include "../Includes/Server.hpp"

void SetPortandPassword(char **argv, Server &Server)
{
    if (argv[2] == NULL)
    {
        std::cout << "No password provided" << std::endl;
        exit(1);
    }
    else
    {
        std::string password = argv[2];
        if (password.empty())
        {
            std::cout << "No password provided" << std::endl;
            exit(1);
        }
        else if (password.length() < 8)
        {
            std::cout << "Password too short" << std::endl;
            exit(1);
        }
        Server.setPassword(password);
    }
    if (argv[1] == NULL)
    {
        std::cout << "No port provided" << std::endl;
        exit(1);
    }
    else
    {
        std::string portNumber = argv[1];
        int port = atoi(argv[1]);
        if (port < 1024 || port > 65535 || portNumber.size() > 5 || portNumber.size() < 4)
        {
            std::cout << "Out of range ( 1024 - 65535 ) try again !! " << std::endl;
            exit(1);
        }
        Server.setPort(port);
    }
}

int main(int argc, char **argv) 
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    Server Server;
    SetPortandPassword(argv, Server);
    Server.initializeServer();
    signal(SIGINT, Server::SignalHandler);
	signal(SIGQUIT, Server::SignalHandler);
    Server.run();
    return 0;
}