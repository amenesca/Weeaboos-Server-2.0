/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:32:06 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/20 11:37:28 by amenesca         ###   ########.fr       */
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
};