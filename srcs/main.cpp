/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:48:13 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/14 17:13:13 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ConfigParser/ConfigParser.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	ConfigParser	settingVServers;
	std::string		configFilePath;
	
	try {
		if (argc == 2)
		{
			configFilePath = argv[1];
			settingVServers.setConfigFilePath(configFilePath);
			settingVServers.initConfig();
		} else if (argc == 1) {
			settingVServers.setConfigFilePath("./conf/default.conf");
			settingVServers.initConfig();
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