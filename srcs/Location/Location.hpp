/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 15:16:31 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/15 17:20:50 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <string>
# include <vector>

class Location
{
	private:
		std::string 				_path;
		std::vector<std::string>	_methods;
		std::vector<std::string>	_index;
		bool						_autoIndex;
		std::string 				_cgi_extension;
		std::string 				_upload;
		std::string 				_return;

	public:
		Location(void);
		~Location(void);
		Location(const Location& copy);
		Location& operator=(const Location& src);

		std::string					getPath(void) const;
		std::vector<std::string>	getMethods(void) const;
		std::vector<std::string>	getIndex(void) const;
		bool						getAutoIndex(void) const;
		std::string					getCgiExtension(void) const;
		std::string					getUpload(void) const;
		std::string					getReturn(void) const;

		void	setPath(const std::string& path);
		void	setMethods(const std::vector<std::string>& methods);
		void	setIndex(const std::vector<std::string>& index);
		void	setAutoIndex(const bool& autoIndex);
		void	setCgiExtension(const std::string& cgiExtension);
		void	setUpload(const std::string& upload);
		void	setReturn(const std::string& Return);
};
