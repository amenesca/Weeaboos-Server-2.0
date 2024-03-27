#pragma once

# include "../Client/Client.hpp"
# include "../ServerLog/ServerLog.hpp"

class CgiHandler {
    private:
        pid_t               _pid;
        std::vector<char*>  _env;
        int                 _request_pipe[2];
        RequestParser       _request;
        ServerLog           _log;
        void                antiBlock(int *pipe1, int *pipe2);
        bool                writePipes(std::string message);
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
        
        std::vector<char*>  createEnv(std::map<std::string, std::string> requestHeaders, Client client);
        std::string         extractQueryString(const std::string& uri);
        int                 getCgi();
        int                 postCgi(Client client);
		std::string         getScriptFilename(const std::string& requestURI);
};