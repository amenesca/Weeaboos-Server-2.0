/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtualServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:16 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/21 16:04:51 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <unistd.h>
# include <cstdio>
# include <cstring>
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

		int		_fd_socket;
		int		_main_port;

		bool isDescriptorOpen(int fd) const;

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
		int							getFdSocket(void) const;
		int							getMainPort(void) const;
		std::vector<Location>*		getLocationsAddress(void);

		void	setServerDefault(const bool& serverDefault);
		void	setPort(const std::vector<int>& port);
		void	setRoot(const std::string& root);
		void	setServerName(const std::vector<std::string>& serverName);
		void	setHost(const in_addr_t& host);
		void	setIndex(const std::vector<std::string>& index);
		void	setMaxBodySize(const int& maxBodySize);
		void	setErrorPage(const std::vector<std::string>& errorPage);
		void	setLocations(const std::vector<Location>& locations);
		void	setFdSocket(const int& fd_socket);
		void	setMainPort(const int& main_port);

		void	initialize(void);
		int		acceptCon(void) const;
		void	closeCon(void);
		
		class SocketError : public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class BindError : public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class ListenError : public std::exception {
			public:
				virtual const char* what() const throw();
		};
};