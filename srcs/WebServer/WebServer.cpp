/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:32:08 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/21 17:28:37 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./WebServer.hpp"

WebServer::WebServer() :
	_vServers(),
	_pollFds(),
	_nbrServers(0)
{
	
}

WebServer::~WebServer()
{
	_vServers.clear();
	_pollFds.clear();
	_nbrServers = 0;
}

WebServer::WebServer(const std::vector<VirtualServer>& vServers, const int& nbrServers) :
	_vServers(vServers),
	_pollFds(),
	_nbrServers(nbrServers)
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
		this->_nbrServers = src.getNbrServers();
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

int		WebServer::getNbrServers(void) const
{
	return this->_nbrServers;
}

void	WebServer::setVServers(const std::vector<VirtualServer>& vServers)
{
	this->_vServers = vServers;
}
		
void	WebServer::setPollFds(const std::vector<pollfd>& pollFds)
{
	this->_pollFds = pollFds;	
}

void	WebServer::setNbrServers(const int& nbrServers)
{
	this->_nbrServers = nbrServers;
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
	bool serverRunning = true;

	while (serverRunning)
	{
		if (!verifyPollStatus())
			return ;

		 // Verificar se tem entrada no server
		for (int i = 0; i < _nbrServers; i++)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				this->openNewConnection(i);
			}
			else if(isPollError(i))
			{
				std::cerr << "Error for poll revents" << std::endl;
//				serverRunning = false;
			}
		}

		// Verificar se tem entrada nos clientes, se existir
		for (size_t j = this->_nbrServers; j < this->_pollFds.size(); j++)
		{
			if (_pollFds[j].revents & POLLIN)
			{
				treatRequest(j - this->_nbrServers, j);
			}
			else if (_pollFds[j].revents & POLLOUT)
			{
				// enviar resposta
			}
			else if(isPollError(j))
			{
				std::cerr << "Error for poll revents" << std::endl;
				serverRunning = false;
			}
		}
	}
}

bool	WebServer::verifyPollStatus(void)
{
	ssize_t status;

	status = poll(_pollFds.data(), _pollFds.size(), -1);

	if (status == -1)
	{
		std::cerr << "Error in poll." << std::endl;
		this->closeAllConnections();
		return (false);
	}
	return (true);
}

bool	WebServer::isPollError(int i)
{
	return (_pollFds[i].revents & POLLERR \
		|| _pollFds[i].revents & POLLHUP \
		|| _pollFds[i].revents & POLLNVAL);
}

void	WebServer::openNewConnection(int i)
{
	int		newClientSocket;
	Client	newClient = Client();

	newClientSocket = accept(this->_vServers[i].getFdSocket(),\
		reinterpret_cast<struct sockaddr*>(newClient.getClientAddrPointer()),\
		newClient.getClientAddrLenPointer());
	
	newClient.setClientSocket(newClientSocket);
	newClient.setServerConfigs(this->_vServers[i]);

	this->addNewClientSockToPoll(newClient.getClientSocket());

	this->_Clients.push_back(newClient);
}

void	treatRequest(int clientPos, int pollPos)
{
	
}