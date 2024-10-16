/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReplies.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:54:08 by ocyn              #+#    #+#             */
/*   Updated: 2024/10/16 18:57:16 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void send_ERR696(Client	&client, Channel &chan, Server &server, vector<string> buffsplit, string err_message) {

	string s = "";
	if (buffsplit.size() < 2)
		s = buffsplit[0];
	else 
		s = buffsplit[1];
	string rpl = "696 " + client.getNickname() + " " + chan.getName() + " " + s;
	for (int i = 2; i < (int)buffsplit.size(); i++)
		rpl += " " + buffsplit[i];
	rpl += (" " + err_message);
	server.sendData(client.getClientFd(), client.getHostname() + rpl);
}

// 001: RPL_WELCOME - Welcome to the IRC server
string RPL_WELCOME(const string &nick, const string &hostname) {
	std::stringstream ss;
	ss << "001 " << nick << " :Welcome to the Internet Relay Network " << hostname;
	return ss.str();
}

// 002: RPL_YOURHOST - Host information
string RPL_YOURHOST(const string &server, const string &version) {
	return "002 :Your host is " + server + ", running version " + version;
}

// 003: RPL_CREATED - Server creation date
string RPL_CREATED(const string &date) {
	return "003 :This server was created " + date;
}

// 004: RPL_MYINFO - Server information
string RPL_MYINFO(const string &server, const string &version, const string &userModes, const string &chanModes) {
	return "004 " + server + " " + version + " " + userModes + " " + chanModes;
}

// 005: RPL_ISUPPORT - Server capabilities
string RPL_ISUPPORT(const string &options) {
	return "005 " + options + " :are supported by this server";
}

// 301: RPL_AWAY - User is away
string RPL_AWAY(const string &nick, const string &message) {
	return "301 " + nick + " :" + message;
}

// 305: RPL_UNAWAY - Away mode is disabled
string RPL_UNAWAY() {
	return "305 :You are no longer marked as being away";
}

// 306: RPL_NOWAWAY - Away mode is enabled
string RPL_NOWAWAY() {
	return "306 :You have been marked as being away";
}

// 311: RPL_WHOISUSER - WHOIS user information
string RPL_WHOISUSER(const string &nick, const string &user, const string &host, const string &realName) {
	return "311 " + nick + " " + user + " " + host + " * :" + realName;
}

// 312: RPL_WHOISSERVER - WHOIS user server
string RPL_WHOISSERVER(const string &nick, const string &server, const string &serverInfo) {
	return "312 " + nick + " " + server + " :" + serverInfo;
}

// 313: RPL_WHOISOPERATOR - WHOIS user is an IRC operator
string RPL_WHOISOPERATOR(const string &nick) {
	return "313 " + nick + " :is an IRC operator";
}

// 317: RPL_WHOISIDLE - WHOIS user idle information
string RPL_WHOISIDLE(const string &nick, const string &idle, const string &signon) {
	return "317 " + nick + " " + idle + " " + signon + " :seconds idle, signon time";
}

// 318: RPL_ENDOFWHOIS - End of WHOIS command
string RPL_ENDOFWHOIS(const string &nick) {
	std::stringstream ss;
	ss << "318 " << nick << " :End of /WHOIS list";
	return ss.str();
}

// 319: RPL_WHOISCHANNELS - Channels the WHOIS user is connected to
string RPL_WHOISCHANNELS(const string &nick, const string &channels) {
	return "319 " + nick + " :" + channels;
}

// 322: RPL_LIST - Channel list
string RPL_LIST(const string &channel, const string &visible, const string &topic) {
	return "322 " + channel + " " + visible + " :" + topic;
}

// 323: RPL_LISTEND - End of LIST command
string RPL_LISTEND() {
	return "323 :End of /LIST";
}

// 324: RPL_CHANNELMODEIS - Channel modes
string RPL_CHANNELMODEIS(const string &nick, const string &channel, const string &modstring) {
	std::stringstream ss;
	if (modstring.empty())
		ss << "324 " << nick << " " << channel ;
	else
		ss << "324 " << nick << " " << channel << modstring;
	return ss.str();
}

// 331: RPL_NOTOPIC - No topic is sets
string RPL_NOTOPIC(const string &channel, const string &nick) {
	std::stringstream ss;
	ss << "331 " << nick << " " << channel << " :No topic is set";
	return ss.str();
}

// 332: RPL_TOPIC - Channel topic
string RPL_TOPIC(const string &channel, const string &topic, const string &nick) {
	return "332 " + nick + " " + channel + " :" + topic;
}

// 341: RPL_INVITING - Invitation sent to a user
string RPL_INVITING(const string &nick, const string &channel) {
	return "341 " + nick + " " + channel;
}

// 353: RPL_NAMREPLY - Response to NAMES command
// Example : "353 #test :@user1 user2 user3"
// @ : Operator
// = : Normal
string RPL_NAMREPLY(const string &symbol, const string &channel, const string &users) {
	std::stringstream ss;
	ss << "353 " << symbol << " " << channel << " :" << users;
	return ss.str();
}

// 366: RPL_ENDOFNAMES - Fin de la commande NAMES
string RPL_ENDOFNAMES(const string &channel) {
	std::stringstream ss;
	ss << "366 " << channel << " :End of /NAMES list";
	return ss.str();
}

// 372: RPL_MOTD - Message of the Day (MOTD)
string RPL_MOTD(const string &text) {
	return "372 :- " + text;
}

// 375: RPL_MOTDSTART - Start of MOTD
string RPL_MOTDSTART(const string &server) {
	return "375 :- " + server + " Message of the day -";
}

// 376: RPL_ENDOFMOTD - End of MOTD
string RPL_ENDOFMOTD() {
	return "376 :End of /MOTD command";
}

// 433: ERR_NICKNAMEINUSE - Nickname already in use
string ERR_NICKNAMEINUSE(const string &nick, const string &command) {
	std::stringstream ss;
	ss << "433 " << nick << " " << command << " :Nickname is already in use";
	return ss.str();
}

//451: ERR_NOTREGISTERED - You must be registered to perform this action
string ERR_NOTREGISTERED(const string &nick) {
	std::stringstream ss;
	if (nick.empty())
		ss << "451 NotRegistered :You have not registered";
	else
		ss << "451 " << nick << " :You have not registered";
	return ss.str();
}

// 482: ERR_CHANOPRIVSNEEDED - Operator rights needed to perform an action on a channel
string ERR_CHANOPRIVSNEEDED(const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "482 " << nick << " " << channel << " :You're not channel operator";
	return ss.str();
}

//  430: ERR_NOSUCHCHANNEL: No channel with this name
string ERR_NOSUCHCHANNEL(const string &nick, const string &channel) {
	std::stringstream ss;	
	ss << "403 " << nick << " " << channel << " :No such channel";
	return ss.str();
}

// 404: ERR_CANNOTSENDTOCHAN: Cannot send message to this channel
string ERR_CANNOTSENDTOCHAN(const string &channel) {
	std::stringstream ss;
	ss << "404 " << channel << " :Cannot send message to " << channel;
	return ss.str();
}


// 421: ERR_UNKNOWNCOMMAND: Unknown command
string ERR_UNKNOWNCOMMAND(const string &command, const string &nick) {
	std::stringstream ss;
	ss << "421 " << nick << " " << command << " :Unknown command";
	return ss.str();
}

// 462: ERR_ALREADYREGISTERED - You are already registered
string ERR_ALREADYREGISTERED(const string &nick) {
	std::stringstream ss;
	ss << "462 " << nick << " :You may not reregister";
	return ss.str();
}

// 461: ERR_NEEDMOREPARAMS - Not enough parameters
string ERR_NEEDMOREPARAMS(const string &nick, const string &command) {
	
	if (nick.empty())
		return "461 NotRegistered " + command + " :Not enough parameters";
	return "461 " + nick  + " " + command + " :Not enough parameters";
}

// 464: ERR_PASSWDMISMATCH - Incorrect password
string ERR_PASSWDMISMATCH(int i) {
	std::stringstream ss;
	ss << "464 " << i << " :Password incorrect";
	return ss.str();
}

// 432: ERR_ERRONEUSNICKNAME - Erroneous nickname
string ERR_ERRONEUSNICKNAME(const string &nick, const string &command) {
	std::stringstream ss;
	ss << "432 " << nick << " " << command << " :Erroneus nickname";
	return ss.str();
}

// 431: ERR_NONICKNAMEGIVEN - No nickname given
string ERR_NONICKNAMEGIVEN(const string &nick) {
	std::stringstream ss;
	ss << "431 " << nick << " :No nickname given";
	return ss.str();
}

// 412: ERR_NOTEXTTOSEND - No text to send
string ERR_NOTEXTTOSEND(const string &nick) {
	std::stringstream ss;
	ss << "412 " << nick << " :No text to send";
	return ss.str();
}

// 401: ERR_NOSUCHNICK - Invalid nickname
string ERR_NOSUCHNICK(const string &command, const string &nick) {
	std::stringstream ss;
	ss << "401 " << command << " " << nick << " :No such nick";
	return ss.str();
}

// 407: TOOMANYTARGETS - Too many recipients
string ERR_TOOMANYTARGETS(const string &nick, const string &command) {
	std::stringstream ss;
	ss << "407 " << nick << " " << command << " :Too many recipients";
	return ss.str();
}

//352: RPL_WHOREPLY - Response to WHO command
string RPL_WHOREPLY(const string &nick, const string &channel, const string &user, const string &host, const string &server, const string &status) {
	std::stringstream ss;
	ss << "352 " << nick << " " << channel << " " << user << " " << host << " " << server << " " << status;
	return ss.str();
}

// 315: RPL_ENDOFWHO - End of WHO command
string RPL_ENDOFWHO(const string &mask) {
	return "315 " + mask + " :End of /WHO list";
}

// 333: RPL_TOPICWHOTIME - Channel topic and modification date
string RPL_TOPICWHOTIME(const string &client, const string &nick, const string &channel, const string &time) {
	std::stringstream ss;
	ss << "333 " << client << " " << nick << " " << channel << " " + time;
	return ss.str();
}

// 441 ERR_USERNOTINCHANNEL - User is not on the channel

string ERR_USERNOTINCHANNEL(const string &client, const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "441 " << client << " " << nick << " " << channel << " :They aren't on that channel";
	return ss.str();
}

//442 ERR_NOTONCHANNEL - You are not on this channel
string ERR_NOTONCHANNEL(const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "442 " << nick << " " << channel << " :You're not on that channel";
	return ss.str();
}

// 417 ERR_INPUTTOOLONG - Command too long
string ERR_INPUTTOOLONG(const string &nick) {
	std::stringstream ss;
	ss << "417 " << nick << " :Input line too long";
	return ss.str();
}

// 329 RPL_CREATIONTIME - Channel creation date
string RPL_CREATIONTIME(const string &nick, const string &channel, const string &time) {
	std::stringstream ss;
	ss << "329 " << nick << " " << channel << " " << time;
	return ss.str();
}

// 696 ERR_INVALIDMOREPARAM - More invalid parameters
// "<client> <target chan/user> <mode char> <parameter> :<description>"
string ERR_INVALIDMOREPARAM(const string &nick, const string &target, const string &mode, const string &parameter, const string &description) {
	std::stringstream ss;
	ss << "696 " << nick << " " << target << " " << mode << " " << parameter << " :" << description;
	return ss.str();
}

// 471 ERR_CHANNELISFULL - Channel is full
string ERR_CHANNELISFULL(const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "471 " << nick << " " << channel << " :Cannot join channel (+l)";
	return ss.str();
}

// 475 ERR_BADCHANNELKEY - Invalid channel key
string ERR_BADCHANNELKEY(const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "475 " << nick << " " << channel << " :Cannot join channel (+k)";
	return ss.str();
}

// 476 ERR_BADCHANMASK - Invalid channel mask 
string ERR_BADCHANMASK(const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "476 " << nick << " " << channel << " :Bad channel mask";
	return ss.str();
}

// 473 ERR_INVITEONLYCHAN - Channel is invite-only 
string ERR_INVITEONLYCHAN(const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "473 " << nick << " " << channel << " :Cannot join channel (+i)";
	return ss.str();
}

// 443 ERR_USERONCHANNEL - User is already on the channel
string ERR_USERONCHANNEL(const string &client,const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "443 " << client << " " << nick << " " << channel << " :is already on channel";
	return ss.str();
}

// 341 RPL_INVITING - Invitation sent to a user
string RPL_INVITING(const string &client, const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "341 " << client << " " << nick << " " << channel << " :Inviting";
	return ss.str();
}

//525 ERR_INVALIDKEY - Key is not well-formed
string ERR_INVALIDKEY(const string &client, const string &channel) {
	std::stringstream ss;
	ss << "525 " << client << " " << channel << " " << ":Key is not well-formed";
	return ss.str();
}

/// @brief Get a numeric reply
/// @param client The client object
/// @param code The numeric reply code
/// @param arg The argument of the command (multiple arguments can be passed if separated by '_')
/// @return string The numeric reply
string getNumericReply(Client& client, int code, string arg)
{
	string s = client.getHostname();
	vector<string> arg_split;
	if (arg != "" && arg.find('_') != string::npos)
		arg_split = split(arg, '_');
	switch (code) {
		case 1:   return s + RPL_WELCOME(client.getNickname(), client.getNickname());
		case 315: return s + RPL_ENDOFWHOIS(client.getNickname());
		case 324: return s + RPL_CHANNELMODEIS(client.getNickname(), arg_split[0], arg_split[1]);
		case 329: return s + RPL_CREATIONTIME(client.getNickname(), arg_split[0], arg_split[1]);
		case 331: return s + RPL_NOTOPIC(arg, client.getNickname());
		case 332: return s + RPL_TOPIC(arg_split[0], arg_split[1], client.getNickname());
		case 333: return s + RPL_TOPICWHOTIME(client.getNickname(), arg_split[0], arg_split[1], arg_split[2]);
		case 341: return s + RPL_INVITING(client.getNickname(), arg_split[0], arg_split[1]);
		case 352: return s + RPL_WHOREPLY(client.getNickname(), arg_split[0], arg_split[1], arg_split[2], arg_split[3], arg_split[4]);
		case 353: return s + RPL_NAMREPLY(arg_split[1] + " =", arg_split[0], arg_split[1]);
		case 366: return s + RPL_ENDOFNAMES(client.getNickname() + " " + arg);
		case 401: return s + ERR_NOSUCHNICK(client.getNickname(), arg);
		case 403: return s + ERR_NOSUCHCHANNEL(client.getNickname(), arg);
		case 404: return s + ERR_CANNOTSENDTOCHAN(arg);
		case 407: return s + ERR_TOOMANYTARGETS(client.getNickname(), arg);
		case 412: return s + ERR_NOTEXTTOSEND(client.getNickname());
		case 417: return s + ERR_INPUTTOOLONG(client.getNickname());
		case 421: return s + ERR_UNKNOWNCOMMAND(arg, client.getNickname());
		case 433: return s + ERR_NICKNAMEINUSE(arg, arg);
		case 431: return s + ERR_NONICKNAMEGIVEN(client.getNickname());
		case 432: return s + ERR_ERRONEUSNICKNAME(client.getNickname(), arg);
		case 441: return s + ERR_USERNOTINCHANNEL(client.getNickname(), arg_split[0], arg_split[1]);
		case 442: return s + ERR_NOTONCHANNEL(client.getNickname(), arg);
		case 443: return s + ERR_USERONCHANNEL(client.getNickname(), arg_split[0], arg_split[1]);
		case 451: return s + ERR_NOTREGISTERED(client.getNickname());
		case 462: return s + ERR_ALREADYREGISTERED(client.getNickname());
		case 461: return s + ERR_NEEDMOREPARAMS(client.getNickname(), arg);
		case 464: return s + ERR_PASSWDMISMATCH(client.getClientFd());
		case 471: return s + ERR_CHANNELISFULL(client.getNickname(), arg);
		case 473: return s + ERR_INVITEONLYCHAN(client.getNickname(), arg);
		case 475: return s + ERR_BADCHANNELKEY(client.getNickname(), arg);
		case 476: return s + ERR_BADCHANMASK(client.getNickname(), arg);
		case 482: return s + ERR_CHANOPRIVSNEEDED(client.getNickname(), arg);
		case 525: return s + ERR_INVALIDKEY(client.getNickname(), arg);
		case 696: return s + ERR_INVALIDMOREPARAM(client.getNickname(), arg_split[0], arg_split[1], arg_split[2], arg_split[3]);
		default: return "";
	}
}
