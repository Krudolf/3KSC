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

#ifndef PORTAL
#define PORTAL

class PhysicsManager;
class HUDManager;
class Arena;

#include "../Entity.hpp"
#include "../characters/Character.hpp"

class Portal: public Entity {
public:
    Portal(float p_position[3]);
    ~Portal();
    void        onEnter(Character* p_character);
    void        onLeave(Character* p_character);
    void        update(float p_delta);
    void        use();
private:
    Arena*          m_arena;
    PhysicsManager* m_physicsManager;
    HUDManager*     m_HUDManager;

    float       m_charge;
    bool        m_using;
    int         m_charactersInPortal;
    Character*  m_players[2] = {NULL,NULL};
};

#endif