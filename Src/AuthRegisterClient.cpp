#include "../Includes/Server.hpp"

bool Server::authenticateClient(Client *client, const std::vector<std::string>& tokens)
{
    
    if (tokens[0] == "PASS")
    {
        if (tokens.size() < 2)
        {
            respond(client->getClientFd(), ":ircserv 461 :Not enough parameters\r\n");
            client->setStatus(false);
            return false;
        }
        else if (tokens[1] == this->password)
        {
            client->setAuthenticated(true);
            return true;
        }
        else
        {
            respond(client->getClientFd(), ":ircserv 464 :Password incorrect\r\n");
            client->setStatus(false);
            return false;
        }
    }
    else if (inCommandslist(tokens[0]))
    {
        respond(client->getClientFd(), ":ircserv 451 :You have not registered\r\n");
        client->setStatus(false);
        return false;
    }
    else
    {
        respond(client->getClientFd(), ":ircserv 421 :Unknown command\r\n");
        client->setStatus(false);
        return false;
    }
    return true;
}

void Server::registerClient(Client *client, const std::vector<std::string>& tokens)
{
    if (tokens[0] == "NICK" && client->getNickname().empty()) // Nickname Validation
    {
        if (tokens.size() < 2)
        {
            respond(client->getClientFd(), ":ircserv 431 :No nickname given\r\n");
            return;
        }
        else if (nickExists(tokens[1]))
        {
            respond(client->getClientFd(), ":ircserv 433 :Nickname is already in use\r\n");
            return;
        }
        else if(!isValidNickname(tokens[1]))
        {
            respond(client->getClientFd(), ":ircserv 432 :Erroneous nickname\r\n");
            return;
        }
        else
        {
            client->setNickname(tokens[1]);
            return;
        }
      
    }
    else if (tokens[0] == "USER" && client->getUsername().empty()) // USERNAME VALIDATION
    {
        if (tokens.size() < 5)
        {
            respond(client->getClientFd(), ":ircserv 461 USER :Not enough parameters\r\n");
            return;
        }
        else if (tokens.size() > 15)
        {
            respond(client->getClientFd(), ":ircserv 461 USER :Too many parameters\r\n");
            return;
        }
        else if (has_non_printables(tokens[1]) || has_non_printables(tokens[2])
            || has_non_printables(tokens[3]) || has_non_printables(tokens[4]))
        {
            respond(client->getClientFd(), ":ircserv 461 USER :Invalid parameters\r\n");
            return;
        }
        else
        {
            client->setUsername(tokens[1]);
            return;
        }

    }
    else if (inCommandslist(tokens[0]))
    {
        respond(client->getClientFd(), ":ircserv 451 :You have not registered\r\n");
        return;
    }
    else
    {
        respond(client->getClientFd(), ":ircserv 421 :Unknown command\r\n");
        return;
    }
}

void Server::checkRegistration(Client *client)
{
    if (!client->getUsername().empty() && !client->getNickname().empty())
    {
        client->setRegistered(true);
        respond(client->getClientFd(), ":ircserv 001 " + client->getNickname() + " :Welcome to the IRC server\r\n");
        std::cout << BOLD << YELLOW << "[*] Client registered     " 
          << RESET << "| IP: " << CYAN << client->getHostName()
          << RESET << " | FD: " << client->getClientFd() << std::endl;
    }
}