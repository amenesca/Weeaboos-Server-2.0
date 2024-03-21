/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:30:01 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/21 17:05:25 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../VirtualServer/VirtualServer.hpp"
# include "../RequestParser/RequestParser.hpp"

class Client
{
    private:
        int					_clientSocket;
        struct sockaddr_in	_client_addr;
        socklen_t			_client_addr_len;
        std::string			_requestBuffer;
        ssize_t				_bytesRead;

		VirtualServer		_serverConfigs;
	
    public:
        Client();
        ~Client();
		Client(const Client& copy);
		Client& operator=(const Client& copy);


        int				getClientSocket(void) const;
		socklen_t		getClientAddrLen(void) const;
		sockaddr_in		getClientAddr(void) const;
		std::string		getRequestBuffer(void) const;
        ssize_t			getBytesRead(void) const;
		VirtualServer	getServerConfigs(void) const;
		
        sockaddr_in	*getClientAddrPointer(void);
        socklen_t	*getClientAddrLenPointer(void);

        void        setClientSocket(const int& clientSocket);
        void        setClientAddrLen(const socklen_t& _client_addr_len);
        void        setBytesRead(const ssize_t& bytesRead);
        void        setRequestBuffer(const std::string& _requestBuffer);
		void 		setClientAddr(const struct sockaddr_in& client_addr);
		void		setServerConfigs(const VirtualServer& serverConfigs);

};