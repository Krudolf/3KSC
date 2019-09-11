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

#include "../../include/entities/items/Item.hpp"
#include "../../include/managers/PhysicsManager.hpp"
#include "../../include/managers/InputManager.hpp"
#include "../../include/entities/Arena.hpp"

//Constructor
Item::Item(float p_position[3], const char* p_modelURL) : Entity(p_position, 0.5f, p_modelURL, 2){
    m_dieTime = m_inputManager->getMasterClock() + 15.0;
}

//Destructor
Item::~Item(){}

//Uses the item
void Item::use(){}

//Sets the owner of the item
void Item::setOwner(int p_owner){}

bool Item::update(){
    if(m_inputManager->getMasterClock() > m_dieTime){
        delete this;
        return true;
    }

    updatePosition();

    return false;
}