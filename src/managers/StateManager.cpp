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

#include "StateManager.h"
#include "State.h"

using namespace sf;

namespace ng
{

StateManager::StateManager() : m(Singleton<StateData>::InstancePtr())
{
}

StateManager::~StateManager()
{
    while (!m->States.empty())
    {
        m->States.back()->Exit(*m->RenderWindow);
        m->States.pop_back();
    }

    if(m->RenderWindow)
        delete m->RenderWindow;
    m->RenderWindow = 0;
}

void StateManager::Initialize(const std::string& title, const sf::VideoMode& mode, bool fullScreen, unsigned int maxFramerate)
{
    m->RenderWindow = 0;
    m->Running = false;
    m->Title = title;
    m->WindowStyle = fullScreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close;
    m->RenderWindow = new sf::RenderWindow(mode, m->Title, m->WindowStyle);
    //m->RenderWindow->enableVerticalSync(true);
    if(maxFramerate)
        m->RenderWindow->setFramerateLimit(maxFramerate);
}

void StateManager::Start(State* state)
{
    ChangeState(state);
    if(!m->Running)
        StartEventProcessor();
}

void StateManager::ChangeState(State* state)
{
    if (!m->States.empty())
    {
        m->States.back()->Exit(*m->RenderWindow);
        m->States.pop_back();
    }

    m->States.push_back(state);
    m->States.back()->Enter(*m->RenderWindow);
}

void StateManager::PushState(State* state)
{
    if (!m->States.empty())
        m->States.back()->Pause();

    m->States.push_back(state);
    m->States.back()->Enter(*m->RenderWindow);
}

void StateManager::PopState()
{
    if(!m->States.empty())
    {
        m->States.back()->Exit(*m->RenderWindow);
        m->States.pop_back();
    }

    if (!m->States.empty())
        m->States.back()->Resume();
}

void StateManager::StartEventProcessor()
{
    Clock clock;
    float elapsed;

    m->Running = true;
    clock.restart();

    while(m->Running)
    {
        elapsed = (float)clock.restart().asMilliseconds();

        while(m->RenderWindow->pollEvent(m->Event))
        {
            if (m->Event.type == Event::Closed)
            {
                m->Running = false;
            }
            else if(m->Event.type == Event::KeyPressed)
            {
                m->States.back()->KeyPressed(m->Event);
            }
            else if(m->Event.type == Event::KeyReleased)
            {
                m->States.back()->KeyReleased(m->Event);
            }
        }

        m->RenderWindow->clear();

        if(m->States.back()->FrameRender(*m->RenderWindow, elapsed))
            m->Running = false;

        m->RenderWindow->display();
    }

    m->RenderWindow->close();
}

} // namespace ng
