/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:32:06 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/21 14:28:59 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <poll.h>
# include <fcntl.h>
# include <iostream>
# include "../VirtualServer/VirtualServer.hpp"

class WebServer
{
	private:
		std::vector<VirtualServer>	_vServers;
		std::vector<pollfd>			_pollFds;
		

	public:
		WebServer(void);
		~WebServer(void);
		WebServer(const std::vector<VirtualServer>& vServers);
		WebServer(const WebServer& copy);
		WebServer& operator=(const WebServer& src);

		std::vector<VirtualServer> getVServers(void) const;
		std::vector<pollfd> getPollFds(void) const;

		void	setVServers(const std::vector<VirtualServer>& vServers);
		void	setPollFds(const std::vector<pollfd>& pollFds);

		void	addNewSocketToPoll(int socketFd);
		void	addVServersSockToPoll(void);
		void	addNewClientSockToPoll(int clientSocket);
		void	closeConnection(int index);
		void	closeAllConnections(void);
		
		void	StartServer(void);
		bool	isPollError(int i);
		bool	verifyPollStatus(void);
};