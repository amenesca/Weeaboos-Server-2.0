# pragma once

# include "../CgiHandler/CgiHandler.hpp"
# include "../Client/Client.hpp"
# include <fstream>
#include <sys/wait.h> // Para a função waitpid
#include <sys/types.h> // Para a função waitpid

class Response {
    private:
		Client			_client;
        int				_status;
        std::string		_body;
        std::string		_header;
		std::string		_httpMessage;
		CgiHandler		_cgiHandler;
		ServerLog		_log;
    public:
        Response();
       	Response(const Client& client);
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
		std::string CreatePath(const std::string& uri);
		std::string createErrorPath(int errorStatus);
		std::string	readData(const std::string& path);
		bool isCGIScript(const std::string& path);
		std::string executeCGI(const std::string& scriptPath);
		std::string readStaticFile(const std::string& filePath);

        void    send();
        void    httpMethods();
		
		void handleGET();
		void handlePOST();
};