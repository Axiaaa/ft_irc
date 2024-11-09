#include "../Command.hpp"


void	quit(Server& server, Client& client, const string &buffer)
{
    vector<Channel*> joinedChannels = client.getJoinedChannels();
    for (vector<Channel*>::iterator it = joinedChannels.begin(); it != joinedChannels.end(); it++)
    {
        part(server, client, (*it)->getName() + " " + buffer);
    }
    client.setQuit(true);
}