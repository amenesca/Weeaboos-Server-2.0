/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:32:06 by amenesca          #+#    #+#             */
/*   Updated: 2024/04/03 15:16:54 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <poll.h>
# include <fcntl.h>
# include <iostream>
# include "../Client/Client.hpp"
# include "../Response/Response.hpp"

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
		WebServer(const std::vector<VirtualServer>& vServers, int nbrServers);
		WebServer(const WebServer& copy);
		WebServer& operator=(const WebServer& src);

		std::vector<VirtualServer> getVServers(void) const;
		std::vector<pollfd> getPollFds(void) const;
		int		getNbrServers(void) const;
		std::vector<Client>	getClients(void) const;

		void	setVServers(const std::vector<VirtualServer>& vServers);
		void	setPollFds(const std::vector<pollfd>& pollFds);
		void	setNbrServers(const int& nbrServers);
		void	setClients(const std::vector<Client>& clients);

		void	addNewSocketToPoll(int socketFd);
		void	addVServersSockToPoll(void);
		void	addNewClientSockToPoll(int clientSocket);
		void	closeConnection(int index);
		void	closeAllConnections(void);
		
		void		StartServer(void);
		bool		isPollError(int i);
		bool		verifyPollStatus(void);
		void		openNewConnection(int i);
		short int	treatRequest(int clientPos, int pollPos);
		short int	treatResponse(int clientPos, int pollPos);

		
};