#pragma once

# include "fcntl.h"
# include "unistd.h"
class ServerLog
{
    private:
        int _stdout_fd;
        int _stderr_fd;
    public:
        ServerLog();
        ~ServerLog();
		ServerLog(const ServerLog& copy);
		ServerLog& operator=(const ServerLog& src);

		int getStdOutFd() const;
		int getStdErrFd() const;

        void createLog();
};
