/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:13 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/18 14:04:33 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ConfigParser/ConfigParser.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	ConfigParser				settingVServers;
	std::string					configFilePath;
	std::vector<VirtualServer>	vServers;
	
	try {
		if (argc == 2)
		{
			configFilePath = argv[1];
			settingVServers.setConfigFilePath(configFilePath);
			settingVServers.initConfig();
			vServers = settingVServers.getVServers(); // VServers Copiados para a main
			// Agora devo começar a conexão de sockets
		} else if (argc == 1) {
			settingVServers.setConfigFilePath("./conf/default.conf");
			settingVServers.initConfig();
			vServers = settingVServers.getVServers(); // VServers Copiados para a main
			// Agora devo começar a conexão de sockets
		} else {
			std::cerr << "webserver: error: Invalid arguments." << std::endl;
			return -1;	
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	return 0;
}