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

#include <SFML/Graphics.hpp>
#include "BreakoutReadyState.h"

using namespace sf;

BreakoutReadyState BreakoutReadyState::gBreakoutReadyState;

void BreakoutReadyState::Enter(RenderWindow& window)
{
    Font& font = fontManager["arial"];

    text.setFont(font);
    text.setCharacterSize(20);
    text.setColor(Color(255, 128, 0));
}

void BreakoutReadyState::Exit(RenderWindow& window)
{
}

void BreakoutReadyState::Pause()
{
}

void BreakoutReadyState::Resume()
{
}

void BreakoutReadyState::KeyPressed(Event& event)
{
    switch(event.key.code)
    {
    case Keyboard::Space:
        PopState();
        break;
    }
}

void BreakoutReadyState::KeyReleased(Event& event)
{
}

bool BreakoutReadyState::FrameRender(RenderWindow& window, float frametime)
{
    text.setString("Press SPACE when you are ready for next level");
    text.setCharacterSize(20);
    text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, 120);
    window.draw(text);

    return false;
}
