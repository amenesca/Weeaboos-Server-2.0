/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:23 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/15 16:31:29 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../VirtualServer/VirtualServer.hpp"
# include <fstream>
# include <sstream>
# include <cstdlib>

class ConfigParser
{
	private:
		std::string					_configFilePath;
		std::vector<VirtualServer>	_vServers;
		std::ifstream				_configFileStream;

	public:
		ConfigParser(void);
		ConfigParser(const std::string& configFilePath);
		~ConfigParser(void);
		ConfigParser(const ConfigParser& copy);
		ConfigParser& operator=(const ConfigParser& src);
	
		std::string					getConfigFilePath(void) const;
		std::vector<VirtualServer>	getVServers(void) const;

		void	setConfigFilePath(const std::string& configFilePath);
		void	setVServers(const std::vector<VirtualServer>& vServers);

		std::vector<std::string>	split(const std::string& input) const;
		std::vector<int>			StrVecToIntVecPort(const std::vector<std::string>& strVector) const;
		in_addr_t					treatHost(std::string buff) const;
		bool						isHostValid(std::string & _parameter) const;
		int							atoi(const std::string line) const;
		int							treatMaxBodySize(const std::string& strMaxBodySize) const;

		void	openConfig(void);
		int		initConfig(void);
		void	createVServers(void);
		void	configVServer(VirtualServer* vServer);
		

		class OpenError : public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class InvalidSyntax : public std::exception {
			public:
				virtual const char* what() const throw();
		};

		class InvalidArguments : public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

