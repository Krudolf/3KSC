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

#include "../include/managers/HUDManager.hpp"
#include "../include/Game.hpp"
#include "../include/managers/EngineManager.hpp"
#include "../include/managers/InputManager.hpp"
#include "../include/entities/Arena.hpp"
#include "../include/ChaoticEngine/fachada/CESceneSprite.hpp"
#include <string> //For std::to_string()

/* ****************************** PLAYER HUD ****************************** */
//Destructor
PlayerHUD::~PlayerHUD(){
    if  (face   )   { delete face;      face = nullptr;     }
    if  (bg     )   { delete bg;        bg = nullptr;       }
    if  (hp     )   { delete hp;        hp = nullptr;       }
    if  (mp     )   { delete mp;        mp = nullptr;       }
    if  (up     )   { delete up;        up = nullptr;       }
    if  (lives  )   { delete lives;     lives = nullptr;    }
    if  (stun   )   { delete stun;      stun = nullptr;     }
    if  (shield )   { delete shield;    shield = nullptr;   }
    if  (wings  )   { delete wings;     wings = nullptr;    }
}










/* ****************************** HUD MANAGER ****************************** */
HUDManager* HUDManager::m_instance = nullptr;


//Returns the only instance of this class
HUDManager* HUDManager::instance(){
    if (!m_instance){
        m_instance = new HUDManager();
    }

    return m_instance;
}

//Constructor
HUDManager::HUDManager(){
    m_game              = Game::getInstance();
    m_engineManager     = &EngineManager::instance();
    m_inputManager      = &InputManager::instance();

    initHUD();
}

//Destructor
HUDManager::~HUDManager(){
    for (int i = 0; i < 2; i++){
        if (m_playerHUDs[i])    { delete m_playerHUDs[i];   m_playerHUDs[i] = nullptr;  }
    }

    if (m_ultimateBG)   { delete m_ultimateBG;      m_ultimateBG = nullptr;     }
    if (m_ultimateFG)   { delete m_ultimateFG;      m_ultimateFG = nullptr;     }
    if (m_ultimateText) { delete m_ultimateText;    m_ultimateText = nullptr;   }
    if (m_winnerMsg)    { delete m_winnerMsg;       m_winnerMsg = nullptr;      }

    m_instance = nullptr;
}

void HUDManager::initHUD(){
    initializePlayer(0);
    initializePlayer(1);
    initializeUB();
    initializeWinnerMessage();
}

//Initializes the HUD for a player
void HUDManager::initializePlayer(int p_index){
    PlayerHUD* t_playerHUD = new PlayerHUD();

    //Data getting
    bool t_NPC      = m_game->isNPC(p_index);
    int t_chosen    = m_game->getChosenPlayer(p_index);
    int t_lives     = m_game->getLives();

    //Faces
    std::string t_string1 = "assets/UI/HUD/Faces/";
    std::string t_string2 = "assets/UI/HUD/Faces/";
    std::string t_name;
    std::string t_type;

    switch (t_chosen){
        case 0: { t_name = "PLUP";      break;  }
        case 1: { t_name = "SPARKY";    break;  }
    }

    if (t_NPC){
        t_type = "_AI_";
    }
    else{
        switch (p_index){
            case 0: { t_type = "_P1_";   break;  }
            case 1: { t_type = "_P2_";   break;  }
        }
    }

    t_string1 += t_name + t_type + "OK.png";
    t_string2 += t_name + t_type + "HURT.png";

    const char* t_url1 = t_string1.c_str();
    const char* t_url2 = t_string2.c_str();

    t_playerHUD->face = m_engineManager->createHUD(t_url1, 128, 192, false);
    t_playerHUD->face->addTexture(t_url2);

    //Bars
    t_playerHUD->bg  = m_engineManager->createHUD("assets/UI/HUD/Bars/BG.png" , 300   , 128   , false);
    t_playerHUD->hp  = m_engineManager->createHUD("assets/UI/HUD/Bars/HP.png" , 300   , 128   , false);
    t_playerHUD->mp  = m_engineManager->createHUD("assets/UI/HUD/Bars/MP.png" , 300   , 128   , false);
    t_playerHUD->up  = m_engineManager->createHUD("assets/UI/HUD/Bars/UP.png" , 300   , 128   , false);
    t_playerHUD->hp->addTexture("assets/UI/HUD/Bars/HP_LOW.png");

    //Lives
    t_playerHUD->lives = m_engineManager->createHUD("assets/UI/HUD/Lives/0.png"   , 300,  64   , false);
    t_playerHUD->lives->addTexture("assets/UI/HUD/Lives/1.png");
    t_playerHUD->lives->addTexture("assets/UI/HUD/Lives/2.png");
    t_playerHUD->lives->addTexture("assets/UI/HUD/Lives/3.png");
    t_playerHUD->lives->addTexture("assets/UI/HUD/Lives/4.png");
    t_playerHUD->lives->addTexture("assets/UI/HUD/Lives/5.png");
    t_playerHUD->lives->addTexture("assets/UI/HUD/Lives/6.png");
    t_playerHUD->lives->setTexture(t_lives);

    //Tokens
    t_playerHUD->stun   = m_engineManager->createHUD("assets/UI/HUD/Tokens/Stun_OFF.png", 32, 32, false);
    t_playerHUD->shield = m_engineManager->createHUD("assets/UI/HUD/Tokens/Shield_OFF.png", 32, 32, false);
    t_playerHUD->wings  = m_engineManager->createHUD("assets/UI/HUD/Tokens/Wings_OFF.png", 32, 32, false);

    t_playerHUD->stun->addTexture("assets/UI/HUD/Tokens/Stun.png");
    t_playerHUD->shield->addTexture("assets/UI/HUD/Tokens/Shield.png");
    t_playerHUD->wings->addTexture("assets/UI/HUD/Tokens/Wings.png");

    //Placing
    int t_x = -462 + (675 * p_index);
    //int t_y = -258;
    int t_y = 354;

    t_playerHUD->face->setAbsolutePosition(t_x + 35, t_y, 1);
    t_playerHUD->bg->setAbsolutePosition(t_x + 99, t_y, 1);
    t_playerHUD->hp->setAbsolutePosition(t_x + 99, t_y, 2);
    t_playerHUD->mp->setAbsolutePosition(t_x + 99, t_y, 3);
    t_playerHUD->up->setAbsolutePosition(t_x + 99, t_y, 4);
    t_playerHUD->lives->setAbsolutePosition(t_x + 99, t_y - 64, 1);
    t_playerHUD->stun->setAbsolutePosition(t_x, t_y, 1);
    t_playerHUD->shield->setAbsolutePosition(t_x, t_y - 32, 1);
    t_playerHUD->wings->setAbsolutePosition(t_x, t_y - 64, 1);

    //Scaling
    t_playerHUD->face->setAbsoluteScale(0.5, 0.5, 1);
    t_playerHUD->bg->setAbsoluteScale(0.5, 0.5, 1);
    t_playerHUD->hp->setAbsoluteScale(0.5, 0.5, 1);
    t_playerHUD->mp->setAbsoluteScale(0.5, 0.5, 1);
    t_playerHUD->up->setAbsoluteScale(0.5, 0.5, 1);
    t_playerHUD->lives->setAbsoluteScale(0.5, 0.5, 1);


    m_playerHUDs[p_index] = t_playerHUD;
    setUltimate(p_index, false);
}

//Initializes the HUD for the Ultimate Bar
void HUDManager::initializeUB(){
    m_ultimateBG    = m_engineManager->createHUD("assets/UI/HUD/Bars/UltiBG.png", 300, 20, false);
    m_ultimateFG    = m_engineManager->createHUD("assets/UI/HUD/Bars/UltiFG.png", 300, 20, false);
    m_ultimateText  = m_engineManager->createHUD("assets/UI/HUD/Bars/PortalText.png", 300, 64, false);

    int t_x = -150;
    int t_y = 316;

    m_ultimateBG->setAbsolutePosition(t_x, t_y, 0);
    m_ultimateFG->setAbsolutePosition(t_x, t_y, 1);
    m_ultimateText->setAbsolutePosition(t_x, t_y + 64, 2);

    m_ultimateCharge = 0;
    m_ultimateFG->setAbsoluteScale(0, 1, 1);
}

//Initializes the winner message
void HUDManager::initializeWinnerMessage(){
    m_winnerMsg = m_engineManager->createHUD("assets/UI/HUD/EndGame/J1.png", 530, 300);
    m_winnerMsg->addTexture("assets/UI/HUD/EndGame/J2.png");
    m_winnerMsg->setVisible(false);
}

// Changes the HP of a player in the screen
// Changes the color of the HP bar and the face of the character
void HUDManager::setHP(int p_player, int p_HP, int p_maxHP){
    //High HP
    if (p_HP >= (p_maxHP / 3)){
        m_playerHUDs[p_player]->hp->setTexture(0);
        m_playerHUDs[p_player]->hp->setAbsoluteScale(((float)(p_HP) / (float) p_maxHP) * 0.5, 0.5, 1);
        m_playerHUDs[p_player]->face->setTexture(0);        
    }

    //Low HP
    else{
        m_playerHUDs[p_player]->hp->setTexture(1);
        m_playerHUDs[p_player]->hp->setAbsoluteScale(((float)(p_HP * 3) / (float) p_maxHP) * 0.5, 0.5, 1);        
        m_playerHUDs[p_player]->face->setTexture(1);        
    }
}

//Changes the MP of a player in the screen
void HUDManager::setMP(int p_player, int p_MP, int p_maxMP){
    m_playerHUDs[p_player]->mp->setAbsoluteScale(((float)(p_MP) / (float) p_maxMP) * 0.5, 0.5, 1);
}

//Changes the lives left of a player in the screen
void HUDManager::setLives(int p_player, int p_lives){
    m_playerHUDs[p_player]->lives->setTexture(p_lives);
}

//Fills the Ultimate Bar
void HUDManager::fillUB(float p_delta){
    m_ultimateCharge += p_delta;

    if (m_ultimateCharge >= 3){
        m_ultimateCharge = 0;
    }

    m_ultimateFG->setAbsoluteScale((m_ultimateCharge / 3), 1, 1);
}

// Enables or disables the Ultimate Attack of a player
void HUDManager::setUltimate(int p_player, bool p_active){
    m_playerHUDs[p_player]->ultimateON = p_active;
    m_playerHUDs[p_player]->up->setVisible(p_active);
}

// Makes the Ultimate blink in player HUD
void HUDManager::ultimateBlink(int p_player){
    int t_time = (int) m_inputManager->getMasterClock();
    m_playerHUDs[p_player]->up->setVisible(t_time % 2 == 0);
}

//Enables or disables the stun token in player HUD
void HUDManager::setStun(int p_player, bool p_active){
    m_playerHUDs[p_player]->stun->setTexture((int) p_active);
}

//Enables or disables the shield token in player HUD
void HUDManager::setShield(int p_player, bool p_active){
    m_playerHUDs[p_player]->shield->setTexture((int) p_active);
}

//Enables or disables the wings token in player HUD
void HUDManager::setWings(int p_player, bool p_active){
    m_playerHUDs[p_player]->wings->setTexture((int) p_active);
}

//Shows the match winner (called in EndGameState)
void HUDManager::showWinnerMessage(){
    m_winnerMsg->setTexture(m_game->getWinner());
    m_winnerMsg->setVisible(true);
}

//Updates the UI
void HUDManager::update(){
    m_ultimateBG->setVisible(m_arena->portalIsActive());
    m_ultimateFG->setVisible(m_arena->portalIsActive());
    m_ultimateText->setVisible(m_arena->portalIsActive());

    for (int i = 0; i < 2; i++){
        if (m_playerHUDs[i]->ultimateON){
            ultimateBlink(i);
        }
    }
}

//Displays the UI
void HUDManager::render(){

}

//Initializes the arena pointer
void HUDManager::setArena(){
    m_arena = Arena::getInstance();
}