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
#ifndef PATHFINDING
#define PATHFINDING

#include <vector>
#include <Box2D/Common/b2Math.h>

class Waypoint;
class PhysicsManager;

class Pathfinding{
    private:   
        PhysicsManager*         m_physicsManager;                                
    public:
        std::vector<Waypoint*>  m_waypoints;

        static Pathfinding& instance();

                  Pathfinding();
                  ~Pathfinding();
        void      init(std::vector<Waypoint*> p_waypoints);
        b2Vec2    findPath(b2Vec2 p_p1, b2Vec2 p_p2, Waypoint* p_lastWaypoint);
        b2Vec2    getClosestWaypoint(b2Vec2 p_position, bool p_direction);
        void      addWaypoint(float p_wayData[3]);
        void      connectWaypoints(float p_id1, float p_id2);
};

#endif