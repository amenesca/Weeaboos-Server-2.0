/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:32:08 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/20 12:00:22 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./WebServer.hpp"

WebServer::WebServer() :
	_vServers(),
	_pollFds()
{
	
}

WebServer::~WebServer()
{
	_vServers.clear();
	_pollFds.clear();
}

WebServer::WebServer(const std::vector<VirtualServer>& vServers) :
	_vServers(vServers),
	_pollFds()
{}

WebServer::WebServer(const WebServer& copy)
{
	*this = copy;
	return ;
}

WebServer& WebServer::operator=(const WebServer& src)
{
	if (this != &src)
	{
		this->_vServers = src.getVServers();
		this->_pollFds = src.getPollFds();
	}
	return *this;
}

std::vector<VirtualServer>	WebServer::getVServers(void) const
{
	return this->_vServers;
}

std::vector<pollfd> WebServer::getPollFds(void) const
{
	return this->_pollFds;
}

void	WebServer::setVServers(const std::vector<VirtualServer>& vServers)
{
	this->_vServers = vServers;
}
		
void	WebServer::setPollFds(const std::vector<pollfd>& pollFds)
{
	this->_pollFds = pollFds;	
}

void	WebServer::addVServersSockToPoll(void)
{
	std::cout << "Adding vservers sockets to poll." << std::endl;
	for (size_t i = 0; i < this->_vServers.size(); i++)
	{
		addNewSocketToPoll(this->_vServers[i].getFdSocket());
	}
	return ;
}

void	WebServer::addNewSocketToPoll(int socketFd)
{
	pollfd newPollFd;
	std::cout << "Adding new socket number: " << socketFd << " to poll" << std::endl;
	fcntl(socketFd, F_SETFL, O_NONBLOCK);
	newPollFd.fd = socketFd;
	newPollFd.events = POLLIN | POLLOUT;
	newPollFd.revents = 0;
	this->_pollFds.push_back(newPollFd);

	return ;
}

void	WebServer::addNewClientSockToPoll(int clientSocket)
{
	std::cout << "Starting Connection with a new client." << std::endl;
	this->addNewSocketToPoll(clientSocket);
}

void	WebServer::closeConnection(int index)
{
	std::cout << "Closing connection: " << this->_pollFds[index].fd \
	<< std::endl;
	close(this->_pollFds[index].fd);
	this->_pollFds.erase(this->_pollFds.begin() + index);
}

void	WebServer::closeAllConnections(void)
{
	size_t	i;

	i = 0;
	while (i < this->_pollFds.size())
	{
		std::cout << "Closing the connection: " << this->_pollFds[i].fd \
		<< std::endl;
		close(this->_pollFds[i++].fd);
	}
	this->_pollFds.clear();
}

void	WebServer::StartServer(void)
{
	this->addVServersSockToPoll();

	
}