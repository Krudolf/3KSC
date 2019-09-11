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

#include "../include/menu_screens/TutorialScreen.hpp"

//Instance initialization
TutorialScreen* TutorialScreen::m_instance = nullptr;

//Returns the only instance of this class
TutorialScreen* TutorialScreen::instance(){
    if (!m_instance){
        m_instance = new TutorialScreen(MenuState::getInstance());
    }

    return m_instance;
}

//Constructor
TutorialScreen::TutorialScreen(MenuState* p_menu)
    : MenuScreen(p_menu){
    createFromFile("assets/UI/menu_screens/Tutorial.cgs");

    hideUnselected();
}

//Destructor
TutorialScreen::~TutorialScreen(){}

//Hides all elements except the selected one
void TutorialScreen::hideUnselected(){
    for (CESceneSprite* t_sprite : m_sprites){
        t_sprite->setVisible(t_sprite == m_selectedNode->element);
    }
}






/* ****************************** ACTIONS ****************************** */
void TutorialScreen::left(){    
    if (m_selectedNode && m_selectedNode->left){
        m_selectedNode = m_selectedNode->left;
        hideUnselected();
    }
}

void TutorialScreen::right(){    
    if (m_selectedNode && m_selectedNode->right){
        m_selectedNode = m_selectedNode->right;
        hideUnselected();
    }
}