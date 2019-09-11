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

#include "../include/menu_screens/BattleSettingsScreen.hpp"
#include "../include/Game.hpp"

//Instance initialization
BattleSettingsScreen* BattleSettingsScreen::m_instance = nullptr;

//Returns the only instance of this class
BattleSettingsScreen* BattleSettingsScreen::instance(){
    if (!m_instance){
        m_instance = new BattleSettingsScreen(MenuState::getInstance());
    }

    return m_instance;
}

//Constructor
BattleSettingsScreen::BattleSettingsScreen(MenuState* p_menu)
    : MenuScreen(p_menu){    
    m_game              = Game::getInstance();

    createFromFile("assets/UI/menu_screens/BattleSettings.cgs");
    init();
}

//Destructor
BattleSettingsScreen::~BattleSettingsScreen(){}

//Initializes battle settings
void BattleSettingsScreen::init(){
    m_battleSettings[0] = 2;
    m_battleSettings[1] = 3;

    m_sprites[2]->setTexture(1);
    m_sprites[3]->setTexture(2);

    m_game->setBattleSettings(0, 2);
    m_game->setBattleSettings(1, 3);
}

void BattleSettingsScreen::update(){
    MenuScreen::update();
    m_selector->setAbsolutePosition(-483, m_selectedNode->element->getPosition().y, 0);
}










/* ****************************** ACTIONS ****************************** */
void BattleSettingsScreen::left(){
    int t_index = getSelectedIndex();

    if (m_battleSettings[t_index] > 1){
        int t_value = --m_battleSettings[t_index];
        m_sprites[t_index + 2]->setTexture(t_value - 1);
        m_game->setBattleSettings(t_index, t_value);
    }
}

void BattleSettingsScreen::right(){
    int t_index = getSelectedIndex();

    if (m_battleSettings[t_index] < 6){
        int t_value = ++m_battleSettings[t_index]; 
        m_sprites[t_index + 2]->setTexture(t_value - 1);
        m_game->setBattleSettings(t_index, t_value);
    }
}

void BattleSettingsScreen::select(){
    save();
}