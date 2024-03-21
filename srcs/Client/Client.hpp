/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:30:01 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/21 14:54:32 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <string>
# include <cstring>
# include <arpa/inet.h>

class Client
{
    private:
        int					_clientSocket;
        socklen_t			_client_addr_len;
        struct sockaddr_in	_client_addr;
        std::string			_requestBuffer;
        ssize_t				_bytesRead;
	
    public:
        Client();
        ~Client();
		Client(const Client& copy);
		Client& operator=(const Client& copy);


        int			getClientSocket(void) const;
		socklen_t	getClientAddrLen(void) const;
		sockaddr_in	getClientAddr(void) const;
		std::string	getRequestBuffer(void) const;
        ssize_t		getBytesRead(void) const;
        sockaddr_in	*getClientAddrPointer(void);
        socklen_t	*getClientAddrLenPointer(void);

        void        setClientSocket(const int& clientSocket);
        void        setClientAddrLen(const socklen_t& _client_addr_len);
        void        setBytesRead(const ssize_t& bytesRead);
        void        setRequestBuffer(const std::string& _requestBuffer);
		void 		setClientAddr(const struct sockaddr_in& client_addr);

};