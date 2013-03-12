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

#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <SFML/Graphics.hpp>
#include "../../managers/State.h"
#include "../../managers/FontManager.h"

class MenuState : public ng::State
{
public:

	void Enter(sf::RenderWindow& window);
	void Exit(sf::RenderWindow& window);

	void Pause();
	void Resume();

	void KeyPressed(sf::Event& event);
	void KeyReleased(sf::Event& event);
	bool FrameRender(sf::RenderWindow& window, float frametime);

	static MenuState* Instance() { return &gMenuState; }

protected:

	MenuState() { }

private:

	static MenuState gMenuState;

	ng::FontManager mFontManager;

	sf::Text mText;
};

#endif
