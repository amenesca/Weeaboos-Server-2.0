#pragma once

# include "../Client/Client.hpp"
# include "../ServerLog/ServerLog.hpp"
# include <ctime>
# include <signal.h>

#define BUFFER_SIZE_CGI 64 * 1024
#define TIME_LIMIT 2

class CgiHandler {
    private:
        pid_t               _pid;
        std::vector<char*>  _env;
        int                 _request_pipe[2];
        RequestParser       _request;
        Client              _client;
        ServerLog           _log;
        time_t              _start_time;
        std::string         _response;
        bool                _active;
        void                antiBlock(int *pipe1, int *pipe2);
        bool                writePipes(std::string message, size_t contentLength);
        int                 readPipes(int fd);
        bool                checkAvailability(int fd);
	public:
        CgiHandler();
        CgiHandler(RequestParser request);
        ~CgiHandler();
		CgiHandler(const CgiHandler& copy);
		CgiHandler& operator=(const CgiHandler& src);

		pid_t				getPid() const;
		std::vector<char*>	getEnv() const;
		int					getRequestPipe1() const;
		int					getRequestPipe2() const;
		RequestParser 		getRequest() const;
		ServerLog			getLog() const;
        Client              getClient() const;
        
        std::vector<char*>  createEnv(std::map<std::string, std::string> requestHeaders, Client client);
        std::string         extractQueryString(const std::string& uri);
        int                 getCgi(Client client);
        int                 postCgi(Client client);
		std::string         getScriptFilename(const std::string& requestURI);
};