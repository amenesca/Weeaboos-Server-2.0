/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 17:03:49 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/25 23:45:43 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <map>
# include <sstream>
# include <exception>
# include <cerrno>
# include <cstring>
# include <vector>
# include <cstdlib>

class RequestParser
{
	private:
		std::string 						_requestMethod;
		std::string 						_uri;
		std::string 						_httpVersion;
		std::string 						_portNumber;
		std::string 						_queryString;
		std::vector<std::string> 			_requestBody;
		std::string							_newRequestBody;
		std::map<std::string, std::string>	_requestHeaders;

		std::string							_stringBuffer; // inutilizado
		int									_contentLenght;

	// Private Methods
		int _validateUri();
	public:
	// Constructors
		RequestParser();
		~RequestParser();
		RequestParser(std::string request);
		RequestParser(const RequestParser& copy);
		RequestParser& operator=(const RequestParser& copy);

	// Class Methods
		void	parse(std::string request);
		void 	parse(void);
		void	validateRequestLine();
		void 	appendBody(const std::string& buffer);
	
	// Getters
		std::string 						getMethod() const ;
		std::string 						getUri() const;
		std::string							getHttpVersion() const;
		std::vector<std::string>			getRequestBody() const;
		std::map<std::string, std::string>	getHeaders() const;
		std::string							getPortNumber() const;
		std::string							getQueryString();
		std::string							getStringBuffer() const;
		int									getContentLenght() const;
		std::string							getNewRequestBody() const;

		void setContentLenght(const int& contentLenght);

	// Exceptions
		class invalidMethod : public std::exception {
		public:
			virtual const char* what() const throw();
		};

		class invalidHttpVersion : public std::exception {
		public:
			virtual const char* what() const throw();
        };
};