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
#ifndef WAYPOINT
#define WAYPOINT

#include <vector>

class Waypoint{
    private:                                       
        
    public:
        int                     m_id;
        std::vector<Waypoint*>  m_connected_waypoints;
        float                   m_position[2]; // [0] for x, [1] for y

                Waypoint(int p_id, float p_position_x, float p_position_y);
                ~Waypoint();
        void    connect(Waypoint *w2);
        void    disconnect(Waypoint *w2);
        bool    isConnected(Waypoint *w2);
};

#endif