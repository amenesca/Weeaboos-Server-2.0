/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:30:01 by amenesca          #+#    #+#             */
/*   Updated: 2024/04/19 13:29:56 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../VirtualServer/VirtualServer.hpp"
# include "../RequestParser/RequestParser.hpp"
# include <ctime>

class Client
{
    private:
        int					_clientSocket;
        struct sockaddr_in	_client_addr;
        socklen_t			_client_addr_len;
        std::string			_requestBuffer;
        ssize_t				_bytesRead;
		bool				_requestRead;
		bool				_firstTimeRequest;

		VirtualServer		_serverConfigs;
		RequestParser		_requestParser;
		ssize_t				_totalBodyBytes;
		time_t				_start_time;

		int					_pollPos;
		
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
		RequestParser	getRequest(void) const;
		bool			getRequestRead(void) const;
		bool			getFirstTimeRequest(void) const;
		ssize_t			getTotalBodyBytes(void) const;
		const time_t&	getStartTime(void) const;
		int				getPollPos(void) const;
		
        sockaddr_in	*getClientAddrPointer(void);
        socklen_t	*getClientAddrLenPointer(void);

        void        setClientSocket(const int& clientSocket);
        void        setClientAddrLen(const socklen_t& _client_addr_len);
        void        setBytesRead(const ssize_t& bytesRead);
        void        setRequestBuffer(const std::string& _requestBuffer);
		void 		setClientAddr(const struct sockaddr_in& client_addr);
		void		setServerConfigs(const VirtualServer& serverConfigs);
		void		setRequest(const RequestParser& request);
		void		setRequestRead(const bool& requestRead);
		void		setFirstTimeRequest(const bool& firstTimeRequest);
		void		setStart_time(time_t start_time);
		void		setPollPos(int z);

		short int	receiveRequest(int client);
		std::string u_int8_to_string(const uint8_t* data, size_t size);
		int countBytesUntilCRLF(const uint8_t* data, int dataSize) const;

};