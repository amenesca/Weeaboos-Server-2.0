/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:37:17 by femarque          #+#    #+#             */
/*   Updated: 2024/04/01 15:24:48 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

CgiHandler::CgiHandler() :
	_pid(0),
	_env(),
	_request_pipe(),
	_request(),
	_log()
{
	_env = std::vector<char*>();
	_pid = 0;
	_request_pipe[0] = 0;
	_request_pipe[1] = 0;
}

CgiHandler::CgiHandler(RequestParser request) :
	_pid(0),
	_env(),
	_request_pipe(),
	_request(request),
	_log()
{

}

CgiHandler::CgiHandler(const CgiHandler& copy)
{
	*this = copy;
	return;
}

CgiHandler& CgiHandler::operator=(const CgiHandler& src)
{
	if (this != &src)
	{
		this->_env = src.getEnv();
		this->_pid = src.getPid();
		this->_request_pipe[0] = src.getRequestPipe1();
		this->_request_pipe[1] = src.getRequestPipe2();
		this->_request = src.getRequest();
		this->_log = src.getLog();
	}
	return *this;
}

pid_t	CgiHandler::getPid() const
{
	return this->_pid;
}

std::vector<char*>	CgiHandler::getEnv() const
{
	return this->_env;
}

int		CgiHandler::getRequestPipe1() const
{
	return this->_request_pipe[0];
}

int		CgiHandler::getRequestPipe2() const
{
	return this->_request_pipe[1];
}

RequestParser 		CgiHandler::getRequest() const
{
	return this->_request;
}

ServerLog		CgiHandler::getLog() const
{
	return this->_log;
}

CgiHandler::~CgiHandler() {
    for (std::vector<char*>::iterator it = _env.begin(); it != _env.end(); ++it)
	{
        delete[] *it;
    }
    _env.clear();
}

std::string CgiHandler::getScriptFilename(const std::string& requestURI) {
	size_t lastSlashPos = requestURI.find_last_of("/");
	if (lastSlashPos == std::string::npos)
		return (requestURI);
	return (requestURI.substr(lastSlashPos + 1));
}

std::vector<char*> CgiHandler::createEnv(std::map<std::string, std::string> requestHeaders, Client client) {
	std::string clientIp;
	std::string clientPort;
	
	clientIp = inet_ntoa(client.getClientAddrPointer()->sin_addr);
	clientPort = ntohs(client.getClientAddrPointer()->sin_port);
	_env.push_back(strdup(("CONTENT_TYPE=" + requestHeaders["Content-Type"]).c_str()));
	_env.push_back(strdup(("CONTENT_LENGTH=" + requestHeaders["Content-Length"]).c_str()));
	_env.push_back(strdup(("REQUEST_URI=" + _request.getUri()).c_str()));
	_env.push_back(strdup(("SCRIPT_NAME=" + _request.getUri().substr(1)).c_str()));
	_env.push_back(strdup(("SCRIPT_FILENAME=" + getScriptFilename(_request.getUri())).c_str()));

	_env.push_back(strdup(("REMOTE_ADDR=" + clientIp + ":" + clientPort).c_str()));
	_env.push_back(strdup(("SERVER_NAME=" + requestHeaders["Host"]).c_str()));
	_env.push_back(strdup(("SERVER_PORT=" + _request.getPortNumber()).c_str()));

	_env.push_back(strdup("AUTH_TYPE=Basic"));
	_env.push_back(strdup("REQUEST_METHOD=POST"));
	_env.push_back(strdup("REDIRECT_STATUS=200"));
	_env.push_back(strdup("DOCUMENT_ROOT=./"));
	_env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
	_env.push_back(strdup("PATH_INFO="));
	_env.push_back(strdup("PATH_TRANSLATED=.//"));
	_env.push_back(strdup("QUERY_STRING="));
	_env.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
	_env.push_back(strdup("SERVER_SOFTWARE=AMANIX"));
	_env.push_back(NULL);

	return (_env);
}

std::string CgiHandler::extractQueryString(const std::string& uri)
{
    std::string queryString;

    size_t pos = uri.find('?');

    if (pos != std::string::npos) {
        queryString = uri.substr(pos + 1);
    }

    return (queryString);
}

int CgiHandler::getCgi()
{
	//decidir o que fazer no get
	return (0);
}

int CgiHandler::postCgi(Client client)
{
	int response_pipe[2];
	std::vector<char*> headerEnv = createEnv(_request.getHeaders(), client);

	if (pipe(response_pipe) == -1)
    {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
    	exit(1);
    }
	if (pipe(_request_pipe) == -1)
    {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
    	exit(1);
    }

	antiBlock(_request_pipe, response_pipe);
	
	if (!writePipes(_request.getNewRequestBody(), _request.getContentLength())) {
        return (1);
	}

	_pid = fork();

	if (_pid == -1)
	{
		std::cerr << "Error on fork: " << strerror(errno) << std::endl;
		exit (1);
	}
	else if (_pid == 0)
	{
		std::vector<char*> argv;
		std::string path;
		path = _request.getUri().substr(1);
		argv.push_back(strdup(path.c_str()));
		argv.push_back(NULL);
		if (access(path.c_str(), X_OK) == -1) {
			std::cerr << "Error on access: " << strerror(errno) << std::endl;
			exit(1);
		}
		if (close(_request_pipe[1]) == -1) {
  			std::cerr << "Error on close: " << strerror(errno) << std::endl;
  			exit(1);
		}
		if (close(response_pipe[0]) == -1) {
  			std::cerr << "Error on close: " << strerror(errno) << std::endl;
  			exit(1);
		}
		if (dup2(_request_pipe[0], STDIN_FILENO) == -1) {
  			std::cerr << "Error on dup2: " << strerror(errno) << std::endl;
  			exit(1);
		}
		if (close(_request_pipe[0]) == -1) {
  			std::cerr << "Error on close: " << strerror(errno) << std::endl;
  			exit(1);
		}
		if (dup2(response_pipe[1], STDOUT_FILENO) == -1) {
  			std::cerr << "Error on dup2: " << strerror(errno) << std::endl;
  			exit(1);
		}
		if (close(response_pipe[1]) == -1) {
  			std::cerr << "Error on close: " << strerror(errno) << std::endl;
  			exit(1);
		}
		_log.createLog();
		if (execve(path.c_str(), argv.data(), headerEnv.data()) == -1) {
			std::cerr << "Error on execve: " << strerror(errno) << std::endl;
			exit(1);
		}
		return (0);
	}
	else
	{
		close(_request_pipe[0]);
		close(response_pipe[1]);
		return (1);
	}
	return (0);
}

void CgiHandler::antiBlock(int *pipe1, int *pipe2)
{
    if (fcntl(pipe1[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cerr << "Error on fcntl: " << strerror(errno) << std::endl;
	}
	if (fcntl(pipe1[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cerr << "Error on fcntl: " << strerror(errno) << std::endl;
	}
	if (fcntl(pipe2[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cerr << "Error on fcntl: " << strerror(errno) << std::endl;
	}
	if (fcntl(pipe2[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1) {
		std::cerr << "Error on fcntl: " << strerror(errno) << std::endl;
	}
}

bool        CgiHandler::writePipes(std::string message, int contentLength)
{
    ssize_t	bytesWritten = 0;
	ssize_t bytesLeftToWrite = static_cast<ssize_t>(contentLength);
	ssize_t totalBytesWritten = 0;
	ssize_t	sizeToWrite = 4096;

	if (static_cast<ssize_t>(contentLength) < sizeToWrite)
	{
		sizeToWrite = contentLength;
	}
	std::cout << "CONTENT LENGTH NO WRITE PIPES: " << contentLength << std::endl;

	while (bytesLeftToWrite != 0)
	{
  		bytesWritten = write(_request_pipe[1], message.c_str() + totalBytesWritten, sizeToWrite);
		
		if (bytesWritten == -1)
			break;
		
		bytesLeftToWrite -= bytesWritten;
		totalBytesWritten += bytesWritten;
		if (bytesLeftToWrite < 4096)
		{
			sizeToWrite = 	bytesLeftToWrite;
		}
		std::cout << "BYTESWRITTEN NO WRITE PIPES: " << bytesWritten << std::endl;
	}
	std::cout << "TOTAL BYTESWRITTEN NO WRITE PIPES: " << totalBytesWritten << std::endl;
    if (bytesWritten == -1)
	{
		std::cout << "Bytes Writen == -1" << std::endl;
        return (false);
    }

    if (static_cast<size_t>(totalBytesWritten) != message.size())
	{
		std::cout << "Bytes Writen != message.size()" << std::endl;
        return (false);
    }

    return (true);
}
