/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:33:07 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/22 13:57:08 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Client.hpp"

Client::Client() :
	_clientSocket(-1),
	_client_addr(),
	_client_addr_len(sizeof(this->_client_addr)),
	_requestBuffer(""),
	_bytesRead(0),
	_requestRead(false),
	_serverConfigs(),
	_request()
{
	
}

Client::~Client()
{
	this->_clientSocket = -1;
	this->_client_addr_len = 0;
	memset(&this->_client_addr,0,sizeof(this->_client_addr));
	_requestRead = false;
	this->_requestBuffer.clear();
	this->_bytesRead = 0;
}

Client::Client(const Client& copy)
{
	*this = copy;
	return ;
}

Client& Client::operator=(const Client& src)
{
	if (this != &src)
	{
		this->_clientSocket = src.getClientSocket();
		this->_client_addr_len = src.getClientAddrLen();
		this->_client_addr = src.getClientAddr();
		this->_requestBuffer = src.getRequestBuffer();
		this->_bytesRead = src.getBytesRead();
		this->_serverConfigs = src.getServerConfigs();
		this->_request = src.getRequest();
		this->_requestRead = src.getRequestRead();
	}
	return *this;
}

int	Client::getClientSocket(void) const
{
	return this->_clientSocket;
}

socklen_t	Client::getClientAddrLen(void) const
{
	return this->_client_addr_len;
}

struct sockaddr_in	Client::getClientAddr(void) const
{
	return this->_client_addr;
}

std::string	Client::getRequestBuffer(void) const
{
	return this->_requestBuffer;
}

ssize_t	Client::getBytesRead(void) const
{
	return this->_bytesRead;
}

VirtualServer Client::getServerConfigs(void) const
{
	return this->_serverConfigs;
}

RequestParser Client::getRequest(void) const
{
	return this->_request;
}

bool		Client::getRequestRead(void) const
{
	return this->_requestRead;
}

struct sockaddr_in *Client::getClientAddrPointer(void)
{
    return &_client_addr;
}

socklen_t *Client::getClientAddrLenPointer(void)
{
    return &_client_addr_len;
}

void Client::setRequestBuffer(const std::string& requestBuffer)
{
    this->_requestBuffer = requestBuffer;
}

void Client::setClientSocket(const int& clientSocket)
{
   this->_clientSocket = clientSocket;
}

void Client::setClientAddrLen(const socklen_t& client_addr_len)
{
    this->_client_addr_len = client_addr_len;
}

void Client::setBytesRead(const ssize_t& bytesRead)
{
    this->_bytesRead = bytesRead;
}

void Client::setClientAddr(const struct sockaddr_in& client_addr)
{
	this->_client_addr = client_addr;
}

void Client::setServerConfigs(const VirtualServer& serverConfigs)
{
	this->_serverConfigs = serverConfigs;
}

void Client::setRequest(const RequestParser& request)
{
	this->_request = request;
}

void Client::setRequestRead(const bool& requestRead)
{
	this->_requestRead = requestRead;
}

short int	Client::receiveRequest(int client)
{
	
}