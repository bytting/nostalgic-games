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

/// @file StateManager.h Contains declaration of the StateManager class

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "../ng/Singleton.h"

namespace ng {

class State;

class StateManager
{
public:

	/**
	 * StateManager::StateManager
	 * @brief Default StateManager constructor
	 */
	StateManager();

	/**
	 * StateManager::~StateManager
	 * @brief StateManager destructor
	 */
	~StateManager();

	/**
	 * StateManager::initialize
	 * @param title - The window caption. Not visible in full screen mode
	 * @param mode - The SFML::VideoMode to use
	 * @param full_screen - Full screen switch
	 * @brief This function initializes the StateManager and creates the SFML::RenderWindow target
	 */
	void Initialize(const std::string& title, const sf::VideoMode& mode, bool full_screen, unsigned int maxFramerate = 0);

	/**
	 * StateManager::start
	 * @param state - The state to start with
	 * @brief This function will start the state manager using the provided state, and begin the event loop
	 */
	void Start(State* state);

	/**
	 * StateManager::change_state
	 * @param state - The state to change to
	 * @brief This function will remove the current state from the state queue and add the provided state at the top
	 */
	void ChangeState(State* state);

	/**
	 * StateManager::push_state
	 * @param state - The state to add
	 * @brief This function will add the provided state to the top of the state queue
	 */
	void PushState(State* state);

	/**
	 * StateManager::pop_state
	 * @brief This function will pop the top state off the state queue
	 */
	void PopState();

private:

	struct StateData : private SingletonClient
	{
		std::vector<State*> States;
		sf::RenderWindow* RenderWindow;
		unsigned long WindowStyle;
		bool Running;
		std::string Title;
		sf::Event Event;
	};

	StateData* const m;

	/**
	 * StateManager::start_event_processor
	 * @brief This function will start the dispatching of system events
	 */
	void StartEventProcessor();
};

} // namespace ng

#endif
