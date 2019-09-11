/********************************************************************************
*********************************************************************************
	3KSC: A fighting game
    Copyright (C) 2018  Chaotic Games

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You can contact Chaotic Games at: chaoticgamesdev@gmail.com
*********************************************************************************
*********************************************************************************/

#ifndef PROJECTILE
#define PROJECTILE

#include "Entity.hpp"

class Projectile: public Entity {
public:
    Projectile(float p_position[3], float p_target[3], int p_rotation, int p_owner, int p_damage, float p_knockbackPower, int p_type);
    ~Projectile();
    bool    hit();
    bool    update(bool p_shouldHit);
private:
    static const char*  m_modelURLs[3];
    int     m_owner;
    int     m_type;
    float   m_target[3];
    int     m_damage;
    float   m_velocity;
    float   m_distanceLeft;
    float   m_knockbackPower;
    int     m_knockSide;


    float   m_step[2];
    void    calculateSteps();
};

#endif