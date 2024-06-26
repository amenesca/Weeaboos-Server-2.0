/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:32:06 by amenesca          #+#    #+#             */
/*   Updated: 2024/04/19 13:31:00 by femarque         ###   ########.fr       */
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
		std::vector<VirtualServer>	_vServers;
		std::vector<Client>			_Clients;
		std::vector<pollfd>			_pollFds;
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
		static void	signal_handler(int signal);
		
		void		StartServer(void);
		bool		isPollError(int i);
		bool		verifyPollStatus(void);
		void		openNewConnection(int i);
		short int	treatRequest(int clientPos, int pollPos);
		short int	treatResponse(int clientPos, int pollPos);
		static bool					_serverRunning;
		
};