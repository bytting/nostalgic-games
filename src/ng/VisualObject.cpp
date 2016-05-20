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

#include "VisualObject.h"

using namespace sf;

namespace ng {

FloatRect VisualObject::Rect() const
{
    return FloatRect(mSprite.getPosition().x, mSprite.getPosition().y, mSprite.getPosition().x + mSprite.getGlobalBounds().width, mSprite.getPosition().y + mSprite.getGlobalBounds().height);
}

	void VisualObject::SetCorners(float ne, float nw, float sw, float se)
	{
		NE = ne;
		NW = nw;
		SW = sw;
		SE = se;
	}

	bool VisualObject::IsOriginInsideBrick(const VisualObject& object)
	{
		return (CenterX() <= object.Right() && CenterX() >= object.Left() && CenterY() <= object.Bottom() && CenterY() >= object.Top());
	}

	void VisualObject::ProcessOriginImpactWithBrick(const VisualObject& object)
	{
		float a = std::atan2(LastPositionCenterY() - object.CenterY(), LastPositionCenterX() - object.CenterX());

		if((a <= 0 && a > object.NE) || (a <= object.NW && a > -PI) || (a >= 0 && a < object.SE) || (a >= object.SW && a < PI))
			Velocity.x *= -1;
		else if((a <= object.NE && a > object.NW) || (a >= object.SE && a < object.SW))
			Velocity.y *= -1;
	}

	bool VisualObject::IsBallOverlappingBall(const VisualObject& object)
	{
		// FIXME: Optimizations
        float dx = object.mSprite.getPosition().x - mSprite.getPosition().x;
        float dy = object.mSprite.getPosition().y - mSprite.getPosition().y;
        return std::sqrt(dx * dx + dy * dy) <= mSprite.getGlobalBounds().width / 2 + object.mSprite.getGlobalBounds().width / 2;
	}

	void VisualObject::ProcessBallImpactWithBall(VisualObject& object)
	{
		// FIXME: Optimizations
        float dx = object.mSprite.getPosition().x - mSprite.getPosition().x;
        float dy = object.mSprite.getPosition().y - mSprite.getPosition().y;
		float d = std::sqrt(dx * dx + dy * dy);
        float impact_distance = mSprite.getGlobalBounds().width / 2 + object.mSprite.getGlobalBounds().width / 2;

		// calculate the velocity in the direction of (dx,dy)
		float vp_a = (Velocity.x * dx + Velocity.y * dy) / d;
		float vp_b = (object.Velocity.x * dx + object.Velocity.y * dy) / d;

		// collision happened dt before d
		float dt = (impact_distance - d) / (vp_a - vp_b);

		// move balls backward one step
        mSprite.move(-Velocity.x * dt, -Velocity.y * dt);
        object.mSprite.move(-object.Velocity.x * dt, -object.Velocity.y * dt);

        dx = object.mSprite.getPosition().x - mSprite.getPosition().x;
        dy = object.mSprite.getPosition().y - mSprite.getPosition().y;
		d = sqrt(dx * dx + dy * dy);

		// calculate the components of velocity
		float ap = dx / d;
		float an = dy / d;

		vp_a = Velocity.x * ap + Velocity.y * an;
		float vn_a = Velocity.x * an - Velocity.y * ap;

		vp_b = object.Velocity.x * ap + object.Velocity.y * an;
		float vn_b = object.Velocity.x * an - object.Velocity.y * ap;

		// calculate new velocity for the balls
		Velocity.x = vp_b * ap + vn_a * an;
		Velocity.y = vp_b * an - vn_a * ap;
		object.Velocity.x = vp_a * ap + vn_b * an;
		object.Velocity.y = vp_a * an - vn_b * ap;

		// move the balls forward one step
        mSprite.move(Velocity.x * dt, Velocity.y * dt);
        object.mSprite.move(object.Velocity.x * dt, object.Velocity.y * dt);
	}

	void VisualObject::UpdateDestination(float frametime)
	{
        mDestination = mSprite.getPosition() + Velocity * frametime;
	}

	void VisualObject::MoveToDestination()
	{
        LastPosition = mSprite.getPosition();
        mSprite.setPosition(mDestination);
	}

} // namespace ng
