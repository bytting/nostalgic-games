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

#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <cmath>
#include <SFML/Graphics.hpp>

namespace ng
{

#ifndef PI
#define PI      3.1415927f
#endif

class VisualObject
{
public:
	
	VisualObject() : Flags(0),
		Velocity(sf::Vector2f(0.f, 0.f)),
		LastPosition(sf::Vector2f(0.f, 0.f)),
		mDestination(sf::Vector2f(0.f, 0.f)),
		NE(0.f), NW(0.f), SW(0.f), SE(0.f), mMass(10.f)
	{}
	
	const sf::Vector2f& Position() const { return mSprite.GetPosition(); }
	
	void SetPosition(const sf::Vector2f& newPos) { mSprite.SetPosition(newPos); }

	void SetPosition(float newX, float newY) { mSprite.SetPosition(newX, newY); }

	void SetX(float newX) { mSprite.SetPosition(newX, mSprite.GetPosition().y); }

	void SetY(float newY) { mSprite.SetPosition(mSprite.GetPosition().x, newY); }

	float Left() const { return mSprite.GetPosition().x; }

	float Top() const { return mSprite.GetPosition().y; }

	float Right() const { return mSprite.GetPosition().x + mSprite.GetTextureRect().Width; }

	float Bottom() const { return mSprite.GetPosition().y + mSprite.GetTextureRect().Height; }

	sf::FloatRect Rect() const;

	sf::Vector2f Size() const { return sf::Vector2f((float)mSprite.GetTextureRect().Width, (float)mSprite.GetTextureRect().Height); }

	float Width() const { return (float)mSprite.GetTextureRect().Width; }

	float Height() const { return (float)mSprite.GetTextureRect().Height; }
	
	float CenterX() const { return mSprite.GetPosition().x + mSprite.GetTextureRect().Width / 2; }

	float CenterY() const { return mSprite.GetPosition().y + mSprite.GetTextureRect().Height / 2; }

	float LastPositionCenterX() const { return LastPosition.x + mSprite.GetTextureRect().Width / 2; }

	float LastPositionCenterY() const { return LastPosition.y + mSprite.GetTextureRect().Height / 2; }

	void SetCorners(float ne, float nw, float sw, float se);

	void SetMass(float newMass) { mMass = newMass; }

	const sf::Sprite& Sprite() const { return mSprite; }

	void SetSpriteColor(const sf::Color& color) { mSprite.SetColor(color); }

	void SetSpriteTexture(const sf::Texture& tex) { mSprite.SetTexture(tex); }

	void SetImageRect(const sf::IntRect& rectangle) { mSprite.SetTextureRect(rectangle); }

	bool IsOriginInsideBrick(const VisualObject& object);

	void ProcessOriginImpactWithBrick(const VisualObject& object);

	bool IsBallOverlappingBall(const VisualObject& object);

	void ProcessBallImpactWithBall(VisualObject& object);

	const sf::Vector2f& Destination() const { return mDestination; }
	
	void SetDestination(const sf::Vector2f& newDestination) { mDestination = newDestination; }

	void SetDestinationX(float newX) { mDestination.x = newX; }

	void SetDestinationY(float newY) { mDestination.y = newY; }

	void UpdateDestination(float frametime);

	void MoveToDestination();

	unsigned long Flags;
	sf::Vector2f Velocity, LastPosition;

private:

	sf::Sprite mSprite;
	sf::Vector2f mDestination;
	float NE, NW, SW, SE, mMass;
};

} // namespace ng

#endif
