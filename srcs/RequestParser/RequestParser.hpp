/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: femarque <femarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 17:03:49 by amenesca          #+#    #+#             */
/*   Updated: 2024/04/19 13:30:19 by femarque         ###   ########.fr       */
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
# include <cstdio>

class RequestParser
{
	private:
		std::string 						_requestMethod;
		std::string 						_uri;
		std::string 						_httpVersion;
		std::string 						_portNumber;
		std::string 						_queryString;
		std::string							_newRequestBody;
		std::map<std::string, std::string>	_requestHeaders;

		std::string							_stringBuffer;
		int									_contentLenght;

		int _validateUri();
	public:
		RequestParser();
		~RequestParser();
		RequestParser(std::string request);
		RequestParser(const RequestParser& copy);
		RequestParser& operator=(const RequestParser& copy);

		void	parse(std::string request);
		void 	parse(void);
		void	validateRequestLine();
		void	startBody(ssize_t bytesReceived, ssize_t bodyPosition,const u_int8_t *buffer);
		void 	appendBody(const u_int8_t* buffer, ssize_t bytesReceived);
	
		std::string 						getMethod() const ;
		std::string 						getUri() const;
		std::string							getHttpVersion() const;
		std::map<std::string, std::string>	getHeaders() const;
		std::string							getPortNumber() const;
		std::string							getQueryString();
		std::string							getStringBuffer() const;
		int									getContentLenght() const;
		std::string							getNewRequestBody() const;

		void setContentLenght(const int& contentLenght);

		class invalidMethod : public std::exception {
		public:
			virtual const char* what() const throw();
		};

		class invalidHttpVersion : public std::exception {
		public:
			virtual const char* what() const throw();
        };
};