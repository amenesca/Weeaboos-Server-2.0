/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:32:06 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/21 17:26:46 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <poll.h>
# include <fcntl.h>
# include <iostream>
# include "../Client/Client.hpp"

class WebServer
{
	private:
		std::vector<VirtualServer>	_vServers; // serve para tacar na poll
		std::vector<Client>			_Clients; // tem que receber as configs
		std::vector<pollfd>			_pollFds; // primeiras posições sempre do server
		int							_nbrServers;

	public:
		WebServer(void);
		~WebServer(void);
		WebServer(const std::vector<VirtualServer>& vServers, const int& nbrServers);
		WebServer(const WebServer& copy);
		WebServer& operator=(const WebServer& src);

		std::vector<VirtualServer> getVServers(void) const;
		std::vector<pollfd> getPollFds(void) const;
		int		getNbrServers(void) const;

		void	setVServers(const std::vector<VirtualServer>& vServers);
		void	setPollFds(const std::vector<pollfd>& pollFds);
		void	setNbrServers(const int& nbrServers);

		void	addNewSocketToPoll(int socketFd);
		void	addVServersSockToPoll(void);
		void	addNewClientSockToPoll(int clientSocket);
		void	closeConnection(int index);
		void	closeAllConnections(void);
		
		void	StartServer(void);
		bool	isPollError(int i);
		bool	verifyPollStatus(void);
		void	receiveData(int i);
		void	openNewConnection(int i);
		void	treatRequest(int clientPos, int pollPos);
		
};