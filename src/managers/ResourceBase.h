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

/// @file ResourceBase.h Contains implementation of ResourceBase

#ifndef RESOURCEBASE_H
#define RESOURCEBASE_H

#include <stdexcept>
#include <string>
#include <map>
#include "../ng/Singleton.h"

namespace ng
{

template<class T>
class ResourceBase
{

public:

    /**
	* ResourceBase::ResourceBase
	* @brief Default ResourceBase constructor
	*/
    ResourceBase() : m(Singleton<ResourceData<T> >::InstancePtr()) {}

    /**
	* ResourceBase::~ResourceBase
	* @brief ResourceBase destructor
	*/
	virtual ~ResourceBase()
	{
		ReleaseAll();
	}

    /**
	* ResourceBase::Insert
	* @param name - Name of the resource to insert
	* @param resource - Pointer to resource to insert
	*/
	void Insert(std::string& name, T* resource)
	{
        typename std::map<std::string, T*>::iterator i = m->ResourceMap.find(name);
        if(i == m->ResourceMap.end())
            m->ResourceMap[name] = resource;
    }

	/**
	* ResourceBase::operator []
	* @param name - Name of the resource to retrieve
	* @return Returns a reference to the requested resource
	*/
	T& operator [] (const std::string& name)
	{
		return *Get(name);
	}

	/**
	 * ResourceBase::operator []
	 * @param name - Name of the resource to retrieve
	 * @return Returns a const reference to the requested resource
	 */
	const T& operator [] (const std::string& name) const
	{
		return *Get(name);
	}

	/**
	 * ResourceBase::Get
	 * @param name - Name of the resource to retrieve
	 * @return Returns a pointer to the requested resource
	 */
	T* Get(const std::string& name)
	{
        typename std::map<std::string, T*>::iterator i = m->ResourceMap.find(name);
        if(i != m->ResourceMap.end())
            return i->second;

        T* r = Load(name);
        if(r)
            m->ResourceMap[name] = r;
        else
            throw std::runtime_error("Unable to load resource: " + name);

        return r;
    }

	/**
	 * ResourceBase::Release
	 * @param name - Name of the resource to release
	 * @brief This function will delete and remove the resource with the given name if it exists
	 */
	void Release(const std::string& name)
	{
        typename std::map<std::string, T*>::iterator i = m->ResourceMap.find(name);
        if(i != m->ResourceMap.end())
        {
            delete i->second;
            m->ResourceMap.erase(m->ResourceMap.find(name));
        }
    }

	/**
	 * ResourceBase::ReleaseAll
	 * @brief This function will delete and remove all remaining resources
	 */
	void ReleaseAll()
    {
        while(m->ResourceMap.begin() != m->ResourceMap.end())
        {
            delete m->ResourceMap.begin()->second;
            m->ResourceMap.erase(m->ResourceMap.begin());
        }
    }	

protected:

	/**
	 * ResourceBase::Load
	 * @param name - The name of the resource to load
	 * @return Returns a pointer to the loaded resource
	 * @brief This is a virtual function leaving the details of loading resources to descendants
	 */
	virtual T* Load(const std::string& name) = 0;	

private:

	template<class U> 
	struct ResourceData : private SingletonClient
	{
		std::map<std::string, U*> ResourceMap;
	};

	ResourceData<T>* const m;
};

} // namespace ng

#endif
