#include "ServerLog.hpp"

ServerLog::ServerLog() :
	_stdout_fd(0),
	_stderr_fd(0)
{
    _stdout_fd = 0;
    _stderr_fd = 0;
}

ServerLog::~ServerLog()
{
    close(_stdout_fd);
    close(_stderr_fd);
}

ServerLog::ServerLog(const ServerLog& copy)
{
	*this = copy;
	return ;
}

ServerLog&	ServerLog::operator=(const ServerLog& src)
{
	if (this != &src)
	{
		this->_stderr_fd = src.getStdErrFd();
		this->_stdout_fd = src.getStdOutFd();
	}
	return *this;
}

int ServerLog::getStdOutFd() const
{
	return this->_stdout_fd;
}

int ServerLog::getStdErrFd() const
{
	return this->_stderr_fd;
}

void ServerLog::createLog()
{
    _stdout_fd = open("srcs/ServerLog/logs/stdout.log", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    _stderr_fd = open("srcs/ServerLog/logs/stderr.log", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    dup2(_stdout_fd, STDOUT_FILENO);
    dup2(_stderr_fd, STDERR_FILENO);
    close(_stdout_fd);
    close(_stderr_fd);
}