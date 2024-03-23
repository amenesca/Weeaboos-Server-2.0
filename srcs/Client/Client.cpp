/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:33:07 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/22 22:07:50 by amenesca         ###   ########.fr       */
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
	_firstTimeRequest(true),
	_serverConfigs(),
	_requestParser()
{
	
}

Client::~Client()
{
	this->_clientSocket = -1;
	this->_client_addr_len = 0;
	memset(&this->_client_addr,0,sizeof(this->_client_addr));
	_requestRead = false;
	_firstTimeRequest = false;
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
		this->_requestParser = src.getRequest();
		this->_requestRead = src.getRequestRead();
		this->_firstTimeRequest = src.getFirstTimeRequest();
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
	return this->_requestParser;
}

bool		Client::getRequestRead(void) const
{
	return this->_requestRead;
}

bool		Client::getFirstTimeRequest(void) const
{
	return this->_firstTimeRequest;
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
	this->_requestParser = request;
}

void Client::setRequestRead(const bool& requestRead)
{
	this->_requestRead = requestRead;
}

void Client::setFirstTimeRequest(const bool& firstTimeRequest)
{
	this->_firstTimeRequest = firstTimeRequest;
}

std::string Client::uint8_to_string(const uint8_t* data, size_t size) {
    return std::string(reinterpret_cast<const char*>(data), size);
}

short int	Client::receiveRequest(int client)
{
	uint8_t	buffer[4096];
	int		bytes;

	memset(buffer, 0, sizeof(uint8_t) * 4096);

	bytes = recv(client, buffer, 4095, 0);
	if (bytes == -1)
		return false;
	else if (bytes == 0)
	{
		std::cout << "Client disconnected" << std::endl;
		return false;
	}
	
	std::cout << buffer << std::endl;
	
	if (this->_firstTimeRequest == true)
	{
		this->_requestParser.parse(uint8_to_string(buffer, bytes));
	}

	if (this->_requestParser.getMethod() == "GET" \
		|| this->_requestParser.getMethod() == "DELETE")
	{
		this->setRequestRead(true);
		return true;
	}
	if (this->_requestParser.getMethod() == "POST" && \
		this->_firstTimeRequest == false)
	{
		this->_requestParser.appendBody(uint8_to_string(buffer, bytes));
		if (this->_requestParser.getTotalBytesOnBody() == \
			this->_requestParser.getContentLenght())
		{
			this->setRequestRead(true);
			return true;
		}
	}
	
	if (this->_firstTimeRequest == true)
		this->setFirstTimeRequest(false);
	return true;
}