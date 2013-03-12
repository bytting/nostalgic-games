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

#include <exception>
#include <string>
#include <fstream>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "Logger.h"
#include "../managers/FontManager.h"
#include "../managers/TextureManager.h"
#include "../managers/StateManager.h"
#include "../states/intro/IntroState.h"

const std::string logfile = "nostalgic-games-debug.log";
const unsigned int maxFramerate = 120;

int main(int argc, char *argv[])
{
    int return_value = 0;

    std::time_t current_time;
    std::time(&current_time);

#ifdef NG_DEBUG

    std::ofstream log(logfile.c_str());
    if(!log)
    {
        std::cerr << "Failed to open log: " << logfile << std::endl;
        return 1;
    }

    std::streambuf* cerr_rdbuf = std::cerr.rdbuf();
    std::cerr.rdbuf(log.rdbuf());

#endif

	try
	{
	    NG_LOG("Debugging " << argv[0] << " at " << std::asctime(std::localtime(&current_time)));				

	    ng::FontManager::ResourceFile = "resources.xml";		
		ng::TextureManager::ResourceFile = "resources.xml";		

        ng::StateManager state_manager;        

		state_manager.Initialize("Nostalgic Games", sf::VideoMode(1024, 768), false, maxFramerate);
		state_manager.Start(IntroState::Instance());

        NG_LOG(argv[0] << " finished");
	}
	catch(std::exception& ex)
	{
        NG_LOG(ex.what());
		return_value = 1;
	}

#ifdef NG_DEBUG

    std::cerr.rdbuf(cerr_rdbuf);
	log.close();

#endif

	return return_value;
}
