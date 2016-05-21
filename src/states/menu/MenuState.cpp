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
#include "MenuState.h"
#include "../breakout/BreakoutState.h"
#include "../breakout/BreakoutAboutState.h"

using namespace sf;

MenuState MenuState::gMenuState;

void MenuState::Enter(RenderWindow& window)
{
    Font& font = mFontManager["arial"];

    mText.setFont(font);
    mText.setCharacterSize(20);
    mText.setColor(Color(255, 128, 0));
}

void MenuState::Exit(RenderWindow& window)
{
}

void MenuState::Pause()
{
}

void MenuState::Resume()
{
}

void MenuState::KeyPressed(Event& event)
{
    switch(event.key.code)
    {
    case Keyboard::F1:
        PushState(BreakoutState::Instance());
        break;
    case Keyboard::F2:
        PushState(BreakoutAboutState::Instance());
        break;
    case Keyboard::Escape:
        PopState();
        break;
    }
}

void MenuState::KeyReleased(Event& event)
{
}

bool MenuState::FrameRender(RenderWindow& window, float frametime)
{
    mText.setString("Menu");
    mText.setCharacterSize(40);
    mText.setPosition(window.getSize().x / 2 - mText.getGlobalBounds().width / 2, 50);
    window.draw(mText);

    mText.setString("F1 - Breakout | F2 - About Breakout");
    mText.setCharacterSize(20);
    mText.setPosition(window.getSize().x / 2 - mText.getGlobalBounds().width / 2, 120);
    window.draw(mText);

    return false;
}
