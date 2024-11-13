#include "../Command.hpp"



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


Self messages are client sides.
*/

void privmsg(Server& server, Client& client, const string &buffer) {
	
	if (client.getRegistrationStatus() != true) {
		server.sendData(client.getClientFd(), getNumericReply(client, 451, ""));
		return ;
	}
	string target = string(buffer).substr(0, string(buffer).find(' '));
	if (string(buffer) == target) {
		server.sendData(client.getClientFd(), getNumericReply(client, 412, ""));
		return ;
	}
	vector<pair<string, string> > buffsplit = bufferParser(buffer);
	for (vector<pair<string, string> >::iterator iterator = buffsplit.begin(); iterator != buffsplit.end(); iterator++) {
	{
		string target = (*iterator).first;
		string message = (*iterator).second;
		if (message.find("#*") != string::npos || message.find("$*") != string::npos) {
			server.sendData(client.getClientFd(), getNumericReply(client, 407, "PRIVMSG")); 
			break ;
		}
		for (vector<Client *>::iterator it = server.getClientsList().begin(); it != server.getClientsList().end(); it++) {
			if (target == (*it)->getNickname() && ((*it)->getRegistrationStatus() == true)) {
				if ((*it)->getNickname() == client.getNickname()) {
					server.sendData(client.getClientFd(), getNumericReply(client, 404, "yourself"));
					break; 
				}
				string msg = "PRIVMSG ";
				msg.append(target);
				msg.append(" ");
				msg.append(message);
				server.sendData((*it)->getClientFd(), client.getHostname() + msg);
				break;
			}
		}
		for (vector<Channel *>::iterator it = server.getChannelsList().begin(); it != server.getChannelsList().end(); it++) {
			if (target == (*it)->getName()) {
				string msg = client.getHostname();
				msg += "PRIVMSG ";
				msg.append(target);
				msg.append(" ");
				msg.append(message);
				(*it)->broadcastMessage(msg, &client, &server);
				break;
			}
		}
		// if (target[0] == '#')
		// 	server.sendData(client.getClientFd(), getNumericReply(client, 403, target));
		// else
		// 	server.sendData(client.getClientFd(), getNumericReply(client, 401, target));
		// }
		}
	}
}