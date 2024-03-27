/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:30:46 by femarque          #+#    #+#             */
/*   Updated: 2024/03/26 22:27:32 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
:   _client(),
    _status(0),
    _body(""),
    _header(""),
    _httpMessage(""),
	_cgiHandler()
{}

Response::Response(Client client) 
:   _client(client),
    _status(0),
    _body(""),
    _header(""),
	_httpMessage(""),
	_cgiHandler()
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

std::string Response::readData(const std::string& uri)
{
    std::string path;
	// Mudei um monte de coisa pq a config vem do cliente!!!
    if (_client.getServerConfigs().getLocations().size() > 1) {
        for (size_t i = 0; i < _client.getServerConfigs().getLocations().size(); i++) {
            if (_client.getServerConfigs().getLocations()[i].getPath() == uri) {
                path = _client.getServerConfigs().getRoot() + "/" + _client.getServerConfigs().getLocations()[i].getIndex()[1];
//              std::cout << path << std::endl;
				break;
            }
        }
    }
    else {
        path = _client.getServerConfigs().getRoot() + uri + _client.getServerConfigs().getLocations()[0].getIndex()[1];
	}
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
//	std::cout << "Valor da URI" << uri << std::endl;
    std::string	data = readData(uri);
     if (!data.empty()) {
        setStatus(200);
        _body = readData(uri);
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
//    for (size_t i = 0; i < bodyData.size(); i++) {
//        std::cout << "BODY: " << bodyData[i] << "\n";
//    }
	
    std::string uri = _client.getRequest().getUri();
//	std::cout << "PROCURANDO URI: " << uri << "\n";
	
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
