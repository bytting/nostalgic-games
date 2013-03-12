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

/// @file StringManager.h Contains declaration of StringManager

#ifndef STRINGMANAGER_H
#define STRINGMANAGER_H

#include <string>
#include <map>
#include "../ng/Singleton.h"

namespace ng
{

class StringManager
{
public:

	StringManager() : m(Singleton<StringData>::InstancePtr()) {}

	std::string& operator [] (const std::string& name)
	{
	    return m->StringMap[name];
	}

private:

    struct StringData : public SingletonClient
    {
        std::map<std::string, std::string> StringMap;
    };

    StringData* m;
};

} // namespace ng

#endif
