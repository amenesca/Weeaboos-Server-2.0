/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:18 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/14 16:49:09 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"

VirtualServer::VirtualServer(void) :
	_serverDefault(false),
	_port(),
	_root(""),
	_serverName(),
	_host(INADDR_NONE),
	_index(),
	_maxBodySize(0),
	_errorPage(),
	_locations()
{
	
}

VirtualServer::~VirtualServer(void)
{
	_serverDefault = 0;
	_port.clear();
	_root.clear();
	_serverName.clear();
	_host = INADDR_NONE;
	_index.clear();
	_maxBodySize = 0;
	_errorPage.clear();
	_locations.clear();
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
		this->_index = src.getIndex();
		this->_maxBodySize = src.getMaxBodySize();
		this->_errorPage = src.getErrorPage();
		this->_locations = src.getLocations();
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