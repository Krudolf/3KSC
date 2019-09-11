/*
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
    along with this program.  If not, see http://www.gnu.org/licenses/.

    You can contact Chaotic Games at: chaoticgamesdev@gmail.com
*/

#include "../include/menu_screens/MapScreen.hpp"
#include "../include/managers/EngineManager.hpp"
#include "../include/managers/SoundManager.hpp"
#include "../include/Game.hpp"

//Instance initialization
MapScreen* MapScreen::m_instance = nullptr;

//Returns the only instance of this class
MapScreen* MapScreen::instance(){
    if (!m_instance){
        m_instance = new MapScreen(MenuState::getInstance());
    }

    return m_instance;
}

//Constructor
MapScreen::MapScreen(MenuState* p_menu)
    : MenuScreen(p_menu){
    m_game = Game::getInstance();
    m_engineManager = &EngineManager::instance();

    createFromFile("assets/UI/menu_screens/Map.cgs");    
}

//Destructor
MapScreen::~MapScreen(){}

void MapScreen::update(){
    MenuScreen::update();

    if(m_selector){
        m_selector->setAbsolutePosition(-384, m_selectedNode->element->getPosition().y, 0);
    }
}










/* ****************************** ACTIONS ****************************** */
void MapScreen::select(){
    this->save();
}

void MapScreen::save(){
    m_game->setMap(getSelectedIndex());
    m_game->nextState();
}