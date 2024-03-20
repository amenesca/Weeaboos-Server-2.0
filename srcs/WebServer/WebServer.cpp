/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:32:08 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/20 11:37:07 by amenesca         ###   ########.fr       */
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