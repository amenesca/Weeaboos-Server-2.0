/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenesca <amenesca@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 15:16:28 by amenesca          #+#    #+#             */
/*   Updated: 2024/03/14 16:48:56 by amenesca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() :
	_path(""),
	_methods(),
	_index(),
	_autoIndex(false)
{
	
}

Location::~Location()
{
	this->_path.clear();
	this->_methods.clear();
	this->_index.clear();
	this->_autoIndex = 0;
}

Location& Location::operator=(const Location& src)
{
	if (this != &src)
	{
		this->_path = src.getPath();
		this->_methods = src.getMethods();
		this->_index = src.getIndex();
		this->_autoIndex = src.getAutoIndex();
	}
	return *this;
}

Location::Location(const Location& copy)
{
	*this = copy;
	return;
}

std::string	Location::getPath(void) const
{
	return this->_path;
}

std::vector<std::string> Location::getMethods(void) const
{
	return this->_methods;
}

std::vector<std::string> Location::getIndex(void) const
{
	return this->_index;
}

bool Location::getAutoIndex(void) const
{
	return this->_autoIndex;
}

void Location::setPath(const std::string& path)
{
	this->_path = path;
}

void Location::setMethods(const std::vector<std::string>& methods)
{
	this->_methods = methods;
}

void Location::setIndex(const std::vector<std::string>& index)
{
	this->_index = index;
}

void Location::setAutoIndex(const bool& autoIndex)
{
	this->_autoIndex = autoIndex;
}