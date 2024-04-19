/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:30:46 by femarque          #+#    #+#             */
/*   Updated: 2024/04/19 13:11:49 by amenesca         ###   ########.fr       */
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
	_request(),
	_pollFds()
{}

Response::Response(const Client& client, std::vector<pollfd> *pollfds) 
:   _client(client),
    _status(0),
    _body(""),
    _header(""),
	_httpMessage(""),
	_cgiHandler(),
	_request(),
	_pollFds(pollfds)
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
		this->_request = src._request;
		this->_pollFds = src._pollFds;
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

bool	Response::MethodIsAllowed(int j)
{
	std::vector<std::string>	AllowedMethods = _client.getServerConfigs().getLocations()[j].getMethods();
	std::string					currentMethod = _client.getRequest().getMethod();

	for (size_t i = 1; i < AllowedMethods.size(); i++)
	{
		if (AllowedMethods[i] == currentMethod)
		{
			std::cout << "Entered here" << std::endl;
			return true;
			break;
		}
	}
	return false;
}

std::string Response::CreatePath(const std::string& uri, int *locationPos)
{
	std::string path = "";
    std::string uri_without_query;

    size_t pos = uri.find('?');

    if (pos != std::string::npos)
        uri_without_query = uri.substr(0, pos);
	else
        uri_without_query = uri;

	if (uri_without_query.find("/cgi-bin/tmp/") != std::string::npos || 
		uri_without_query.find("/images/") != std::string::npos)
	{
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
				*locationPos = j;
				if (!MethodIsAllowed(j))
				{
					std::cout << "teste 1" << std::endl;
					return "ERRO405";
				}
				path = _client.getServerConfigs().getRoot() + "/" + serverLocations[j].getIndex()[1];
				std::cout << "PATH FORMADO: " << path << std::endl;
				break;
			}
		}
	}
	else
		return "ERRO403";
	return path;
}

std::string Response::extractQueryString(const std::string& uri)
{
    std::string queryString;

    size_t pos = uri.find('?');

    if (pos != std::string::npos) {
        queryString = uri.substr(pos + 1);
    }

    return (queryString);
}

std::string Response::deleteFile(const std::string& path)
{
    if (remove(path.c_str()) != 0)
        return "Failed to delete the file";
    else
        return "File deleted successfully";
}

std::string Response::readData(const std::string& path, const std::string& query)
{
    if (isCGIScript(path))
        return executeCGI(path, query);
	else
        return readStaticFile(path);
}

bool Response::isCGIScript(const std::string& path)
{
    return (access(path.c_str(), X_OK) == 0);
}

int	Response::addFdToPoll(int fd)
{
	pollfd newPollFd;
	std::cout << "Adding new fd number: " << fd << " to poll" << std::endl;
	fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	newPollFd.fd = fd;
	newPollFd.events = POLLIN | POLLOUT;
	newPollFd.revents = 0;
	this->_pollFds->push_back(newPollFd);

	return (this->_pollFds->size() - 1);
}

std::string Response::executeCGI(const std::string& path, const std::string& queryString)
{
    int pipefd[2];
	// int pollPos[2];
    pipe(pipefd);
	
	// pollPos[0] = addFdToPoll(pipefd[0]);
	// pollPos[1] = addFdToPoll(pipefd[1]);
    pid_t pid = fork();

    if (pid == -1)
    {
        std::cerr << "Failed to fork" << std::endl;
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0)
    {
		std::vector<char*> args;
        args.push_back(strdup(path.c_str()));
		if (!queryString.empty())
			args.push_back(strdup(queryString.c_str()));
		args.push_back(NULL);
        if (access(path.c_str(), X_OK) == -1) {
			std::cerr << "Error on access: " << strerror(errno) << std::endl;
			exit(1);
		}
		close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        if (execve(path.c_str(), args.data(), NULL) == -1) {
            std::cerr << "Failed to execute CGI script" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
     	waitpid(pid, NULL, 0);
		close(pipefd[1]);

		std::stringstream new_string;
		char buffer[4096];
		ssize_t bytesRead;

		while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0)
		{
			new_string.write(buffer, bytesRead);
			if (bytesRead == 0 || bytesRead == -1)
			{
				close(pipefd[0]);
    			return "";
			}
		}

		close(pipefd[0]);
		return new_string.str();
    }
    return "";
}

std::string Response::readStaticFile(const std::string& path)
{
	std::ifstream file;

	file.open(path.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error opening file: " << path << std::endl;
		return "";
	}

    std::stringstream new_string;
    new_string << file.rdbuf();
    file.close();

    return (new_string.str());
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
	if (errorStatus == 500)
	{
		path = this->_client.getServerConfigs().getRoot() + "/" + this->_client.getServerConfigs().getErrorPage()[5];
	}
	if (errorStatus == 403)
	{
		path = this->_client.getServerConfigs().getRoot() + "/" + this->_client.getServerConfigs().getErrorPage()[6];
		
	}
	return path;
}

int	cgiExists(const std::string& pathToCgi)
{
	 std::ifstream file(pathToCgi.c_str());
    if (!file.is_open())
        return (1);
	else
	{
		file.close();
		return (0);
	}
}

std::string	Response::buildAutoindexPage(std::string & path)
{
    std::string     delimeter;
    std::string     page;
    DIR             *dir;
    struct dirent   *entry;

    delimeter = "\r\n";
	page.append("<!DOCTYPE html>").append(delimeter);
    page.append("<html lang=\"pt-br\">").append(delimeter);
    page.append("<head>").append(delimeter);
    page.append("    <meta charset=\"UTF-8\">").append(delimeter);
    page.append("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">").append(delimeter);
	page.append("    <title>DefaultPage</title>").append(delimeter);
    page.append("<body>").append(delimeter);
    page.append("    <h1>Index of ").append(path).append("</h1>").append(delimeter);
    page.append("    <ul>").append(delimeter);
    dir = opendir(path.c_str());
    if (dir != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            std::string item = entry->d_name;
            if (entry->d_type == DT_DIR)
            {
                page.append("            <li><a href=\"")\
                .append(item).append("/\">").append(item)\
                .append("/</a></li>").append(delimeter);
            }
            else
			{
                page.append("            <li><a href=\"")\
                .append(item).append("\">").append(item)\
                .append("/</a></li>").append(delimeter);
			}
		}
        closedir(dir);
    }
    page.append("    </ul>").append(delimeter);
    page.append("</body>").append(delimeter);
    page.append("</html>").append(delimeter);
    path.clear();
    
	return page;
}

void Response::handleGET()
{
	int locationPos = 0;
    std::string uri = _client.getRequest().getUri();
	std::string path = CreatePath(uri, &locationPos);
	std::string errorPath;
	
	if (path == "ERRO405")
	{
		errorPath = createErrorPath(405);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(405);
        setHeader("405 Method Not Allowed", "text/html");
		send();
		return;
	}
	if (path == "ERRO403")
	{
		errorPath = createErrorPath(403);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(403);
        setHeader("403 Forbidden", "text/html");
		send();
		return;
	}
	
	std::ifstream index;
	if (!path.empty())
		index.open(path.c_str());
	
	if (path.empty() || !index.is_open()) 
	{
		if (!path.empty())
		{
			if (this->_client.getServerConfigs().getLocations()[locationPos].getAutoIndex() == true)
			{
				path = this->_client.getServerConfigs().getRoot() + this->_client.getServerConfigs().getLocations()[locationPos].getPath();
				std::cout << "Path do autoindex: " << path << std::endl;
				_body = buildAutoindexPage(path);
				setStatus(200);
				setHeader("200 OK", "text/html");
				send();
				return;
			}
		}
        errorPath = createErrorPath(404);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(404);
        setHeader("404 Not Found", "text/html");
		send();
		return;
    }
	index.close();
    RequestParser _request(_client.getRequest());
	std::string query = extractQueryString(_request.getUri());
	std::string data = readData(path, query);
	
	if (data.empty()) 
	{	
        errorPath = createErrorPath(500);
		_body = readStaticFile(errorPath);
        setStatus(500);
        setHeader("500 Internal Server Error", "text/html");
		send();
		return;
    }
	else
	{
        _body = data;
        setStatus(200);
        setHeader("200 OK", "text/html");
	}
    send();
}

void Response::handlePOST()
{
	int locationPos = 0;
    std::string bodyData = _client.getRequest().getNewRequestBody();
    std::string path = CreatePath(this->_client.getRequest().getUri(), &locationPos);
	std::string errorPath;
	
	if (path == "ERRO405")
	{
		errorPath = createErrorPath(405);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(405);
        setHeader("405 Method Not Allowed", "text/html");
		send();
		return;
	}

	std::ifstream index;
	if (!path.empty())
		index.open(path.c_str());
	
	if (path.empty() || !index.is_open())
	{
		errorPath = createErrorPath(404);
		_body = readStaticFile(errorPath);
        setStatus(404);
        setHeader("404 Not Found", "text/html");
		send();
		return;
	}
	index.close();
	
    if (bodyData.empty())
	{
       errorPath = createErrorPath(400);
		_body = readStaticFile(errorPath);
        setStatus(400);
        setHeader("400 Bad Request", "text/plain");
		send();
		return;
    }
	else if (static_cast<int>(bodyData.size()) > _client.getServerConfigs().getMaxBodySize())
	{
		std::cout << "Body Size: " << static_cast<int>(bodyData.size()) << std::endl;
		std::cout << "Max Body Size: " <<_client.getServerConfigs().getMaxBodySize() << std::endl;
		
		errorPath = createErrorPath(413);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(413);
        setHeader("413 Request Entity Too Large", "text/html");
		send();
		return;
	}
	else
	{
        CgiHandler cgiHandler(this->_client.getRequest());
        std::string response = cgiHandler.postCgi(_client);
        _body = response;
        setStatus(200);
        setHeader("200 OK", "text/html");
    }
    send();
}

void Response::handleDELETE()
{
	int	locationPos = 0;
	std::string path = CreatePath(_client.getRequest().getUri(), &locationPos);
	std::string errorPath;
	
	if (path == "ERRO405")
	{
		errorPath = createErrorPath(405);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(405);
        setHeader("405 Method Not Allowed", "text/html");
		send();
		return;
	}
	if (path == "ERRO403")
	{
		errorPath = createErrorPath(403);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(403);
        setHeader("403 Forbidden", "text/html");
		send();
		return;
	}
	
	if (!fileExists(path))
	{
		errorPath = createErrorPath(404);
		std::cout << "Error Path: " << errorPath << std::endl;
		_body = readStaticFile(errorPath);
        setStatus(404);
        setHeader("404 Not Found", "text/html");
		send();
		return;
	}
	else
	{
		std::string response = deleteFile(path);
		_body = response;
		setStatus(200);
		setHeader("200 OK", "text/html");
	}
	send();
}

bool Response::fileExists(const std::string& path)
{
	std::ifstream file(path.c_str());
	if (!file.is_open())
		return (false);
	file.close();
	return (true);
}

void Response::httpMethods()
{
    if (_client.getRequest().getMethod() == "GET") {
		handleGET();
	}
    else if (_client.getRequest().getMethod() == "POST") {
        handlePOST();
    }
	else if (_client.getRequest().getMethod() == "DELETE") {
		handleDELETE();
	}
    else {
        std::string errorPath = createErrorPath(405);
		_body = readStaticFile(errorPath);
        setStatus(405);
        setHeader("405 Method Not Allowed", "text/plain");
    }
    return ;
}
