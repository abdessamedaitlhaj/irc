#pragma once 

//add comment later
#include <vector>
#include "client.hpp"
#include "channel.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>


// Define colors
#define RESET   "\e[0m"
#define RED     "\e[1;31m"
#define GREEN   "\e[1;32m"
#define YELLOW  "\e[1;33m"
#define BLUE    "\e[1;34m"
#define MAGENTA "\e[1;35m"
#define CYAN    "\e[1;36m"
#define WHITE   "\e[1;37m"



class server {
	private :
		int _port;
		int	_socket;
		static int _break ;
		struct sockaddr_in serverAddress;
		struct pollfd	_poll;
		std::string _password;
		std::vector<client> _Clients;
		std::vector<struct pollfd> _pollfds;
		std::vector<channel> _Channels;
	public :
		void setup();
		void serverSocket();
		server(char** av);
		int parse_port(std::string port);
		std::string parse_password(std::string password);
		static void BreakSignal(int signum);
};