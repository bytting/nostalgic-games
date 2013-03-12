/* 
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later version.
 
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/gpl.txt

Copyright (c) 2010 Dag Robøle
*/

/// @file Singleton.h Contains implementation of Singleton and SingletonClient

#ifndef SINGLETON_H
#define SINGLETON_H

#include <cassert>

namespace ng 
{

/** 
 * @class Singleton Singleton.h
 * @brief A Singleton class used to provide Singleton properties to instances of its template
 */
template <class T> 
class Singleton : private T
{
private:
	
	/**
	 * Singleton::Singleton
	 * @brief Default Singleton constructor
	 */
	Singleton() {}
	
public:
	
	/**
	 * Singleton::Instance
	 * @return Returns the singleton instance	
	 */
	static T& Instance()
	{   
		static T inst;
		return inst;
	}   
	
	/**
	 * Singleton::InstancePtr
	 * @return Returns a pointer to the singleton instance
	 */
	static T* InstancePtr()
	{   
		//T& inst = Instance();
		//return &inst;
		return &Instance();
	}   
};

/**
 * @class SingletonClient Singleton.h
 * @brief A class used to provide Singleton behaviour to its descendants
 */
class SingletonClient
{
private:
	
	/**
	 * SingletonClient::SingletonClient
	 * @param sc - The SingletonClient to copy
	 * @brief Copy constructor for the SingletonClient. 
	 * This constructor is private to prevent copying of SingletonClients and its descendants
	 */
	SingletonClient(const SingletonClient& sc) 
	{   
		assert(false);
	}   
	
	/**
	 * SingletonClient::operator =
	 * @param sc - The SingletonClient to assign
	 * @brief Assign operator for the SingletonClient. 
	 * This operator is private to prevent assignment of SingletonClients and its descendants
	 */
	SingletonClient& operator = (const SingletonClient& sc) 
	{   
		assert(false);
		return *this;
	}   
	
protected:
	
	/**
	 * SingletonClient::SingletonClient	
	 * @brief Default SingletonClient constructor
	 */
	SingletonClient() {}
};

} // namespace ng

#endif
