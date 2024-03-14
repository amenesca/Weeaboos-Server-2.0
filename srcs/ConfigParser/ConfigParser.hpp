/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:23 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/14 17:00:55 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../VirtualServer/VirtualServer.hpp"

class ConfigParser
{
	private:
		std::string					_configFilePath;
		std::vector<VirtualServer>	_vServers;

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
		
};