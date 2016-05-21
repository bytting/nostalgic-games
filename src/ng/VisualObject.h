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
#define PI      3.14159265358979323846
#endif

class VisualObject
{
public:

    VisualObject() :
        Flags(0),
        Velocity(sf::Vector2f(0.f, 0.f)),
        LastPosition(sf::Vector2f(0.f, 0.f)),
        mDestination(sf::Vector2f(0.f, 0.f)),
        NE(0.f), NW(0.f), SW(0.f), SE(0.f), mMass(10.f)
    {}

    float Width() const { return (float)mSprite.getTextureRect().width; }

    float Height() const { return (float)mSprite.getTextureRect().height; }

    sf::Vector2f Size() const { return sf::Vector2f(Width(), Height()); }

    float Left() const { return mSprite.getPosition().x; }

    float Top() const { return mSprite.getPosition().y; }

    float Right() const { return Left() + Width(); }

    float Bottom() const { return Top() + Height(); }

    sf::FloatRect Rect() const { return sf::FloatRect(Left(), Top(), Right(), Bottom()); }

    void SetX(float newX) { mSprite.setPosition(newX, Top()); }

    void SetY(float newY) { mSprite.setPosition(Left(), newY); }

    const sf::Vector2f& Position() const { return mSprite.getPosition(); }

    void SetPosition(const sf::Vector2f& newPos) { mSprite.setPosition(newPos); }

    void SetPosition(float newX, float newY) { mSprite.setPosition(newX, newY); }

    float CenterX() const { return Left() + Width() / 2.f; }

    float CenterY() const { return Top() + Height() / 2.f; }

    float LastPositionCenterX() const { return LastPosition.x + Width() / 2.f; }

    float LastPositionCenterY() const { return LastPosition.y + Height() / 2.f; }

    void SetCornerRadians(float ne, float nw, float sw, float se);

    void SetMass(float newMass) { mMass = newMass; }

    const sf::Sprite& Sprite() const { return mSprite; }

    void SetSpriteColor(const sf::Color& color) { mSprite.setColor(color); }

    void SetSpriteTexture(const sf::Texture& tex) { mSprite.setTexture(tex); }

    void SetImageRect(const sf::IntRect& rectangle) { mSprite.setTextureRect(rectangle); }

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
