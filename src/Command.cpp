#include "Server.hpp"
#include "NumericReplies.cpp"
#include "Utils.hpp"

/*
    @brief Send a NICK command to the server
    @param server The server object
    @param client_fd The client file descriptor
    @param buffer The buffer containing the nickname
*/
void nick(Server &server, Client &client, const char *buffer)
{
    if (!buffer) {
        server.sendData(client.getClientFd(), getNumericReply(client, 431, client.getNickname()));
        return ;
    }
    for (size_t i = 0; buffer[i]; ++i) {
        if (!isAuthorized(buffer[i])) {
            server.sendData(client.getClientFd(), getNumericReply(client, 432, buffer));
            return ;
        }
    }
    for (vector<Client>::iterator it = server.getClientsList().begin(); it != server.getClientsList().end(); it++) {
        if (it->getNickname() == string(buffer)) {
            server.sendData(client.getClientFd(), getNumericReply(client, 433, buffer));
            return ;
            }
    }
    std::string nick = "NICK :";
    nick += buffer;
    server.sendData(client.getClientFd(), client.getHostname() + nick);
    client.setNickname(buffer);
}

/*
    @brief Store the username of the client
    @param server The server object
    @param client_fd The client file descriptor
    @param buffer The buffer containing the username

    If the client is already registered, the server will send a 462 error message.
    If the buffer is less than 4, the server will send a 461 error message.
    Otherwise, the server will send a 001 message.
*/
void user(Server& server, Client& client, const char *buffer)
{   
    vector<string> buff_split = split(string(buffer), ' ');
    if (buff_split.size() < 4) {
        server.sendData(client.getClientFd(), getNumericReply(client, 461, "USER"));
        return ;
    }
    if (client.getRegistrationStatus()) {
        server.sendData(client.getClientFd(), getNumericReply(client, 462, "USER")); 
        return ;
    }
    if (client.getUsername() == "")
        client.setUsername(buff_split[0]);
    server.sendData(client.getClientFd(), getNumericReply(client, 001, ""));
    client.setRegistrationStatus(true);
}


/*
    Command: PRIVMSG
   Parameters: <msgtarget> <text to be sent>

   PRIVMSG is used to send private messages between users, as well as to
   send messages to channels.  <msgtarget> is usually the nickname of
   the recipient of the message, or a channel name.

   The <msgtarget> parameter may also be a host mask (#<mask>) or server
   mask ($<mask>).  In both cases the server will only send the PRIVMSG
   to those who have a server or host matching the mask.  The mask MUST
   have at least 1 (one) "." in it and no wildcards following the last
   ".".  This requirement exists to prevent people sending messages to
   "#*" or "$*", which would broadcast to all users.  Wildcards are the
   '*' and '?'  characters.  This extension to the PRIVMSG command is
   only available to operators.

   Numeric Replies:

           ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY
*/

void privmsg(Server& server, Client& client, const char *buffer) {

    std::cout << buffer << std::endl;
    string target = string(buffer).substr(0, string(buffer).find(' '));
    string message = string(buffer).substr(string(buffer).find(' '));
    if (string(buffer) == target) {
        server.sendData(client.getClientFd(), getNumericReply(client ,412, ""));
        return ;
    }
    if (message.find("#*") != string::npos || message.find("$*") != string::npos) {
        server.sendData(client.getClientFd(), getNumericReply(client, 407, "PRIVMSG")); 
    }
    for (vector<Client>::iterator it = server.getClientsList().begin(); it != server.getClientsList().end(); it++) {
        if (target == it->getNickname() && it->getRegistrationStatus() == true) {
        string msg; 
        msg += "PRIVMSG ";
        msg += buffer;
        server.sendData(client.getClientFd(), client.getHostname() + msg);
        }
    }
    server.sendData(client.getClientFd(), getNumericReply(client, 401, target));
}