/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:30:46 by femarque          #+#    #+#             */
/*   Updated: 2024/04/04 15:58:36 by amenesca         ###   ########.fr       */
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

Response::Response(const Client& client) 
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

std::string Response::CreatePath(const std::string& uri)
{
	std::string path = "";
    std::string uri_without_query;

    // Encontra a posição do caractere '?'
    size_t pos = uri.find('?');

    // Se '?' foi encontrado, extrai a parte da URI antes dele
    if (pos != std::string::npos) {
        uri_without_query = uri.substr(0, pos);
    } else {
        uri_without_query = uri; // Se não houver '?', usa a URI completa
    }

	if (uri_without_query.find("/cgi-bin/tmp/") != std::string::npos || 
		uri_without_query.find("/images/") != std::string::npos) {
		path = _client.getServerConfigs().getRoot() + uri_without_query;
		return path;
	}

	if (this->_client.getServerConfigs().getStrHost() == this->_client.getRequest().getHeaders()["Host"])
	{	
		std::vector<Location> serverLocations = this->_client.getServerConfigs().getLocations();
		for (size_t j = 0; j < serverLocations.size(); j++) 
		{
			std::cout << "Path da location: " << serverLocations[j].getPath() << std::endl;
			std::cout << "Uri sem query: " << uri_without_query << std::endl;
			if (serverLocations[j].getPath() == uri_without_query) 
			{
				path = _client.getServerConfigs().getRoot() + "/" + serverLocations[j].getIndex()[1];
				std::cout << "PATH FORMADO: " << path << std::endl;
				break;
			}
		}
	}
	return path;
}

std::string Response::readData(const std::string& uri)
{
    std::string path = CreatePath(uri);

    std::cout << "PATH RESPONSE: " << path << std::endl;

    // Verifica se o recurso é um script CGI
    if (isCGIScript(path)) {
        return executeCGI(path);
    } else {
        return readStaticFile(path);
    }
}

bool Response::isCGIScript(const std::string& path)
{
    // Verifica se o arquivo existe e é executável
    return (access(path.c_str(), X_OK) == 0);
}

std::string Response::executeCGI(const std::string& scriptPath)
{
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();

    if (pid == -1)
    {
        std::cerr << "Failed to fork" << std::endl;
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0)
    {
        std::cout << "Executando script CGI: " << scriptPath << std::endl;
        if (access(scriptPath.c_str(), X_OK) == -1) {
			std::cerr << "Error on access: " << strerror(errno) << std::endl;
			exit(1);
		}
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);

        std::vector<char*> args;
        args.push_back(strdup(scriptPath.c_str()));
        //args.push_back(strdup(uri.c_str()));
		args.push_back(NULL);
        //char* args[] = {const_cast<char*>(scriptPath.c_str()), const_cast<char*>(uri.c_str()), NULL};
        if (execve(scriptPath.c_str(), args.data(), NULL) == -1) {
            std::cerr << "Failed to execute CGI script" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cerr << "Failed to execute CGI script" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        // Estamos no processo pai
        waitpid(pid, NULL, 0);
        // Fecha o lado de escrita do pipe
        close(pipefd[1]);

        // Lê a saída do script CGI a partir do pipe
        std::stringstream ss;
        char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            ss.write(buffer, bytesRead);
        }

        return ss.str();
    }
}

std::string Response::readStaticFile(const std::string& filePath)
{
    std::ifstream file(filePath.c_str());
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    return ss.str();
}

std::string Response::createErrorPath(int errorStatus)
{
	std::string path;
	if (errorStatus == 404)
	{
		path = this->_client.getServerConfigs().getRoot() + "/" + this->_client.getServerConfigs().getErrorPage()[2];
	}
	if (errorStatus == 400)
	{
		path = this->_client.getServerConfigs().getRoot() + "/" + this->_client.getServerConfigs().getErrorPage()[1];
	}
	if (errorStatus == 405)
	{
		path = this->_client.getServerConfigs().getRoot() + "/" + this->_client.getServerConfigs().getErrorPage()[3];
	}
	if (errorStatus == 413)
	{
		path = this->_client.getServerConfigs().getRoot() + "/" + this->_client.getServerConfigs().getErrorPage()[4];
	}
	return path;
}

int	cgiExists(const std::string& pathToCgi)
{
	 std::ifstream file(pathToCgi.c_str());
    if (!file.is_open())
	{
        return 1;
    } 
	else
	{
		file.close();
		return 0;
	}
}

void Response::handleGET()
{
    std::string uri = _client.getRequest().getUri();
    std::cout << "Valor da URI: " << uri << std::endl;
    std::string data = readData(uri);
//    std::cout << "Data do get:" << data << std::endl;
	std::string errorPath;
	

	if (!data.empty()) 
	{
		std::cout << "Entrou aqui data certo" << std::endl;
        _body = data;
        setStatus(200);
        setHeader("200 OK", "text/html");
    } 
	else 
	{	
		std::cout << "Entrou aqui data errado" << std::endl;
        errorPath = createErrorPath(404);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(404);
        setHeader("404 Not Found", "text/html");
    }
    send();
}

void Response::handlePOST()
{
    std::string bodyData = _client.getRequest().getNewRequestBody();
    std::string path = CreatePath(this->_client.getRequest().getUri());
	std::cout << "PATH DO POST: " << path << std::endl;

	if (static_cast<int>(bodyData.size()) > _client.getServerConfigs().getMaxBodySize())
	{
		std::string errorPath = createErrorPath(413);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(413);
        setHeader("413 Request Entity Too Large", "text/html");
		send();
		return;
	}
	if (path.empty()) // Alteração de alan
	{
		std::string errorPath = createErrorPath(404);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(404);
        setHeader("404 Not Found", "text/html");
		send();
		return;
	} // fim da alteração de alan
    if (bodyData.empty())
	{
        std::string errorPath = createErrorPath(400);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(400);
        setHeader("400 Bad Request", "text/plain");
    } 
	else
	{
        // Assume que o POST é sempre para um script CGI
        CgiHandler cgiHandler(_client.getRequest());
        std::string response = cgiHandler.postCgi(_client);
        _body = response;
        setStatus(200);
        setHeader("200 OK", "text/html");
    }
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
        std::string errorPath = createErrorPath(405);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(405); // 405 =  Method Not Allowed
        setHeader("405 Method Not Allowed", "text/plain");
    }
    return ;
}
