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
#include "../include/AI/Waypoint.hpp"

// Creates waypoint with id of p_id, and places it at (x,y) in space (z is not needed)
Waypoint::Waypoint(int p_id, float p_position_x, float p_position_y){
    m_id = p_id;
    m_position[0] = p_position_x;
    m_position[1] = p_position_y;
}

Waypoint::~Waypoint(){}

// Connects waypoint 1 with waypoint 2, and waypoint 2 with waypoint 1
void Waypoint::connect(Waypoint *w2){
    this->m_connected_waypoints.push_back(w2); // Assign connected waypoint to this waypoint
    w2->m_connected_waypoints.push_back(this); // Assign connected waypoint to waypoint number 2
}

// Disconnects waypoint 1 from waypoint 2, and viceversa
void Waypoint::disconnect(Waypoint *w2){
    // Iterate through the list of connected waypoints of w1 to find an empty slot
    for(int i=0; i<this->m_connected_waypoints.size(); i++)
    {
        if(this->m_connected_waypoints.at(i)->m_id == w2->m_id){ // Slot is empty
            this->m_connected_waypoints.erase(this->m_connected_waypoints.begin()+i); // Assign connected waypoint
        }
    }

    // Iterate through the list of connected waypoints of w2 to find an empty slot
    for(int i=0; i<w2->m_connected_waypoints.size(); i++)
    {
        if(w2->m_connected_waypoints.at(i)->m_id == this->m_id){ // Slot is empty
            w2->m_connected_waypoints.erase(this->m_connected_waypoints.begin()+i); // Assign connected waypoint
        }
    }
}

// Returns true if waypoint 1 is connected to waypoint 2
bool Waypoint::isConnected(Waypoint *w2){
    for(int i=0; i<this->m_connected_waypoints.size(); i++){
        if(this->m_connected_waypoints.at(i)->m_id==w2->m_id){
            return true;
        }
    }
    return false;
}