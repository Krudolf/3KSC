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

#ifndef AI_CHARACTER
#define AI_CHARACTER

class PhysicsManager;
class Pathfinding;
class InputManager;
class Arena;
class Character;
class AINode;

#include <Box2D/Common/b2Math.h> 
#include <string>
#include <json.hpp>

class AICharacter{
    protected:
        AINode**        m_nodes;                // Root of player tree
        PhysicsManager* m_physicsManager;
        InputManager*   m_inputManager;
        Arena*          m_arena;
        Pathfinding*    m_pathfinding;

        Character*      m_player;               // Pointer to player
        int             m_index;                // Player index
        float           m_HP;                   // Player HP
        float           m_MP;                   // Player MP
        float           m_x;                    // Player X coordinate
        float           m_y;                    // Player Y coordinate
        b2Vec2          m_position;             // Vector containing player position
        float           m_distanceToEnemy;      // Determines if an enemy is close
        float           m_distanceToPortal;     // Determines if an enemy is close
        float           m_portalActive;         // Determines if portal is active
        float           m_distanceToItem;       // Determines if an item is close
        float           m_specialUpRange;       // Determines if an enemy is in range of special ability up
        float           m_specialSideRange;     // Determines if an enemy is in range of special ability side
        float           m_specialDownRange;
        float           m_time;                 // Time that has passed since the last change of action
        float           m_starting_time;        // Control time
        float           m_ultimate_ready;       // Determines if ultimate is ready
        
        std::string     readFile(const char* p_url);
    public:
        AICharacter(Character* p_player);
        ~AICharacter();
        virtual void update()       {}
        virtual void buildTree()    {}
};

#endif

