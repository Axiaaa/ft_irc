/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NumericReplies.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcamerly <lcamerly@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:54:08 by ocyn              #+#    #+#             */
/*   Updated: 2024/09/24 16:21:09 by lcamerly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// 001: RPL_WELCOME - Bienvenue sur le serveur IRC
string RPL_WELCOME(const string &nick, const string &hostname) {
	std::stringstream ss;
	ss << "001 " << nick << " :Welcome to the Internet Relay Network " << hostname;
	return ss.str();
}

// 002: RPL_YOURHOST - Informations sur l'hôte
string RPL_YOURHOST(const string &server, const string &version) {
	return "002 :Your host is " + server + ", running version " + version;
}

// 003: RPL_CREATED - Date de création du serveur
string RPL_CREATED(const string &date) {
	return "003 :This server was created " + date;
}

// 004: RPL_MYINFO - Informations sur le serveur
string RPL_MYINFO(const string &server, const string &version, const string &userModes, const string &chanModes) {
	return "004 " + server + " " + version + " " + userModes + " " + chanModes;
}

// 005: RPL_ISUPPORT - Capacités du serveur
string RPL_ISUPPORT(const string &options) {
	return "005 " + options + " :are supported by this server";
}

// 301: RPL_AWAY - L'utilisateur est absent
string RPL_AWAY(const string &nick, const string &message) {
	return "301 " + nick + " :" + message;
}

// 305: RPL_UNAWAY - Le mode absent est désactivé
string RPL_UNAWAY() {
	return "305 :You are no longer marked as being away";
}

// 306: RPL_NOWAWAY - Le mode absent est activé
string RPL_NOWAWAY() {
	return "306 :You have been marked as being away";
}

// 311: RPL_WHOISUSER - Informations WHOIS de l'utilisateur
string RPL_WHOISUSER(const string &nick, const string &user, const string &host, const string &realName) {
	return "311 " + nick + " " + user + " " + host + " * :" + realName;
}

// 312: RPL_WHOISSERVER - Serveur de l'utilisateur WHOIS
string RPL_WHOISSERVER(const string &nick, const string &server, const string &serverInfo) {
	return "312 " + nick + " " + server + " :" + serverInfo;
}

// 313: RPL_WHOISOPERATOR - L'utilisateur WHOIS est un opérateur IRC
string RPL_WHOISOPERATOR(const string &nick) {
	return "313 " + nick + " :is an IRC operator";
}

// 317: RPL_WHOISIDLE - Information de l'inactivité de l'utilisateur WHOIS
string RPL_WHOISIDLE(const string &nick, const string &idle, const string &signon) {
	return "317 " + nick + " " + idle + " " + signon + " :seconds idle, signon time";
}

// 318: RPL_ENDOFWHOIS - Fin de la commande WHOIS
string RPL_ENDOFWHOIS(const string &nick) {
	std::stringstream ss;
	ss << "318 " << nick << " :End of /WHOIS list";
	return ss.str();
}

// 319: RPL_WHOISCHANNELS - Canaux auxquels l'utilisateur WHOIS est connecté
string RPL_WHOISCHANNELS(const string &nick, const string &channels) {
	return "319 " + nick + " :" + channels;
}

// 322: RPL_LIST - Liste des canaux
string RPL_LIST(const string &channel, const string &visible, const string &topic) {
	return "322 " + channel + " " + visible + " :" + topic;
}

// 323: RPL_LISTEND - Fin de la commande LIST
string RPL_LISTEND() {
	return "323 :End of /LIST";
}

// 324: RPL_CHANNELMODEIS - Modes du canal
string RPL_CHANNELMODEIS(const string &nick, const string &arg) {
	std::stringstream ss;
	ss << "324 " << nick << " " << arg;
	return ss.str();
}

// 331: RPL_NOTOPIC - Aucun sujet défini pour le canal
string RPL_NOTOPIC(const string &channel, const string &nick) {
	std::stringstream ss;
	ss << "331 " << nick << " " << channel << " :No topic is set";
	return ss.str();
}

// 332: RPL_TOPIC - Sujet actuel du canal
string RPL_TOPIC(const string &channel, const string &topic, const string &nick) {
	return "332 " + nick + " " + channel + " :" + topic;
}

// 341: RPL_INVITING - Invitation envoyée à un utilisateur
string RPL_INVITING(const string &nick, const string &channel) {
	return "341 " + nick + " " + channel;
}

// 353: RPL_NAMREPLY - Réponse à la commande NAMES (liste des utilisateurs)
// Exemple : "353 #test :@user1 user2 user3"
// @ : Opérateur
// + : Voix
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

// 375: RPL_MOTDSTART - Début du MOTD
string RPL_MOTDSTART(const string &server) {
	return "375 :- " + server + " Message of the day -";
}

// 376: RPL_ENDOFMOTD - Fin du MOTD
string RPL_ENDOFMOTD() {
	return "376 :End of /MOTD command";
}

// 433: ERR_NICKNAMEINUSE - Pseudonyme déjà utilisé
string ERR_NICKNAMEINUSE(const string &nick, const string &command) {
	std::stringstream ss;
	ss << "433 " << nick << " " << command << " :Nickname is already in use";
	return ss.str();
}

//451: ERR_NOTREGISTERED - Vous devez être enregistré pour effectuer cette action
string ERR_NOTREGISTERED(const string &nick) {
	std::stringstream ss;
	ss << "451 " << nick << " :You have not registered";
	return ss.str();
}

// 482: ERR_CHANOPRIVSNEEDED - Droits d'opérateur nécessaires pour effectuer une action sur un canal
string ERR_CHANOPRIVSNEEDED(const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "482 " << nick << " " << channel << " :You're not channel operator";
	return ss.str();
}

//  430: ERR_NOSUCHCHANNEL: Aucun canal avec ce nom
string ERR_NOSUCHCHANNEL(const string &nick, const string &channel) {
	std::stringstream ss;	
	ss << "403 " << nick << " " << channel << " :No such channel";
	return ss.str();
}

// 404: ERR_CANNOTSENDTOCHAN: Ne peut pas envoyer de message dans ce canal
string ERR_CANNOTSENDTOCHAN(const string &channel) {
	std::stringstream ss;
	ss << "404 " << channel << " :Cannot send message to " << channel;
	return ss.str();
}


// 421: ERR_UNKNOWNCOMMAND: Commande inconnue
string ERR_UNKNOWNCOMMAND(const string &command, const string &nick) {
	std::stringstream ss;
	ss << "421 " << nick << " " << command << " :Unknown command";
	return ss.str();
}

// 462: ERR_ALREADYREGISTERED - Vous êtes déjà enregistré
string ERR_ALREADYREGISTERED(const string &nick) {
	std::stringstream ss;
	ss << "462 " << nick << " :You may not reregister";
	return ss.str();
}

// 461: ERR_NEEDMOREPARAMS - Pas assez de paramètres
string ERR_NEEDMOREPARAMS(const string &nick, const string &command) {
	
	if (nick.empty())
		return "461 NotRegistered " + command + " :Not enough parameters";
	return "461 " + nick  + " " + command + " :Not enough parameters";
}

// 464: ERR_PASSWDMISMATCH - Mot de passe incorrect
string ERR_PASSWDMISMATCH(int i) {
	std::stringstream ss;
	ss << "464 " << i << " :Password incorrect";
	return ss.str();
}

// 432: ERR_ERRONEUSNICKNAME - Pseudonyme incorrect
string ERR_ERRONEUSNICKNAME(const string &nick, const string &command) {
	std::stringstream ss;
	ss << "432 " << nick << " " << command << " :Erroneus nickname";
	return ss.str();
}

// 431: ERR_NONICKNAMEGIVEN - Aucun pseudonyme donné
string ERR_NONICKNAMEGIVEN(const string &nick) {
	std::stringstream ss;
	ss << "431 " << nick << " :No nickname given";
	return ss.str();
}

// 412: ERR_NOTEXTTOSEND - Aucun texte donné
string ERR_NOTEXTTOSEND(const string &nick) {
	std::stringstream ss;
	ss << "412 " << nick << " :No text to send";
	return ss.str();
}

// 401: ERR_NOSUCHNICK - Pseudonyme invalide
string ERR_NOSUCHNICK(const string &command, const string &nick) {
	std::stringstream ss;
	ss << "401 " << command << " " << nick << " :No such nick";
	return ss.str();
}

// 407: TOOMANYTARGETS - Trop de destinataires
string ERR_TOOMANYTARGETS(const string &nick, const string &command) {
	std::stringstream ss;
	ss << "407 " << nick << " " << command << " :Too many recipients";
	return ss.str();
}

//352: RPL_WHOREPLY - Réponse à la commande
string RPL_WHOREPLY(const string &nick, const string &channel, const string &user, const string &host, const string &server, const string &status) {
	std::stringstream ss;
	ss << "352 " << nick << " " << channel << " " << user << " " << host << " " << server << " " << status;
	return ss.str();
}

// 315: RPL_ENDOFWHO - Fin de la commande WHO
string RPL_ENDOFWHO(const string &mask) {
	return "315 " + mask + " :End of /WHO list";
}

// 333: RPL_TOPICWHOTIME - Sujet du canal et date de modification
string RPL_TOPICWHOTIME(const string &client, const string &nick, const string &channel, const string &time) {
	std::stringstream ss;
	ss << "333 " << client << " " << nick << " " << channel << " " + time;
	return ss.str();
}

//442 ERR_NOTONCHANNEL - Vous n'êtes pas sur ce canal
string ERR_NOTONCHANNEL(const string &nick, const string &channel) {
	std::stringstream ss;
	ss << "442 " << nick << " " << channel << " :You're not on that channel";
	return ss.str();
}

// 417 ERR_INPUTTOOLONG - Commande trop longue
string ERR_INPUTTOOLONG(const string &nick) {
	std::stringstream ss;
	ss << "417 " << nick << " :Input line too long";
	return ss.str();
}

/* 
 *  @brief Get the numeric reply corresponding to the code
 *  @example Ex : "<username>!<hostname>@<servername> 001 :Welcome to the Internet Relay Network"
 *  @param client The client object
 *  @param code The numeric reply code
 *  @param arg The argument of the numeric reply (if multiple arguments, separate them with a '_')
 *  @return string : The numeric reply.
 */
string getNumericReply(Client& client, int code, string arg)
{
	string s = client.getHostname();
	vector<string> arg_split;
	if (arg != "" && arg.find('_') != string::npos)
		arg_split = split(arg, '_');
	switch (code) {
		case 1: return s + RPL_WELCOME(client.getNickname(), client.getNickname());
		case 315: return s + RPL_ENDOFWHOIS(client.getNickname());
		case 324: return s + RPL_CHANNELMODEIS(client.getNickname(), arg);
		case 331: return s + RPL_NOTOPIC(arg, client.getNickname());
		case 332: return s + RPL_TOPIC(arg_split[0], arg_split[1], client.getNickname());
		case 333: return s + RPL_TOPICWHOTIME(client.getNickname(), arg_split[0], arg_split[1], arg_split[2]);
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
		case 442: return s + ERR_NOTONCHANNEL(client.getNickname(), arg);
		case 451: return s + ERR_NOTREGISTERED(client.getNickname());
		case 462: return s + ERR_ALREADYREGISTERED(client.getNickname());
		case 461: return s + ERR_NEEDMOREPARAMS(client.getNickname(), arg);
		case 464: return s + ERR_PASSWDMISMATCH(client.getClientFd());
		case 482: return s + ERR_CHANOPRIVSNEEDED(client.getNickname(), arg);
		default: return "";
	}
}
