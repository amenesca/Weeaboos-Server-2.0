# pragma once

# include "../CgiHandler/CgiHandler.hpp"
# include "../Client/Client.hpp"
# include <fstream>

class Response {
    private:
		Client						_client;
        int							_status;
        std::string					_body;
        std::string					_header;
		std::string					_httpMessage;
		CgiHandler					_cgiHandler;
		std::vector<VirtualServer> _allServersConfigs;
    public:
        Response();
       	Response(const Client& client, const std::vector<VirtualServer>& allServers);
        ~Response();
		Response(const Response& copy);
		Response&	operator=(const Response& copy);

        int         				getStatus() const;
        std::string 				getHeader() const;
		std::string 				getHttpMessage() const;
		Client						getClient() const;
		std::string 				getBody() const;
		CgiHandler					getCgiHandler() const;
		std::vector<VirtualServer>	getAllServersConfigs() const;

        void        setStatus(int status);
		std::string	setHeader(std::string status, std::string contentType);
		std::string	toString(int number);
		std::string CreatePath(const std::string& uri);
		std::string	readData(const std::string& uri);
        void    send();
        void    httpMethods();
		
		void handleGET();
		void handlePOST();
};