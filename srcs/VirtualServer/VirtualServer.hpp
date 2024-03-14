/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:16 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/14 17:00:33 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <arpa/inet.h>

# include "../Location/Location.hpp"

class VirtualServer
{
	private:
		bool						_serverDefault;
		std::vector<int>			_port;
		std::string					_root;
		std::vector<std::string>	_serverName;
		in_addr_t					_host;
		std::vector<std::string>	_index;
		int							_maxBodySize;
		std::vector<std::string>	_errorPage;
		std::vector<Location>		_locations;

	public:
		VirtualServer(void);
		~VirtualServer(void);
		VirtualServer(const VirtualServer& copy);
		VirtualServer& operator=(const VirtualServer& src);

		bool 						getServerDefault(void) const;
		std::vector<int>			getPort(void) const;
		std::string					getRoot(void) const;
		std::vector<std::string>	getServerName(void) const;
		in_addr_t					getHost(void) const;
		std::vector<std::string>	getIndex(void) const;
		int							getMaxBodySize(void) const;
		std::vector<std::string>	getErrorPage(void) const;
		std::vector<Location>		getLocations(void) const;

		void	setServerDefault(const bool& serverDefault);
		void	setPort(const std::vector<int>& port);
		void	setRoot(const std::string& root);
		void	setServerName(const std::vector<std::string>& serverName);
		void	setHost(const in_addr_t& host);
		void	setIndex(const std::vector<std::string>& index);
		void	setMaxBodySize(const int& maxBodySize);
		void	setErrorPage(const std::vector<std::string>& errorPage);
		void	setLocations(const std::vector<Location>& locations);
};