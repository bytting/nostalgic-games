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

#ifndef BREAKOUTSTATE_H
#define BREAKOUTSTATE_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "../../managers/FontManager.h"
#include "../../managers/TextureManager.h"
#include "../../managers/StringManager.h"
#include "../../managers/State.h"
#include "../../ng/VisualObject.h"

const int BRICK_WIDTH = 42;
const int BRICK_HEIGHT = 18;

const int BRICK_ROWS_MAX = 8;
const int BRICK_COLS_MAX = 18;

const int BALL_LARGE_LEFT = 42;
const int BALL_LARGE_TOP = 144;
const int BALL_LARGE_WIDTH = 16;
const int BALL_LARGE_HEIGHT = 16;
const float BALL_LARGE_MASS = 30.f;

const int BALL_MEDIUM_LEFT = 58;
const int BALL_MEDIUM_TOP = 144;
const int BALL_MEDIUM_WIDTH = 14;
const int BALL_MEDIUM_HEIGHT = 14;
const float BALL_MEDIUM_MASS = 10.f;

const int BALL_SMALL_LEFT = 72;
const int BALL_SMALL_TOP = 144;
const int BALL_SMALL_WIDTH = 8;
const int BALL_SMALL_HEIGHT = 8;
const float BALL_SMALL_MASS = 3.3f;

const int BALLS_MAX = 30;

const int PAD_LARGE_LEFT = 0;
const int PAD_LARGE_TOP = 162;
const int PAD_LARGE_WIDTH = 58;
const int PAD_LARGE_HEIGHT = 12;

const int PAD_MEDIUM_LEFT = 0;
const int PAD_MEDIUM_TOP = 170;
const int PAD_MEDIUM_WIDTH = 50;
const int PAD_MEDIUM_HEIGHT = 12;

const int PADS_MAX = 2;

const float ENGLISH_RATE = 10.f;

const unsigned long OBJECT_ACTIVE       = (1 << 0);
const unsigned long OBJECT_POWERUP      = (1 << 1);
const unsigned long OBJECT_SOLID        = (1 << 2);

class BreakoutState : public ng::State
{
public:

    void Enter(sf::RenderWindow& window);
    void Exit(sf::RenderWindow& window);

    void Pause();
    void Resume();

    void KeyPressed(sf::Event& event);
    void KeyReleased(sf::Event& event);
    bool FrameRender(sf::RenderWindow& window, float frametime);

    static BreakoutState* Instance() { return &gBreakoutState; }

protected:

    BreakoutState() { }

private:

    static BreakoutState gBreakoutState;

    ng::FontManager fontManager;
    ng::TextureManager textureManager;
    ng::StringManager stringManager;

    sf::Text text;
    sf::Sprite spriteBackground;

    std::vector<std::vector<ng::VisualObject> > bricks;
    std::vector<ng::VisualObject> pads;
    std::vector<ng::VisualObject> balls;

    float padSpeed, ballSpeed;
    int score, remainingBricks;
    unsigned int currentLevel;

    void InitializeBreakout(sf::RenderWindow& window);
};

#endif
