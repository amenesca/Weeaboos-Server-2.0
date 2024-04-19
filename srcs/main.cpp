/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:13 by amenesca          #+#    #+#             */
/*   Updated: 2024/04/19 13:29:05 by femarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <csignal>
#include "./ConfigParser/ConfigParser.hpp"
#include "./WebServer/WebServer.hpp"

int main(int argc, char **argv)
{
	ConfigParser				settingVServers;
	std::vector<VirtualServer>	vServers;

	try {
		if (argc == 2)
		{
			settingVServers.setConfigFilePath(argv[1]);
			settingVServers.initConfig();
			vServers = settingVServers.getVServers();
		} else if (argc == 1) {
			settingVServers.setConfigFilePath("./conf/default.conf");
			settingVServers.initConfig();
			vServers = settingVServers.getVServers();
		} else {
			std::cerr << "webserver: error: Invalid arguments." << std::endl;
			return -1;	
		}

		for (size_t i = 0; i < vServers.size(); i++)
		{
			vServers[i].initialize();
		}

		WebServer Server(vServers, vServers.size());
		Server.StartServer();
		
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	return 0;
}