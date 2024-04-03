/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 00:37:17 by femarque          #+#    #+#             */
/*   Updated: 2024/04/03 16:54:25 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

CgiHandler::CgiHandler() :
	_pid(0),
	_env(),
	_request_pipe(),
	_request(),
	_client(),
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
	_client(),
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
		this->_client = src.getClient();
		this->_log = src.getLog();
		this->_start_time = src._start_time;
		this->_active = src._active;
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

Client 		CgiHandler::getClient() const
{
	return this->_client;
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

std::string UriWithoutQuery(const std::string& uri)
{
	    std::string uri_without_query;

    // Encontra a posição do caractere '?'
    size_t pos = uri.find('?');

    // Se '?' foi encontrado, extrai a parte da URI antes dele
    if (pos != std::string::npos) {
        uri_without_query = uri.substr(0, pos);
    } else {
        uri_without_query = uri; // Se não houver '?', usa a URI completa
    }
	return uri_without_query;
}

int CgiHandler::getCgi(Client client)
{
	int response_pipe[2];
	std::vector<char*> headerEnv = createEnv(_request.getHeaders(), client);

	if (pipe(response_pipe) == -1)
    {
        std::cerr << "Error creating pipe: " << strerror(errno) << std::endl;
    	exit(1);
    }

	_pid = fork();
	_active = true;
	if (_pid == -1)
	{
		std::cerr << "Error on fork: " << strerror(errno) << std::endl;
		exit (1);
	}
	else if (_pid == 0)
	{
		std::vector<char*> argv;
		std::string path;
		std::string root = client.getServerConfigs().getRoot();
		path = root + "/" + UriWithoutQuery(_request.getUri().substr(1));
		std::cout << "PATH: " << path << std::endl;
		std::string querry = extractQueryString(_request.getUri());
		std::cout << "QUERY: " << querry << std::endl;
		argv.push_back(strdup(path.c_str()));
		argv.push_back(NULL);
		if (close(response_pipe[0]) == -1) {
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
		//_log.createLog();
		
		if (execve(path.c_str(), argv.data(), headerEnv.data()) == -1) {
			std::cerr << "Error on execve: " << strerror(errno) << std::endl;
			exit(1);
		}
		return (0);
	}
	else
	{
		close(response_pipe[1]);
		if (!checkAvailability(response_pipe[0]))
		{
			return (-1);
		}
		return (1);
	}
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
	
	if (!writePipes(_request.getNewRequestBody())) {
        return (1);
	}

	_pid = fork();
	_active = true;
	if (_pid == -1)
	{
		std::cerr << "Error on fork: " << strerror(errno) << std::endl;
		exit (1);
	}
	else if (_pid == 0)
	{
		std::vector<char*> argv;
		std::string path;
		std::string root = client.getServerConfigs().getRoot();
		path = root + "/" + UriWithoutQuery(_request.getUri().substr(1));
		std::cout << "PATH: " << path << std::endl;
		std::string querry = extractQueryString(_request.getUri());
		std::cout << "QUERY: " << querry << std::endl;
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
		//_log.createLog();
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
		if (!checkAvailability(response_pipe[0]))
		{
			return (-1);
		}
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

bool        CgiHandler::writePipes(std::string message)
{
    size_t  bytesWritten;
    ssize_t bytes;

    bytesWritten = 0;
    while (bytesWritten < message.length())
    {
        bytes = write(this->_request_pipe[1], message.c_str() + bytesWritten, \
                    message.length() - bytesWritten);
        if (bytes == -1)
        {
            return (false);
        }
        bytesWritten += bytes;
    }
    return (true);
}

/*bool CgiHandler::writePipes(std::string message, size_t contentLength)
{
    size_t bytesWritten = 0;
    size_t bufferSize = 4096; // Tamanho do buffer para cada parte da imagem
    while (bytesWritten < contentLength)
    {
        size_t bytesToWrite = std::min(bufferSize, contentLength - bytesWritten);
        ssize_t bytes = write(this->_request_pipe[1], message.c_str() + bytesWritten, bytesToWrite);
        if (bytes == -1)
        {
            return false;
        }
        bytesWritten += bytes;
    }
    return true;
}*/

int	CgiHandler::readPipes(int fd)
{
    char    buffer[BUFFER_SIZE_CGI];
    int     bytesRead;

    bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead > 0)
    {
        this->_response.append(buffer, bytesRead);
        return (bytesRead);
    }
    else
    {
        return (0);
    }
}

bool CgiHandler::checkAvailability(int fd)
{
    time_t startTime = time(NULL);
    time_t currentTime;
    int bytes;

    while (this->_active)
    {
        currentTime = time(NULL);
        if (difftime(currentTime, startTime) >= TIME_LIMIT)
        {
            kill(this->_pid, SIGKILL);
            this->_active = false;
            return (false);
        }
        bytes = readPipes(fd);
        if (bytes > 0)
        {
            this->_active = false;
            return true;
        }
		usleep(10000);
    }
    return (true);
}