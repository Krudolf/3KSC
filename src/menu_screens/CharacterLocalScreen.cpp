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

#include "../include/menu_screens/CharacterLocalScreen.hpp"

#include "../include/managers/InputManager.hpp"
#include "../include/managers/SoundManager.hpp"
#include "../include/Game.hpp"

#include <cstdlib>
#include <ctime>

#define k_characters    3   //Number of different characters (RANDOM included)
#define k_players       2   //Max number of players

//Instance initialization
CharacterLocalScreen* CharacterLocalScreen::m_instance = nullptr;

//Returns the only instance of this class
CharacterLocalScreen* CharacterLocalScreen::instance(){
    if (!m_instance){
        m_instance = new CharacterLocalScreen(MenuState::getInstance());
    }

    return m_instance;
}

//Constructor
CharacterLocalScreen::CharacterLocalScreen(MenuState* p_menu)
    : MenuScreen(p_menu){
    m_game              = Game::getInstance();

    createFromFile("assets/UI/menu_screens/CharacterSelect.cgs");    
    m_joystickConnected = m_inputManager->isConnected(0);
    init();
}

//Destructor
CharacterLocalScreen::~CharacterLocalScreen(){}

//Simulates initial character choosing
void CharacterLocalScreen::init(){
    //P1 (Random, playable)
    m_nowChoosing   = 0;
    m_chosen[0]     = 2;
    m_NPC[0]        = false;
    m_game->setPlayerNPC(0, false);
    chooseCharacter();

    //P2 (Random, NPC only if no joysticks connected)
    m_nowChoosing   = 1;
    m_chosen[1]     = 2;
    m_NPC[1]        = false;

    if (!m_joystickConnected){
        m_NPC[1] = true;
        colorizeBackground();
        colorizeBorder();
        m_game->setPlayerNPC(1, true);
    }
    else{
        m_game->setPlayerNPC(1, false);
    }

    chooseCharacter();

    //Return control to user, focused in P1
    m_nowChoosing = 0;
}

//Shows the face and name of the chosen character
void CharacterLocalScreen::chooseCharacter(){
    //If NPC, select the texture index + 4
    int t_NPCExtra = (m_NPC[m_nowChoosing] ? 4 : 0);

    m_sprites[m_nowChoosing]->setTexture(m_chosen[m_nowChoosing] + 1 + t_NPCExtra);  

    //Don't send 'random' to Game. Send an actual random character
    int t_character = (m_chosen[m_nowChoosing] == 2 ? getRandomCharacter() : m_chosen[m_nowChoosing]);
    m_game->setChosenPlayer(m_nowChoosing, t_character);    
}

// Returns a random character index
// Implementation based in std::rand() reference page
// (http://en.cppreference.com/w/cpp/numeric/random/rand)
int CharacterLocalScreen::getRandomCharacter(){
    //Get different random seed with current time
    std::srand(std::time(nullptr));

    // Get random in interval [0, k_characters - 2] (Last index means random itself)
    return std::rand()%(k_characters - 1);
}

//Puts a rectangle around the player choosing character
void CharacterLocalScreen::colorizeBackground(){
    //Go foward or backwards in textures vector
    int t_factor = (m_NPC[m_nowChoosing] ? 1 : -1);

    m_sprites[m_nowChoosing]->setTexture(m_sprites[m_nowChoosing]->getTexture() + (4 * t_factor));
}

//Changes the color of the focus rectangle
void CharacterLocalScreen::colorizeBorder(){
    m_sprites[m_nowChoosing + 2]->setTexture((int) m_NPC[m_nowChoosing]);
}

//Updates the state of the screen
void CharacterLocalScreen::update(){
    //Put rectangle around the player who's choosing a character
    m_sprites[2]->setVisible(m_nowChoosing == 0);
    m_sprites[3]->setVisible(m_nowChoosing == 1);
    
    //Call to default update for navigation purposes
    MenuScreen::update();
}










/* ****************************** ACTIONS ****************************** */
void CharacterLocalScreen::left(){
    if (m_chosen[m_nowChoosing] > 0)    { m_chosen[m_nowChoosing]--;                    }
    else                                { m_chosen[m_nowChoosing] = k_characters - 1;   }
    
    chooseCharacter();
    
}

void CharacterLocalScreen::right(){
    if (m_chosen[m_nowChoosing] < k_characters - 1) { m_chosen[m_nowChoosing]++;    }
    else                                            { m_chosen[m_nowChoosing] = 0;  }
    
    chooseCharacter();
}

void CharacterLocalScreen::select(){
    m_soundManager->modifyParameter("menu_sounds", 0.9f, "option");
    m_soundManager->playSound("menu_sounds");
    save();
}

void CharacterLocalScreen::back(){
    if (m_nowChoosing == 1) { m_nowChoosing = 0;    }
    else                    { MenuScreen::back();   }
}

void CharacterLocalScreen::save(){
    if (m_nowChoosing == 0) { m_nowChoosing = 1;    }
    else{
        MenuScreen::save();  
    }
}

void CharacterLocalScreen::toggleNPC(){
    //If there's no joystick, P2 must be AI
    if (m_nowChoosing == 0 || m_joystickConnected){
        m_NPC[m_nowChoosing] = !m_NPC[m_nowChoosing];

        colorizeBackground();
        colorizeBorder();

        m_game->setPlayerNPC(m_nowChoosing, m_NPC[m_nowChoosing]);
    }
}