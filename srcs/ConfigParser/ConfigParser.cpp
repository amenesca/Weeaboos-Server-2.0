/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:21 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/14 17:28:58 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser(void) :
	_configFilePath(""),
	_vServers(),
	_configFileStream()
{

}

ConfigParser::ConfigParser(const std::string& configFilePath) :
	_configFilePath(configFilePath),
	_vServers(),
	_configFileStream()
{

}

ConfigParser::~ConfigParser(void)
{
	_configFilePath.clear();
	_vServers.clear();
	if (_configFileStream.is_open())
	{
		_configFileStream.clear();
		_configFileStream.close();
	}
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

void ConfigParser::openConfig() {
	
	_configFileStream.open(_configFilePath.c_str(), std::ios::in);

	if (!_configFileStream.is_open()) {
		throw OpenError();
	}
}

int ConfigParser::initConfig(void)
{
	return 0;
}

const char *ConfigParser::OpenError::what() const throw() {
	return ("WebServer: error: can't open configuration file.");
}

const char *ConfigParser::InvalidSyntax::what() const throw() {
	return ("WebServer: error: invalid syntax.");
}