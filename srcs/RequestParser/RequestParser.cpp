/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 17:11:42 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/28 16:54:39 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./RequestParser.hpp"

RequestParser::RequestParser() :
	_requestMethod(""),
	_uri(""),
	_httpVersion(""),
	_portNumber(""),
	_queryString(""),
	_newRequestBody(""),
	_requestHeaders(),
	_stringBuffer(""),
	_contentLenght(0)
{
}

RequestParser::~RequestParser()
{
	_requestMethod.clear();
	_uri.clear();
	_httpVersion.clear();
	_portNumber.clear();
	_queryString.clear();
	_newRequestBody.clear();
	_requestHeaders.clear();
	_stringBuffer.clear();
	_contentLenght = 0;
}

RequestParser::RequestParser(std::string request) :
	_requestMethod(""),
	_uri(""),
	_httpVersion(""),
	_portNumber(""),
	_queryString(""),
	_newRequestBody(""),
	_requestHeaders(),
	_stringBuffer(""),
	_contentLenght(0)
{
	this->parse(request);
}

RequestParser::RequestParser(const RequestParser& copy)
{
	*this = copy;
}

RequestParser& RequestParser::operator=(const RequestParser& copy)
{
	if (this != &copy)
	{
		this->_httpVersion = copy.getHttpVersion();
		this->_newRequestBody = copy.getNewRequestBody();
		this->_requestHeaders = copy.getHeaders();
		this->_requestMethod = copy.getMethod();
		this->_uri = copy.getUri();
		this->_portNumber = copy.getPortNumber();
		this->_stringBuffer = copy.getStringBuffer();
		this->_contentLenght = copy.getContentLenght();
	}
	return *this;
}

void RequestParser::parse(std::string request)
{
    // Declaring variables
//	std::cout << "--- PARSER DA REQUEST ---" << std::endl;
	
    std::string requestLine, headerLine, bodyLine;
    std::istringstream requestStream(request);

    // Solicitation Line parsing
    std::getline(requestStream, requestLine);
    std::istringstream requestLineStream(requestLine);
    requestLineStream >> this->_requestMethod >> this->_uri >> this->_httpVersion;

    // Header parsing
    while(std::getline(requestStream, headerLine) && headerLine != "\r\n\r\n")
	{
        size_t separator = headerLine.find(": ");
        if (separator != std::string::npos)
		{
            std::string key = headerLine.substr(0, separator);
            std::string value = headerLine.substr(separator + 2); // + 2 para ignorar ": "

            // Se a chave for "Host", extrair apenas o conteúdo antes do ":"
            if (key == "Host")
			{
                size_t portSeparator = value.find(":");
                if (portSeparator != std::string::npos)
				{
                    std::string hostWithoutPort = value.substr(0, portSeparator);
					std::string portNumber = value.substr(portSeparator + 1);

					value = hostWithoutPort;
					_portNumber = portNumber;
                }
            }

            this->_requestHeaders[key] = value;
			if (headerLine.find("Sec-Fetch-User: ?1") != std::string::npos)
			{
				std::cout << "Condição funcionou" << std::endl;
				break;
			}
        }
    }
    // Body parsing
	if (_requestMethod == "POST")
	{
		this->_contentLenght = std::atoi(this->_requestHeaders["Content-Length"].c_str());
    }
    return;
}

void RequestParser::startBody(ssize_t bytesReceived, ssize_t bodyPosition,const u_int8_t* buffer) {
    // Verificar se a posição do corpo está dentro dos limites do buffer
    if (bodyPosition >= 0 && bodyPosition < bytesReceived) {
        // Calcular o tamanho do corpo (bytes restantes após a posição do corpo)
        ssize_t totalBytesToAdd = bytesReceived - bodyPosition;

        // Adicionar o corpo à requisição usando o método append
        this->_newRequestBody.append(reinterpret_cast<const char*>(buffer + bodyPosition), totalBytesToAdd);
    }
}

void RequestParser::appendBody(const u_int8_t* buffer, ssize_t bytesReceived)
{
	this->_newRequestBody.append(reinterpret_cast<const char*>(buffer), bytesReceived);
}

int RequestParser::_validateUri()
{
    // Verificar se a URI começa com uma barra "/"
    if (_uri.empty() || _uri[0] != '/')
	{
        return -1; // Deu merda
    }

    // Verificar se os caracteres da URI são válidos
    for (std::size_t i = 1; i < _uri.length(); ++i)
	{
        char ch = _uri[i];

        if (!std::isalnum(ch) && ch != '/' && ch != '_' && ch != '-') {
            return -1; // Deu merda    
        }
    }

    return 0;
}

void RequestParser::validateRequestLine()
{
	if (_requestMethod != "GET" && _requestMethod != "POST" && _requestMethod != "DELETE")
	{
		throw invalidMethod();
	}

	if (_httpVersion != "HTTP/1.1")
	{
		throw invalidHttpVersion();
	}
	_validateUri();
}

std::string RequestParser::getMethod() const
{
	return this->_requestMethod;
}

std::string RequestParser::getUri() const
{
	return this->_uri;
}

std::string RequestParser::getHttpVersion() const
{
	return this->_httpVersion;
}

std::string RequestParser::getPortNumber() const
{
	return this->_portNumber;
}

std::map<std::string, std::string> RequestParser::getHeaders() const
{
	return this->_requestHeaders;
}

std::string RequestParser::getQueryString()
{
    size_t pos = _uri.find('?');
    if (pos != std::string::npos)
	{
        _queryString = _uri.substr(pos + 1);
    }
    return _queryString;
}

std::string RequestParser::getStringBuffer(void) const
{
	return this->_stringBuffer;
}

int			RequestParser::getContentLenght(void) const
{
	return this->_contentLenght;
}

std::string	RequestParser::getNewRequestBody(void) const
{
	return this->_newRequestBody;
}

void	RequestParser::setContentLenght(const int& contentLenght)
{
	this->_contentLenght = contentLenght;
}

const char *RequestParser::invalidMethod::what() const throw()
{
	return "Error: invalid method in request";
}

const char *RequestParser::invalidHttpVersion::what() const throw()
{
	return "Error: invalid http version";
}
