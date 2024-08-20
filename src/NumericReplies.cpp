#include "Server.hpp"
#include "Exceptions.hpp"

// 001: RPL_WELCOME - Bienvenue sur le serveur IRC
string RPL_WELCOME(const string &nick) {
    return "001 " + nick + " :Welcome to the Internet Relay Network " + nick;
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
    return "318 " + nick + " :End of /WHOIS list";
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
string RPL_CHANNELMODEIS(const string &channel, const string &modes) {
    return "324 " + channel + " " + modes;
}

// 331: RPL_NOTOPIC - Aucun sujet défini pour le canal
string RPL_NOTOPIC(const string &channel) {
    return "331 " + channel + " :No topic is set";
}

// 332: RPL_TOPIC - Sujet actuel du canal
string RPL_TOPIC(const string &channel, const string &topic) {
    return "332 " + channel + " :" + topic;
}

// 341: RPL_INVITING - Invitation envoyée à un utilisateur
string RPL_INVITING(const string &nick, const string &channel) {
    return "341 " + nick + " " + channel;
}

// 353: RPL_NAMREPLY - Réponse à la commande NAMES (liste des utilisateurs)
string RPL_NAMREPLY(const string &symbol, const string &channel, const string &users) {
    return "353 " + symbol + " " + channel + " :" + users;
}

// 366: RPL_ENDOFNAMES - Fin de la commande NAMES
string RPL_ENDOFNAMES(const string &channel) {
    return "366 " + channel + " :End of /NAMES list";
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
string ERR_NICKNAMEINUSE(const string &nick) {
    return "433 * " + nick + " :Nickname is already in use";
}

//451: ERR_NOTREGISTERED - Vous devez être enregistré pour effectuer cette action
string ERR_NOTREGISTERED() {
    return "451 :You have not registered";
}

// 482: ERR_CHANOPRIVSNEEDED - Droits d'opérateur nécessaires pour effectuer une action sur un canal
string ERR_CHANOPRIVSNEEDED(const string &channel) {
    return "482 " + channel + " :You're not channel operator";
}

//  ERR_NOSUCHNICK: Aucun utilisateur avec ce pseudonyme
string ERR_NOSUCHNICK(const string &nick) {
    return "401 " + nick + " :No such nick/channel";
}

//  ERR_NOSUCHCHANNEL: Aucun canal avec ce nom
string ERR_NOSUCHCHANNEL(const string &channel) {
    return "403 " + channel + " :No such channel";
}

//  ERR_CANNOTSENDTOCHAN: Ne peut pas envoyer de message dans ce canal
string ERR_CANNOTSENDTOCHAN(const string &channel) {
    return "404 " + channel + " :Cannot send to channel";
}


//  ERR_UNKNOWNCOMMAND: Commande inconnue
string ERR_UNKNOWNCOMMAND(const string &command) {
    return "421 " + command + " :Unknown command";
}

string getNumericReply(int code, string arg1, string arg2, string arg3, string arg4) 
{
    switch (code) {
        case 1: return RPL_WELCOME(arg1);
        case 2: return RPL_YOURHOST(arg1, arg2);
        case 3: return RPL_CREATED(arg1);
        case 4: return RPL_MYINFO(arg1, arg2, arg3, arg4);
        case 5: return RPL_ISUPPORT(arg1);
        case 301: return RPL_AWAY(arg1, arg2);
        case 305: return RPL_UNAWAY();
        case 306: return RPL_NOWAWAY();
        case 311: return RPL_WHOISUSER(arg1, arg2, arg3, arg4);
        case 312: return RPL_WHOISSERVER(arg1, arg2, arg3);
        case 313: return RPL_WHOISOPERATOR(arg1);
        case 317: return RPL_WHOISIDLE(arg1, arg2, arg3);
        case 318: return RPL_ENDOFWHOIS(arg1);
        case 319: return RPL_WHOISCHANNELS(arg1, arg2);
        case 322: return RPL_LIST(arg1, arg2, arg3);
        case 323: return RPL_LISTEND();
        case 324: return RPL_CHANNELMODEIS(arg1, arg2);
        case 331: return RPL_NOTOPIC(arg1);
        case 332: return RPL_TOPIC(arg1, arg2);
        case 341: return RPL_INVITING(arg1, arg2);
        case 353: return RPL_NAMREPLY(arg1, arg2, arg3);
        case 366: return RPL_ENDOFNAMES(arg1);
        case 372: return RPL_MOTD(arg1);
        case 375: return RPL_MOTDSTART(arg1);
        case 376: return RPL_ENDOFMOTD();
        case 433: return ERR_NICKNAMEINUSE(arg1);
        case 451: return ERR_NOTREGISTERED();
        case 482: return ERR_CHANOPRIVSNEEDED(arg1);
        case 401: return ERR_NOSUCHNICK(arg1);
        case 403: return ERR_NOSUCHCHANNEL(arg1);
        case 404: return ERR_CANNOTSENDTOCHAN(arg1);
        case 421: return ERR_UNKNOWNCOMMAND(arg1);
        default: return "";   
    }
}
