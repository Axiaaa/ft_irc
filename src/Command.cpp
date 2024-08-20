#include "Server.hpp"
#include "NumericReplies.cpp"

/*
    @brief Send a NICK command to the server
    @param server The server object
    @param client_fd The client file descriptor
    @param buffer The buffer containing the nickname
*/
void nick(Server &server, Client &client, const char *buffer)
{
    std::string nick = "NICK :";
    nick += buffer;
    nick += "\r\n";
    client.setNickname(buffer);
    server.sendData(client.getClientFd(), nick);
}

/*
    @brief Send a USER command to the server
    @param server The server object
    @param client The client object
    @param buffer The buffer containing the username

    Example: 
    nickname!username@localhost USER username 0 * :realname
*/
void user(Server &server, Client &client, const char *buffer)
{   
    std::string user = "USER ";
    std::cout << "Buffer: " << client.getRegistrationStatus() << std::endl;
    // if (client.getRegistrationStatus()) {
    //     server.sendData(client.getClientFd(), getNumericReply(client, 462, "USER", "", "", "")); 
    //     return
    // }
    string space = " ";
    vector<string> buff_split = split(string(buffer), ' ');
    std::cout << "Buffer split size: " << buff_split.size() << std::endl;
    client.init(buffer, server);
    if (buff_split.size() < 4) {
        server.sendData(client.getClientFd(), getNumericReply(client, 461, "USER", "", "", ""));
        return ;
    }
    user += buffer;
    user += "\r\n";
    std::cout << YELLOW << "Sending command: " << user << RESET << std::endl;
    server.sendData(client.getClientFd(), client.getHostname() + user);
}