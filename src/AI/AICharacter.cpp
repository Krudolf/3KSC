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

#include "../include/AI/AICharacter.hpp"
#include "../include/AI/AINode.hpp"
#include "../include/managers/PhysicsManager.hpp"
#include "../include/managers/InputManager.hpp"
#include "../include/entities/Arena.hpp"
#include "../include/AI/Pathfinding.hpp"
#include "../include/entities/characters/Character.hpp"

#include <fstream>
#include <sstream>

AICharacter::AICharacter(Character* p_player){
    m_nodes              = nullptr;                      //Different in each character
    m_physicsManager    = &PhysicsManager::instance();
    m_inputManager      = &InputManager::instance();
    m_arena             = Arena::getInstance();
    m_pathfinding       = &Pathfinding::instance();

    m_player            = p_player;
    m_index             = m_player->getIndex();
    m_HP                = m_player->getHP();
    m_MP                = m_player->getMP();
    m_x                 = m_player->getX();
    m_y                 = m_player->getY();
    m_position          = b2Vec2(m_x, m_y);
    m_time              = 0.0f;
    m_starting_time     = m_inputManager->getMasterClock();
}

AICharacter::~AICharacter(){
    delete m_nodes;
    m_nodes = nullptr;
}

//Reads a file and returns its content as an string
std::string AICharacter::readFile(const char* p_url){
    //Open file
    std::ifstream t_file;
    t_file.open(p_url);

    //Read file contents
    std::stringstream t_strStream;
    t_strStream << t_file.rdbuf();

    //Return file contents as a string
    return t_strStream.str();
}
