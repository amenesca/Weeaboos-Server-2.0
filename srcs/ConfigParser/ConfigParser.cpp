/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:21 by amenesca          #+#    #+#             */
/*   Updated: 2024/04/19 13:25:57 by femarque         ###   ########.fr       */
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

std::vector<std::string> ConfigParser::split(const std::string& input) const
{
	std::vector<std::string> tokens;
	std::istringstream iss(input);
	std::string token;

	while (iss >> token)
	{
		if (token.find('#') != std::string::npos)
		{
			break;
		}
		tokens.push_back(token);
	}

	if (tokens.size() <= 1)
	{
		throw InvalidArguments();
	}
    return tokens;
}

std::vector<int> ConfigParser::StrVecToIntVecPort(const std::vector<std::string>& strVec) const
{
	std::vector<int> intVec;
	bool firstLoop = true;

    for (std::vector<std::string>::const_iterator it = strVec.begin(); it != strVec.end(); ++it)
	{
		if (firstLoop == true)
		{
			firstLoop = false;
			continue;
		}
		
        char* endPtr;
        long intValue = std::strtol((*it).c_str(), &endPtr, 10);

        if (*endPtr != '\0' || endPtr == (*it).c_str()) {
            throw InvalidArguments();
        }
		else if (intValue < 0 || intValue > 65535)
		{
            throw InvalidArguments();
        }
		else
		{
            intVec.push_back(static_cast<int>(intValue));
        }
    }

    return intVec;
}

int	ConfigParser::atoi(const std::string line) const
{
	int		signal;
	int		nbr;
	size_t	i;

	signal = 1;
	i = 0;
	while ((i < line.size()) && (std::isspace(line[i])))
		i++;
	if ((line[i] == '+') || (line[i] == '-'))
	{
		if (line[i] == '-')
			signal = -1;
		i++;
	}
	nbr = 0;
	while ((i < line.size()) && std::isdigit(line[i]))
	{
		nbr = (10 * nbr) + (line[i] - '0');
		i++;
	}
	return (nbr * signal);
}

bool	ConfigParser::isHostValid(std::string & _parameter) const
{
	int					nbr;
	unsigned short int	i;
	unsigned short int	j;

	i = 0;
	if ((_parameter.size() > 15) || (_parameter.size() < 7))
	{
		throw InvalidArguments();
	}
	while (i < _parameter.size())
	{
		j = 0;
		while (j < 3 && ((i + j) < _parameter.size()) && \
								(_parameter[(i + j)] != '.'))
		{
			if (!std::isdigit(_parameter[(i + j)]))
			{
				throw InvalidArguments();
			}
			j++;
		}
		if ((_parameter[(i + j)] != '.') && ((i + j) < _parameter.size()))
		{
			throw InvalidArguments();
		
		}
		nbr = atoi(_parameter.substr(i, j));
		if ((nbr > 255) || (nbr < 0))
		{
			throw InvalidArguments();
		}
		i += (j + 1);
	}
	return (true);
}

in_addr_t	ConfigParser::treatHost(std::string buff) const
{
	if (buff == "localhost")
	{
		buff = "127.0.0.1";	
	}
	if (isHostValid(buff))
	{
		return inet_addr(buff.c_str());
	}
	throw InvalidArguments();
	return (INADDR_NONE);
}

int	ConfigParser::treatMaxBodySize(const std::string& strMaxBodySize) const
{
	char* endPtr;
    int intValue = std::strtol(strMaxBodySize.c_str(), &endPtr, 10);

	if (*endPtr != '\0' || endPtr == strMaxBodySize.c_str())
	{
		throw InvalidArguments();
	}
	else if (intValue <= 0)
	{
		throw InvalidArguments();
	}
	
	return (intValue);
}

bool ConfigParser::treatAutoIndex(const std::string& _argument) const
{
	if (_argument == "on")
	{
		return true;
	}
	else if (_argument == "off")
	{
		return false;
	}
	else
	{
		throw InvalidArguments();
	}
	return false;
}

void ConfigParser::treatLocation(VirtualServer* currentServer, std::string locationPath)
{
	std::string buff;
	Location locationInstance;
	locationInstance.setPath(locationPath);
	bool rightBrace = false;

	while (std::getline(this->_configFileStream >> std::ws, buff))
	{
		if (!buff.compare(0,12,"cgi_extension") && \
		buff.find("}") == std::string::npos)
		{
			locationInstance.setCgiExtension(split(buff)[1]);
			continue;
		}
		else if (!buff.compare(0,6,"upload") &&
		buff.find("}") == std::string::npos)
		{
			locationInstance.setUpload(split(buff)[1]);
			continue;
		}
		else if (!buff.compare(0,5,"index") && \
		buff.find("}") == std::string::npos)
		{
			locationInstance.setIndex(split(buff));
			continue;
		}
		else if (!buff.compare(0,9,"autoindex") && \
		buff.find("}") == std::string::npos)
		{
			if (!buff.compare(0,5,"index"))
			{
				continue;
			}
			locationInstance.setAutoIndex(treatAutoIndex(split(buff)[1]));
			continue;
		}
		else if (!buff.compare(0,13,"allow_methods") && \
		buff.find("}") == std::string::npos)
		{
			locationInstance.setMethods(split(buff));
			continue;
		}
		else if (!buff.compare(0, 6,"return") && \
		buff.find("}") == std::string::npos)
		{
			locationInstance.setReturn(split(buff)[1]);
		}
		else if (buff.find("}", 0) != std::string::npos)
		{
			rightBrace = true;
			break;
		}
		else
		{
			throw InvalidSyntax();
		}
	}
	if (rightBrace == false)
	{
			throw InvalidSyntax();
	}
	currentServer->getLocationsAddress()->push_back(locationInstance);
}

void ConfigParser::configVServer(VirtualServer* currentServer)
{
	std::string buff;
	bool rightBrace = false;

	while (std::getline(this->_configFileStream >> std::ws, buff))
	{
		if (buff[0] == '#')
			continue;
		
		if (buff == "}")
		{
			rightBrace = true;
			break ;
		}
		else if ((!buff.compare(0, 6, "listen") || \
		!buff.compare(0,4,"port")) && \
		buff.find("}") == std::string::npos)
		{
			currentServer->setPort(StrVecToIntVecPort(split(buff)));
			continue;
		}
		else if (!buff.compare(0, 4,"root") && \
		buff.find("}") == std::string::npos)
		{
			currentServer->setRoot(split(buff)[1]);
			continue;
		}
		else if (!buff.compare(0, 11,"server_name") && \
		buff.find("}") == std::string::npos)
		{
			currentServer->setServerName(split(buff));
			continue;
		}
		else if (!buff.compare(0, 4,"host") && \
		buff.find("}") == std::string::npos)
		{
			currentServer->setHost(treatHost(split(buff)[1]));
			currentServer->setStrHost(split(buff)[1]);
			continue;
		}
		else if (!buff.compare(0,5,"index") && \
		buff.find("}") == std::string::npos)
		{
			currentServer->setIndex(split(buff));
			continue;
		}
		else if (!buff.compare(0,13,"max_body_size") && \
		buff.find("}") == std::string::npos)
		{
			currentServer->setMaxBodySize(treatMaxBodySize(split(buff)[1]));
			continue;
		}
		else if (!buff.compare(0,10,"error_page") && \
			buff.find("}") == std::string::npos)
		{
			currentServer->setErrorPage(split(buff));
			continue;
		}
		else if (!buff.compare(0, 8,"location") && \
		buff.find("}") == std::string::npos) 
		{
			if (buff.find("{", buff.size() - 1) == std::string::npos || split(buff).size() != 3)
			{
				throw InvalidSyntax();
			}
			treatLocation(currentServer, split(buff)[1]);
			continue;
		}
		else
		{
			throw InvalidSyntax();
		}
	}
	if (rightBrace == false) {
			throw InvalidSyntax();
	}
}

void ConfigParser::createVServers(void)
{
	std::string buff;
	size_t i = 0;

	while (std::getline(this->_configFileStream >> std::ws, buff))
	{
		if (buff[0] == '#')
			continue;
		
		std::istringstream iss(buff);
		std::string token;
		iss >> token;

		if (token == "server" && \
		buff.find("{", buff.size() - 1) != std::string::npos && \
		split(buff).size() == 2)
		{
			VirtualServer serverInstance;

			if (i == 0)
				serverInstance.setServerDefault(true);
			
			this->_vServers.push_back(serverInstance);
			this->configVServer(&_vServers[i]);
			
			serverInstance.~VirtualServer();
			token.clear();
			buff.clear();
			iss.clear();
			i++;
		} 
		else
		{
			throw InvalidSyntax();	
		}
		
	}
}

int ConfigParser::initConfig(void)
{
	this->openConfig();
	this->createVServers();
	return 0;
}

const char *ConfigParser::OpenError::what() const throw() {
	return ("webserver: error: can't open configuration file.");
}

const char *ConfigParser::InvalidSyntax::what() const throw() {
	return ("webserver: error: invalid syntax.");
}

const char *ConfigParser::InvalidArguments::what() const throw() {
	return ("webserver: error: invalid arguments.");
}