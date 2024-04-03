/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:18 by amenesca          #+#    #+#             */
/*   Updated: 2024/04/03 13:55:12 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"

VirtualServer::VirtualServer(void) :
	_serverDefault(false),
	_port(),
	_root(""),
	_serverName(),
	_host(INADDR_NONE),
	_strHost(""),
	_index(),
	_maxBodySize(0),
	_errorPage(),
	_locations(),
	_fd_socket(-1),
	_main_port(0)
{
	
}

VirtualServer::~VirtualServer(void)
{
	_serverDefault = 0;
	_port.clear();
	_root.clear();
	_serverName.clear();
	_host = INADDR_NONE;
	_strHost.clear();
	_index.clear();
	_maxBodySize = 0;
	_errorPage.clear();
	_locations.clear();
	_main_port = 0;
}

VirtualServer& VirtualServer::operator=(const VirtualServer& src)
{
	if (this != &src)
	{
		this->_serverDefault = src.getServerDefault();
		this->_port = src.getPort();
		this->_root = src.getRoot();
		this->_serverName = src.getServerName();
		this->_host	= src.getHost();
		this->_strHost = src.getStrHost();
		this->_index = src.getIndex();
		this->_maxBodySize = src.getMaxBodySize();
		this->_errorPage = src.getErrorPage();
		this->_locations = src.getLocations();
		this->_fd_socket = src.getFdSocket();
		this->_main_port = src.getMainPort();
	}
	return *this;
}

VirtualServer::VirtualServer(const VirtualServer& copy)
{
	*this = copy;
	return;
}

bool VirtualServer::getServerDefault(void) const
{
	return this->_serverDefault;
}

std::vector<int> VirtualServer::getPort(void) const
{
	return this->_port;
}

std::string	VirtualServer::getRoot(void) const
{
	return this->_root;
}

std::vector<std::string> VirtualServer::getServerName(void) const
{
	return this->_serverName;
}

in_addr_t VirtualServer::getHost(void) const
{
	return this->_host;
}

std::string VirtualServer::getStrHost(void) const
{
	return this->_strHost;
}

std::vector<std::string> VirtualServer::getIndex(void) const
{
	return this->_index;
}

int	VirtualServer::getMaxBodySize(void) const
{
	return this->_maxBodySize;
}

std::vector<std::string> VirtualServer::getErrorPage(void) const
{
	return this->_errorPage;
}

std::vector<Location> VirtualServer::getLocations(void) const
{
	return this->_locations;
}

int	VirtualServer::getFdSocket(void) const
{
	return this->_fd_socket;
}

int VirtualServer::getMainPort(void) const
{
	return this->_main_port;
}

std::vector<Location>*	VirtualServer::getLocationsAddress(void)
{
	return &this->_locations;
}

void	VirtualServer::setServerDefault(const bool& serverDefault)
{
	this->_serverDefault = serverDefault;
}

void	VirtualServer::setPort(const std::vector<int>& port)
{
	this->_port = port;
}

void	VirtualServer::setRoot(const std::string& root)
{
	this->_root = root;
}

void	VirtualServer::setServerName(const std::vector<std::string>& serverName)
{
	this->_serverName = serverName;
}

void	VirtualServer::setHost(const in_addr_t& host)
{
	this->_host = host;
}

void	VirtualServer::setStrHost(const std::string& strHost)
{
	this->_strHost = strHost;
}

void	VirtualServer::setIndex(const std::vector<std::string>& index)
{
	this->_index = index;
}

void	VirtualServer::setMaxBodySize(const int& maxBodySize)
{
	this->_maxBodySize = maxBodySize;
}

void	VirtualServer::setErrorPage(const std::vector<std::string>& errorPage)
{
	this->_errorPage = errorPage;
}

void	VirtualServer::setLocations(const std::vector<Location>& locations)
{
	this->_locations = locations;
}

void	VirtualServer::setFdSocket(const int& fd_socket)
{
	this->_fd_socket = fd_socket;
}

void	VirtualServer::setMainPort(const int& main_port)
{
	this->_main_port = main_port;
}

void	VirtualServer::initialize(void)
{
	struct sockaddr_in	server_addr;
	socklen_t			server_addr_len;
	int					opt;
	
	if (_serverDefault == true)
	{
		opt = 1;
		this->_main_port = _port[0];
		this->_fd_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
		
		if (this->_fd_socket == -1)
		{
			throw SocketError();
		}
		setsockopt(this->_fd_socket, SOL_SOCKET, SO_REUSEADDR, \
			&opt, sizeof(int));

		std::memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = this->getHost();
		server_addr.sin_port = htons(this->_main_port);

		server_addr_len = sizeof(server_addr);

		if (bind(this->_fd_socket, \
			reinterpret_cast<struct sockaddr*>(&server_addr), \
			server_addr_len) == -1)
		{
			throw BindError();
		}

		if (listen(this->_fd_socket, 1024) == -1)
		{
			throw ListenError();
		}
	}
	return ;
}

int						VirtualServer::acceptCon(void) const
{
	struct sockaddr_in	client_address;
	socklen_t			client_address_len;
	int					client_socket;

	client_address_len = sizeof(client_address);
	client_socket = accept(this->_fd_socket, \
		reinterpret_cast<struct sockaddr*>(&client_address), \
		&client_address_len);
	return (client_socket);
}

void					VirtualServer::closeCon(void)
{
	if (this->_fd_socket > 0)
	{
		close(this->_fd_socket);
		this->_fd_socket = -1;
	}
}

bool VirtualServer::isDescriptorOpen(int fd) const {
    // Tenta associar um fluxo de arquivo ao descritor de arquivo fornecido
    FILE *file = fdopen(fd, "r");
    if (file) {
        // Se a associação for bem-sucedida, o descritor de arquivo está aberto
        fclose(file);
        return true;
    } else {
        // Se a associação falhar, o descritor de arquivo não está aberto
        return false;
    }
}

const char *VirtualServer::SocketError::what() const throw() {
	return ("webserver: error: can't create a new socket.");
}

const char *VirtualServer::BindError::what() const throw() {
	return ("webserver: error: can't bind address to socket.");
}

const char *VirtualServer::ListenError::what() const throw() {
	return ("webserver: error: can't listen to connections on socket.");
}