#include "../../include/Server.hpp"

void Server::handleJoin(int fd, std::string &input, Client& client)
{
    std::vector<std::string> tokens;

    tokens = Server::split(input, std::string("\t "));


    //need more params
    if (tokens.size() < 2)
    {
        sendResponse(fd, ERR_NEEDMOREPARAMS(client.getNickName(), tokens[0]));
        return ;
    }
    // if (tokens[1] == "0")
    // {
    //     std::vector<std::string> userchannels = client.getChannels();
    // }
    std::vector<std::pair<std::string, std::string > > pairs_channels;
    std::string channels= tokens[1];
    std::string passwords = "";
    if (tokens.size() > 2)
        passwords = tokens[2];
    std::string buffer;
    //split channels
    for (size_t i = 0; i < channels.length(); i++)
    {
        if (channels[i] == ',')
        {
            pairs_channels.push_back(std::make_pair(buffer, ""));
            buffer.clear();
        }
        else
            buffer += channels[i];
    }
    pairs_channels.push_back(std::make_pair(buffer, ""));
    //split passwords
    if (!passwords.empty()) {
        buffer.clear();
        size_t j = 0;
        for (size_t i = 0; i < passwords.length(); i++) {
            if (passwords[i] == ',') {
                if (j < pairs_channels.size())
                    pairs_channels[j].second = buffer;
                j++;
                buffer.clear();
            } else {
                buffer += passwords[i];
            }
        }
        if (j < pairs_channels.size())
            pairs_channels[j].second = buffer;
    }
    //channel processing
    for (size_t i = 0; i < pairs_channels.size(); i++)
    {
        std::string channel_name = pairs_channels[i].first;
        std::string password = pairs_channels[i].second;
        // if (channel_name.empty() || (channel_name[0] != '#' && channel_name[0] != '&'))
        if (channel_name[0] != '#' && channel_name[0] != '&')
        {
            sendResponse(fd, ERR_NOSUCHCHANNEL(client.getNickName(), channel_name));
            continue ;
        }
        std::string _channel_name = channel_name.substr(1);
        Channel *channel = getChannel(_channel_name);
        std::cout << "channel_name: " << _channel_name << std::endl;
        if (!channel)
        {
            Channel *new_channel = new Channel(_channel_name);
            new_channel->setname(_channel_name);
            new_channel->isOperator(&client);
            _channels.push_back(new_channel);
            new_channel->addMember(&client);
            channel = new_channel;
            std::cout << "channel created" <<  new_channel->getName() << std::endl;
        }
        else
        {
            if (channel->isMember(&client))
            {
                std::cout << "isMember" << std::endl;
                continue ;
            }
            if (channel->getInviteOnly())
            {
                if(!channel->isInvited(&client, channel_name, 1))
                {
                    sendResponse(fd, ERR_INVITEONLYCHAN(client.getNickName(), channel_name));
                    continue ;
                }
            }
            std::cout << "getPassword: " << channel->getPassword() << std::endl;
            std::cout << "password: " << password << std::endl;
            if (!channel->getPassword().empty() && channel->getPassword() != password)
            {
                if (!channel->isInvited(&client, channel_name, 0))
                {
                    sendResponse(fd, ERR_PASSMISMATCH(client.getNickName()));
                    continue ;
                }
            }
            channel->addMember(&client);
        }
    }
}
