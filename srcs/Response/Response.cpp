/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:30:46 by femarque          #+#    #+#             */
/*   Updated: 2024/04/03 14:05:04 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
:   _client(),
    _status(0),
    _body(""),
    _header(""),
    _httpMessage(""),
	_cgiHandler(),
	_allServersConfigs()
{}

Response::Response(const Client& client, const std::vector<VirtualServer>& allServers) 
:   _client(client),
    _status(0),
    _body(""),
    _header(""),
	_httpMessage(""),
	_cgiHandler(),
	_allServersConfigs(allServers)
{}

Response::~Response() {}

Response&	Response::operator=(const Response& src)
{
	if (this != &src)
	{
		this->_client = src.getClient();
		this->_status = src.getStatus();
		this->_body = src.getBody();
		this->_header = src.getHeader();
		this->_httpMessage = src.getHttpMessage();
		this->_cgiHandler = src.getCgiHandler();
		this->_allServersConfigs = src.getAllServersConfigs();
	}
	return *this;
	
}

Response::Response(const Response& copy)
{
	*this = copy;
	return;
}

int         Response::getStatus() const
{
    return this->_status;
}

std::string Response::getHeader() const
{
    return this->_header;
}

std::string Response::getHttpMessage() const
{
	return this->_httpMessage;
}

Client		Response::getClient() const
{
	return this->_client;
}

std::string Response::getBody() const
{
	return this->_body;
}

CgiHandler	Response::getCgiHandler() const
{
	return this->_cgiHandler;	
}

std::vector<VirtualServer> Response::getAllServersConfigs() const
{
	return this->_allServersConfigs;
}

void    Response::setStatus(int status)
{
    this->_status = status;
}

std::string Response::toString(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

std::string    Response::setHeader(std::string status, std::string contentType) {
    std::string header;

    header.append("HTTP/1.1 ").append(status).append("\r\n");
    header.append("Content-Type: ").append(contentType).append("\r\n");
    header.append("Content-Length: ").append(toString(_body.size()));
    header.append("\r\n").append("\r\n");

    _header = header;
    return(header);
}

void Response::send() {
   _httpMessage = _header;
   _httpMessage.append(_body);
}

std::string Response::CreatePath(const std::string& uri)
{
	std::string path;
    std::string uri_without_query;

    // Encontra a posição do caractere '?'
    size_t pos = uri.find('?');

    // Se '?' foi encontrado, extrai a parte da URI antes dele
    if (pos != std::string::npos) {
        uri_without_query = uri.substr(0, pos);
    } else {
        uri_without_query = uri; // Se não houver '?', usa a URI completa
    }

	std::string		hostToCheck = _client.getRequest().getHeaders()["Host"];
	bool			timeToBreak = false;
    // Resto do código permanece o mesmo, usando uri_without_query
	for (size_t i = 0; i < this->_allServersConfigs.size(); i++)
	{
		VirtualServer	currentServer  = _allServersConfigs[i];
		std::cout << "Current Server Host: " << currentServer.getStrHost() << std::endl;
		std::cout << "Request Host: " << hostToCheck << std::endl;
		if (currentServer.getStrHost() == hostToCheck)
		{
			timeToBreak = true;
			std::vector<Location> currentServerLocations = currentServer.getLocations();
			for (size_t j = 0; j < currentServerLocations.size(); j++) 
			{
				std::cout << "Path da location: " << currentServerLocations[j].getPath() << std::endl;
				std::cout << "Uri sem query: " << uri_without_query << std::endl;
				if (currentServerLocations[j].getPath() == uri_without_query) 
				{
					path = _client.getServerConfigs().getRoot() + "/" + currentServerLocations[j].getIndex()[1];
					std::cout << "PATH FORMADO: " << path << std::endl;
					break;
				}
			}
			if (timeToBreak == true)
				break;
		}
	}
	return path;
}

std::string Response::readData(const std::string& uri)
{
	std::string path = CreatePath(uri);
	
    std::cout << "PATH RESPONSE: " << path << std::endl;
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        std::cout << "Error opening index.html" << std::endl;
        return ("");
    }
    std::string data;
    std::string line;
    while (std::getline(file, line)) {
        data += line;
        data += "\n";
    }
    file.close();
    return (data);
}

void Response::handleGET()
{
    std::string	uri = _client.getRequest().getUri();
	std::cout << "Valor da URI: " << uri << std::endl;
    std::string	data = readData(uri);
     if (!data.empty()) {
        _body = readData(uri);
        CgiHandler get_cgi = CgiHandler(_client.getRequest());
        get_cgi.getCgi(_client);
        setStatus(200);
        setHeader("200 OK", "text/html");
        send();
    }
    else
    {
        setStatus(404);
        setHeader("404 Not Found", "text/html");
        _body = "404 Not Found";
    }
}

void Response::handlePOST()
{
	std::string bodyData = _client.getRequest().getNewRequestBody();
//	std::cout << bodyData << "\nBody Printado"<< std::endl;
	
    std::string uri = _client.getRequest().getUri();
	std::cout << "PROCURANDO URI: " << uri << "\n";
	
    if (_client.getRequest().getNewRequestBody().empty()) {
        setStatus(400);
        setHeader("400 Bad Request", "text/plain");
        _body = "400 Bad Request: No request body found";
        return ;
    }
    
    CgiHandler post_cgi = CgiHandler(_client.getRequest());
//    std::cout << "ANTES DO CGIHANDLER\n";
    post_cgi.postCgi(_client);
    setStatus(200);
    send();
}

void Response::httpMethods()
{
    if (_client.getRequest().getMethod() == "GET") {
//		std::cout << "Handle get" << std::endl;
		handleGET();
	}
    else if (_client.getRequest().getMethod() == "POST") {
//		std::cout << "HANDLE POST" << std::endl;
        handlePOST();
    }
    else {
        setStatus(405); // 405 =  Method Not Allowed
        setHeader("405 Method Not Allowed", "text/plain");
        _body = "405 Method Not Allowed";
    }
    return ;
}
