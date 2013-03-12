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

#include <cmath>
#include <fstream>
#include <sstream>
#include "BreakoutState.h"
#include "../../ng/Logger.h"
#include "../../ng/Utils.h"
#include "BreakoutPauseState.h"
#include "BreakoutReadyState.h"
#include "BreakoutStateLevels.h"

using namespace sf;

BreakoutState BreakoutState::gBreakoutState;

static float RandomFloatRange(float a, float b)
{
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}

void BreakoutState::Enter(RenderWindow& window)
{
    NG_LOG("Entering breakout state");

	currentLevel = score = 0;
	InitializeBreakout(window);

	window.ShowMouseCursor(false);
}

void BreakoutState::Exit(RenderWindow& window)
{
    NG_LOG("Exiting breakout state");

    window.ShowMouseCursor(true);
}

void BreakoutState::Pause()
{
}

void BreakoutState::Resume()
{
}

void BreakoutState::KeyPressed(Event& event)
{
	switch(event.Key.Code)
	{
	case Keyboard::Escape:
		PopState();
		break;
	case Keyboard::Pause:
		PushState(BreakoutPauseState::Instance());
		break;	
	default: break;
	}
}

void BreakoutState::KeyReleased(Event& event)
{
}

bool BreakoutState::FrameRender(RenderWindow& window, float frametime)
{	
	frametime = frametime / 1000;

	// Move main pad	
	pads[0].Velocity = Vector2f(0.f, 0.f);

	if(Keyboard::IsKeyPressed(Keyboard::Left))
		pads[0].Velocity.x -= padSpeed;

	if(Keyboard::IsKeyPressed(Keyboard::Right))
		pads[0].Velocity.x += padSpeed;

	pads[0].UpdateDestination(frametime);

	if(pads[0].Destination().x < 0.f)
		pads[0].SetDestinationX(0.f);

	if(pads[0].Destination().x > window.GetWidth() - pads[0].Width())
		pads[0].SetDestinationX(window.GetWidth() - pads[0].Width());

	pads[0].MoveToDestination();

	// Move bonus pad if active
	if(pads[1].Flags & OBJECT_ACTIVE)
	{
		pads[1].Velocity = Vector2f(0.f, 0.f);

		if(Keyboard::IsKeyPressed(Keyboard::A))
			pads[1].Velocity.x -= padSpeed;

		if(Keyboard::IsKeyPressed(Keyboard::D))
			pads[1].Velocity.x += padSpeed;

		pads[1].UpdateDestination(frametime);

		if(pads[1].Destination().x < 0.f)
			pads[1].SetDestinationX(0.f);

		if(pads[1].Destination().x > window.GetWidth() - pads[1].Width())
			pads[1].SetDestinationX(window.GetWidth() - pads[1].Width());

		pads[1].MoveToDestination();
	}

	// Move balls
	for(int b=0; b<BALLS_MAX; b++)
	{
		if(balls[b].Flags & OBJECT_ACTIVE)
		{
			// Prevent horizontal velocity vector
			if(std::abs(balls[b].Velocity.y) < 10.f)
				balls[b].Velocity.y += 2.f;

			// Noramlize ball speed
			float dist = std::sqrt(balls[b].Velocity.x * balls[b].Velocity.x + balls[b].Velocity.y * balls[b].Velocity.y);
			if(dist < ballSpeed - 20.f)
				balls[b].Velocity *= 1.f + frametime / 2;
			else if(dist > ballSpeed + 20.f)
				balls[b].Velocity *= 1.f - frametime / 2;

			// Process collision with window edges
			const Vector2f& pos = balls[b].Destination();
			if(pos.x <= 0 || pos.x >= window.GetWidth() - balls[b].Width())
			{
				balls[b].SetPosition(balls[b].LastPosition);
				balls[b].Velocity.x *= -1;
			}

			if(pos.y <= 0 || pos.y >= window.GetHeight() - balls[b].Height())
			{
				balls[b].SetPosition(balls[b].LastPosition);
				balls[b].Velocity.y *= -1;
			}			

			// FIXME: Ball may hit another ball after this and end up outside screen
			balls[b].UpdateDestination(frametime);

			// Process collision with pads
			for(int p=0; p<PADS_MAX; p++)
				if(pads[p].Flags & OBJECT_ACTIVE)
					if(balls[b].IsOriginInsideBrick(pads[p]))
					{
						balls[b].ProcessOriginImpactWithBrick(pads[p]);
						balls[b].Velocity.x += (balls[b].CenterX() - pads[p].CenterX()) * ENGLISH_RATE; // Add 'English'
						balls[b].UpdateDestination(frametime);
					}

			// Process collision with bricks
			for(int r=0; r<BRICK_ROWS_MAX; r++)
				for(int c=0; c<BRICK_COLS_MAX; c++)
					if(bricks[r][c].Flags & OBJECT_ACTIVE)
						if(balls[b].IsOriginInsideBrick(bricks[r][c]))
						{
							balls[b].ProcessOriginImpactWithBrick(bricks[r][c]);
							if(!(bricks[r][c].Flags & OBJECT_SOLID))
							{
								bricks[r][c].Flags &= ~OBJECT_ACTIVE;
								score++;
								remainingBricks--;
							}
							balls[b].UpdateDestination(frametime);
						}

			// Move the ball
			balls[b].MoveToDestination();

			// Process collision with other balls
			for(int c=0; c<BALLS_MAX; c++)
				if(balls[c].Flags & OBJECT_ACTIVE && b != c)
					if(balls[b].IsBallOverlappingBall(balls[c]))
					{
						balls[b].ProcessBallImpactWithBall(balls[c]);
						balls[b].UpdateDestination(frametime);
						balls[c].UpdateDestination(frametime);
					}
		}
	}	

    // Draw background
    window.Draw(spriteBackground);
	
	// Draw all active bricks
	for(int i=0; i<BRICK_ROWS_MAX; i++)
		for(int j=0; j<BRICK_COLS_MAX; j++)
			if(bricks[i][j].Flags & OBJECT_ACTIVE)
				window.Draw(bricks[i][j].Sprite());

	// Draw all active pads
	for(int i=0; i<PADS_MAX; i++)
		if(pads[i].Flags & OBJECT_ACTIVE)
			window.Draw(pads[i].Sprite());

	// Draw all active balls
	for(int i=0; i<BALLS_MAX; i++)
		if(balls[i].Flags & OBJECT_ACTIVE)
			window.Draw(balls[i].Sprite());

    // Print status
    text.SetString(stringManager["username"] + " | Score " + ConvertToString(score));
	text.SetPosition(window.GetWidth() - 100.f, float(window.GetHeight() - text.GetCharacterSize() - 4));
	window.Draw(text);

    // Print debug info
    #ifdef NG_DEBUG

	float framerate = 1.f / frametime;
	std::string debug_info = "FPS " + ConvertToString(framerate);
	debug_info.resize(9, ' ');
	text.SetString(debug_info + " | Bricks left " + ConvertToString(remainingBricks));
	text.SetPosition(10.f, float(window.GetHeight() - text.GetCharacterSize() - 4));
	window.Draw(text);

    #endif

    // Check if level is finished
    if(remainingBricks <= 0)
    {
        currentLevel++;
        if(currentLevel >= MAX_LEVELS)
        {
            PopState(); // Jump back to main menu for now...
        }
        else
        {
            // Advance to next level
            InitializeBreakout(window);
            PushState(BreakoutReadyState::Instance());
        }
    }

	return false;
}

void BreakoutState::InitializeBreakout(RenderWindow& window)
{
	padSpeed = 300.f;
	ballSpeed = 220.f;
	remainingBricks = 0;

    Font& font = fontManager["arial"];

	text.SetFont(font);
	text.SetCharacterSize(12);
	text.SetColor(Color(255, 128, 0));

    Texture& img_space01 = textureManager["breakout-space01"];

    spriteBackground.SetTexture(img_space01);
    spriteBackground.SetTextureRect(IntRect(0, 0, window.GetWidth(), window.GetHeight()));

    Texture& img_breakout = textureManager["breakout"];	

    float brick_NE = std::atan2((float)-BRICK_HEIGHT, (float)BRICK_WIDTH);
	float brick_NW = std::atan2((float)-BRICK_HEIGHT, (float)-BRICK_WIDTH);
	float brick_SW = std::atan2((float)BRICK_HEIGHT, (float)-BRICK_WIDTH);
	float brick_SE = std::atan2((float)BRICK_HEIGHT, (float)BRICK_WIDTH);

	float pad_NE = std::atan2((float)-PAD_LARGE_HEIGHT, (float)PAD_LARGE_WIDTH);
	float pad_NW = std::atan2((float)-PAD_LARGE_HEIGHT, (float)-PAD_LARGE_WIDTH);
	float pad_SW = std::atan2((float)PAD_LARGE_HEIGHT, (float)-PAD_LARGE_WIDTH);
	float pad_SE = std::atan2((float)PAD_LARGE_HEIGHT, (float)PAD_LARGE_WIDTH);

	bricks.resize(BRICK_ROWS_MAX);
	for(int i=0; i<BRICK_ROWS_MAX; i++)
	{
		for(int j=0; j<BRICK_COLS_MAX; j++)
		{
			bricks[i].resize(BRICK_COLS_MAX);
			bricks[i][j].Flags = Levels[i + currentLevel * BRICK_ROWS_MAX][j];
			bricks[i][j].SetCorners(brick_NE, brick_NW, brick_SW, brick_SE);
			bricks[i][j].SetSpriteTexture(img_breakout);
			if(bricks[i][j].Flags == 1)
            {
                bricks[i][j].SetImageRect(IntRect(0, BRICK_HEIGHT * i, BRICK_WIDTH, BRICK_HEIGHT));
                remainingBricks++;
			}
			else if(bricks[i][j].Flags == 3)
			{
                bricks[i][j].SetImageRect(IntRect(BRICK_WIDTH, BRICK_HEIGHT * i, BRICK_WIDTH, BRICK_HEIGHT));
                bricks[i][j].Flags |= OBJECT_POWERUP;
                remainingBricks++;
			}
			else if(bricks[i][j].Flags == 5)
			{
			    bricks[i][j].SetImageRect(IntRect(0, BRICK_HEIGHT * BRICK_ROWS_MAX, BRICK_WIDTH, BRICK_HEIGHT));
			    bricks[i][j].Flags |= OBJECT_SOLID;
			}
			else
			{
			    bricks[i][j].SetImageRect(IntRect(0, 0, BRICK_WIDTH, BRICK_HEIGHT));
			    bricks[i][j].Flags = 0;
			}
			bricks[i][j].SetPosition(8.f + (j * (BRICK_WIDTH + 14)), 8.f + (i * (BRICK_HEIGHT + 14)));
			bricks[i][j].Velocity = Vector2f(0.f, 0.f);
			bricks[i][j].SetDestination(bricks[i][j].Position());
			bricks[i][j].LastPosition = bricks[i][j].Position();
		}
	}

	pads.resize(PADS_MAX);
	for(int i=0; i<PADS_MAX; i++)
	{
		//pads[i].flags = i ? 0 : OBJECT_ACTIVE;
		pads[i].Flags = OBJECT_ACTIVE;
		pads[i].SetCorners(pad_NE, pad_NW, pad_SW, pad_SE);
		pads[i].SetSpriteTexture(img_breakout);
		pads[i].SetImageRect(IntRect(PAD_LARGE_LEFT, PAD_LARGE_TOP, PAD_LARGE_WIDTH, PAD_LARGE_HEIGHT));
		pads[i].SetPosition(window.GetWidth() / 2.f - PAD_LARGE_WIDTH / 2.f, window.GetHeight() - 50.f);
		pads[i].Velocity = Vector2f(0.f, 0.f);
		pads[i].SetDestination(pads[i].Position());
		pads[i].LastPosition = pads[i].Position();
	}
	pads[1].SetSpriteColor(Color(220, 220, 28));
	pads[1].SetX(window.GetWidth() / 4.f);

	balls.resize(BALLS_MAX);
	for(int i=0; i<BALLS_MAX; i++)
	{
		//balls[i].flags = i ? 0 : OBJECT_ACTIVE;
		balls[i].Flags = OBJECT_ACTIVE;
		balls[i].SetSpriteTexture(img_breakout);
		balls[i].SetImageRect(IntRect(BALL_MEDIUM_LEFT, BALL_MEDIUM_TOP, BALL_MEDIUM_WIDTH, BALL_MEDIUM_HEIGHT));
		balls[i].SetPosition(window.GetWidth() / 2.f - BALL_MEDIUM_WIDTH / 2.f, window.GetHeight() - 65.f);
		balls[i].Velocity = Vector2f(RandomFloatRange(-ballSpeed, ballSpeed), -ballSpeed);		
		balls[i].SetDestination(balls[i].Position());
		balls[i].LastPosition = balls[i].Position();
		balls[i].SetMass(BALL_MEDIUM_MASS);
	}
}
