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

#include <stdexcept>
#include <fstream>
#include "FontManager.h"
#include "../ng/Logger.h"
#include "../xml/rapidxml.hpp"

using namespace sf;

namespace ng
{

std::string FontManager::ResourceFile;

FontManager::FontManager()	
{
}

Font* FontManager::Load(const std::string& name)
{
	using namespace std;
	using namespace rapidxml;

	Font* font = NULL;
	ifstream::pos_type size;
	char* memblock = NULL;

	ifstream file(ResourceFile.c_str(), ios::in | ios::binary | ios::ate);
	if (!file.is_open())
		throw std::runtime_error("FontManager failed to open file: " + ResourceFile);

	size = file.tellg();
	memblock = new char[size + (ifstream::pos_type)1];
	file.seekg(0, ios::beg);
	file.read(memblock, size);
	file.close();
	memblock[size] = '\0';

	xml_document<> doc;
	doc.parse<0>(memblock);

	xml_node<>* root = doc.first_node("resources");

	for (xml_node<>* fontSection = root->first_node("fonts"); fontSection && !font; fontSection = fontSection->next_sibling("fonts"))
	{
		for (xml_node<>* fontElement = fontSection->first_node("font"); fontElement && !font; fontElement = fontElement->next_sibling("font"))
		{
			for(xml_attribute<> *attr = fontElement->first_attribute(); attr; attr = attr->next_attribute())
			{
				if(attr->value() == name)
				{
					font = new Font();
					if(!font->LoadFromFile(attr->next_attribute()->value()))
					{
						delete font;
						doc.clear();
						delete [] memblock;
						throw std::runtime_error("FontManager failed to load resource: " + name);
					}
					break;
				}
			}
		}
	}

	doc.clear();
	delete [] memblock;

	return font;
}

} // namespace ng
