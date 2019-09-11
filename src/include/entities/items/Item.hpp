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

#ifndef ITEM
#define ITEM

class PhysicsManager;
class Arena;

#include "../Entity.hpp"
#include <SFML/System/Clock.hpp>

class Item: public Entity {
public:
    Item(float p_position[3], const char* p_modelURL);
    virtual ~Item();
    virtual void    setOwner(int p_owner);
    virtual void    use();
    bool            update();
    
protected:
    PhysicsManager*     m_physicsManager;
    Arena*              m_arena;

    int                 m_owner;
    float               m_duration;
    float               m_durationGround;
    static const char*  m_modelURLs[4];

    float               m_dieTime;
};

#endif