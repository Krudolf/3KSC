
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

#ifndef SNOWMAN
#define SNOWMAN

class Projectile;
// class EngineManager;
class PhysicsManager;
class Arena;

#include "Entity.hpp"

class Snowman: public Entity {
public:
    Snowman(float p_position[3], int p_owner, float p_damage, float p_knockPower);
    ~Snowman();
    bool        lockNLoad();
    void        updateBullet();

    Projectile* getBullet();
    bool        getBulletLaunched();

private:
    PhysicsManager* m_physicsManager;
    Arena*          m_arena;

    Projectile*     m_snowball;
    int             m_ammo;
    int             m_owner;
    float           m_target[3];            //Position of the target

    bool            m_bulletLaunched;

    float           m_damage;
    float           m_knockPower;

    float           m_launchOffset;
    float           m_launchTime;
};

#endif