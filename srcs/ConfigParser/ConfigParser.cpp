/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:21 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/14 16:56:14 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(void) :
	_configFilePath(""),
	_vServers()
{

}

ConfigParser::ConfigParser(const std::string& configFilePath) :
	_configFilePath(configFilePath),
	_vServers()
{

}

ConfigParser::~ConfigParser(void)
{
	_configFilePath.clear();
	_vServers.clear();
}

ConfigParser& ConfigParser::operator=(const ConfigParser& src)
{
	if (this != &src)
	{
		this->_configFilePath = src.getConfigFilePath();
		this->_vServers = src.getVServers();
	}
	return *this;
}

ConfigParser::ConfigParser(const ConfigParser& copy)
{
	*this = copy;
	return ;
}

std::string ConfigParser::getConfigFilePath(void) const
{
	return this->_configFilePath;
}

std::vector<VirtualServer> ConfigParser::getVServers(void) const
{
	return this->_vServers;
}

void ConfigParser::setConfigFilePath(const std::string& configFilePath)
{
	this->_configFilePath = configFilePath;
}

void ConfigParser::setVServers(const std::vector<VirtualServer>& vServers)
{
	this->_vServers = vServers;
}

