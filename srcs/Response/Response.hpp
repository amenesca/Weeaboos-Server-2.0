# pragma once

# include "../CgiHandler/CgiHandler.hpp"
# include "../Client/Client.hpp"
# include "../RequestParser/RequestParser.hpp"
# include <fstream>
# include <sys/wait.h> 
# include <sys/types.h>
# include <dirent.h>
# include <poll.h>

class Response {
    private:
		Client				_client;
        int					_status;
        std::string			_body;
        std::string			_header;
		std::string			_httpMessage;
		CgiHandler			_cgiHandler;
		ServerLog			_log;
		RequestParser		_request;
		std::vector<pollfd>	*_pollFds; // resolvendo questão da poll

    public:
        Response();
       	Response(const Client& client, std::vector<pollfd> *pollfds);
        ~Response();
		Response(const Response& copy);
		Response&	operator=(const Response& copy);

        int         getStatus() const;
        std::string getHeader() const;
		std::string getHttpMessage() const;
		Client		getClient() const;
		std::string getBody() const;
		CgiHandler  getCgiHandler() const;

        void        setStatus(int status);
		std::string	setHeader(std::string status, std::string contentType);
		std::string	toString(int number);
		std::string buildAutoindexPage(std::string & path);
		std::string CreatePath(const std::string& uri, int *locationPos);
		std::string createErrorPath(int errorStatus);
		std::string extractQueryString(const std::string& uri);
		std::string	readData(const std::string& path, const std::string& query);
		bool 		isCGIScript(const std::string& path);
		std::string executeCGI(const std::string& path, const std::string& queryString);
		std::string readStaticFile(const std::string& filePath);
		std::string deleteFile(const std::string& path);
		bool		MethodIsAllowed(int j);
		bool		fileExists(const std::string& path);
		int			addFdToPoll(int i);


        void    send();
        void    httpMethods();
		
		void handleGET();
		void handlePOST();
		void handleDELETE();
};