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
#include "LoginState.h"
#include "../menu/MenuState.h"
#include "../../ng/Utils.h"

using namespace sf;

LoginState LoginState::gLoginState;

void LoginState::Enter(RenderWindow& window)
{
	Font& font = fontManager["arial"];

    text.setFont(font);
    text.setCharacterSize(20);
    text.setColor(Color(255, 128, 0));

	info = "Enter your name";
	username = "";
}

void LoginState::Exit(RenderWindow& window)
{
}

void LoginState::Pause()
{
}

void LoginState::Resume()
{
    info = "Enter your name";
    username = "";
}

void LoginState::KeyPressed(Event& event)
{
    switch(event.key.code)
	{
	case Keyboard::Return:
        if(Login())
            PushState(MenuState::Instance());
		break;

	case Keyboard::Escape:
		PopState();
		break;

    case Keyboard::BackSpace:
		if(username.length() > 0)
			username = username.substr(0, username.length() - 1);
		break;

    case Keyboard::Space:
		if(username.length() > 0 && username.length() < 13 && username[username.length() - 1] != ' ')
            username += " ";
		break;

	default:
        if(username.length() < 13 && event.text.unicode <= Keyboard::Z)
        {	
			// FIXME: Keyboard::Key enumeration is not frozen. Make a switch?
            if(Keyboard::isKeyPressed(Keyboard::RShift) || Keyboard::isKeyPressed(Keyboard::LShift))
                username += 'A' + event.text.unicode;
			else
                username += 'a' + event.text.unicode;
        }
        break;
	}
}

void LoginState::KeyReleased(Event& event)
{
}

bool LoginState::FrameRender(RenderWindow& window, float frametime)
{	
    text.setString("Login");
    text.setCharacterSize(40);
    text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, 50);
    window.draw(text);

    text.setString(info);
    text.setCharacterSize(20);
    text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, 100);
    window.draw(text);
	
    text.setString(username);
    text.setCharacterSize(20);
    text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, 140);
    window.draw(text);

	return false;
}

bool LoginState::Login()
{
    username = TrimString(username);

    if(username.length() < 3)
    {
        info = "Name is too short. Minimum 3 characters. Try again";
        return false;
    }

    stringManager["username"] = username;

    return true;
}

