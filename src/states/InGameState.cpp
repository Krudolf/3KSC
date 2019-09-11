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

#include "../include/states/InGameState.hpp"
#include "../include/states/EndGameState.hpp"
#include "../include/states/MenuState.hpp"
#include "../include/Game.hpp"

#include "../include/managers/EngineManager.hpp"
#include "../include/managers/InputManager.hpp"
#include "../include/managers/HUDManager.hpp"
#include "../include/managers/SoundManager.hpp"
#include "../include/managers/PhysicsManager.hpp"
#include "../include/entities/Arena.hpp"
#include "../include/AI/Pathfinding.hpp"
#include "../include/Client.hpp"

#include "../include/extra/Inputs.hpp"
#include "../include/extra/Screens.hpp"

#include <fstream>
#include <sstream>
#include <string>


InGameState* m_instance = nullptr;

//Instance initialization
InGameState& InGameState::instance(){
    static InGameState instance(Game::getInstance());
    instance.initState(Game::getInstance());

    return instance;
}

//Constructor
InGameState::InGameState(Game* p_game, bool p_onlineMode){
    //Online stuff
    m_instance = this;
    m_onlineMode = false;
}

//Destructor
InGameState::~InGameState(){
    delete m_arena;
    m_arena = nullptr;
}

void InGameState::initState(Game* p_game){
    m_game              = p_game;
    m_engineManager     = &EngineManager::instance();
    m_inputManager      = &InputManager::instance();
    m_HUDManager        = HUDManager::instance();
    m_soundManager      = &SoundManager::instance();
    m_physicsManager    = &PhysicsManager::instance();
    m_pathfinding       = &Pathfinding::instance();
    m_deltaTime         = 0;

    createArena("assets/Fusfus_Stadium.cgm");
    m_HUDManager->setArena();

    if(m_onlineMode){
        m_client = &Client::instance();
        m_client->start();
        m_inputManager->onlineMode();
        m_onlineMode = true;
    }
    else{
       // m_arena->spawnItems();
        m_arena->spawnPlayers();
    }

    m_time  = 0;
    m_FPS   = 0;

    m_changeState = false;
}


void InGameState::input(){
    if(m_inputManager->isKeyPressed(Key::Escape)){
        this->nextState();
    }
}

void InGameState::update(){
    double t_time = m_engineManager->getElapsedTime();
    m_soundManager->update();
    m_engineManager->updateFrameDeltaTime(t_time);

    if(m_onlineMode)
        m_client->update((float)t_time);
    else
        m_arena->update((float)t_time);

    int t_playerCount = m_arena->getPlayerCount();
    Character* t_currentPlayer;
    
    //Input and update for every character
    for(int i = 0; i < t_playerCount && !m_changeState; i++){
        t_currentPlayer = m_arena->getPlayer(i);

        if(t_currentPlayer){
            t_currentPlayer->input();
            t_currentPlayer->update();
            m_changeState = !t_currentPlayer->getAlive();
        }
    }
    //Update the physics one step more(need to be done first of all)
    m_physicsManager->update(m_deltaTime);

    //m_engineManager->updateParticleSystem();
    m_HUDManager->update();

    //Winner
    if(m_changeState){
        m_HUDManager->showWinnerMessage();

        bool t_kbInput  = m_inputManager->isKeyPressed(Key::Return);
        bool t_jsInput  = m_inputManager->isConnected(0) && (m_inputManager->isButtonPressed(0, Button::Start));

        if (t_kbInput || t_jsInput){
            this->nextState();
        }

    }

    // calculateFPS(t_time);
}

void InGameState::calculateFPS(double t_time){
    // std::cout << t_time << std::endl;
    m_time += t_time;
    m_FPS++;
    if(m_time >= 1.0){
        std::cout << m_FPS << " FPS" << std::endl;
        m_time  = 0;
        m_FPS   = 0;
    }
}

void InGameState::render(){
    m_engineManager->drawScene();
}

//Change to next state
void InGameState::nextState(){
    if (m_arena)        { delete m_arena;       m_arena = nullptr;  }
    if (m_HUDManager)   { delete m_HUDManager;  m_HUDManager = nullptr;}
    m_engineManager->cleanScene();
    m_soundManager->stopAll();
    m_physicsManager->clear();
    MenuState::getInstance()->goToMainScreen();
    m_game->setState(&MenuState::instance());
}

void InGameState::createArena(const char* p_fileCgm){
    std::ifstream t_file(p_fileCgm);
    std::string t_line;
    std::string t_name;

    while(std::getline(t_file, t_line)){
        if(t_line == "" || t_line[0] == '#')// Skip everything and continue with the next line
            continue;

        std::istringstream t_tokens(t_line);
        std::vector<std::string> t_elements(std::istream_iterator<std::string>{t_tokens}, std::istream_iterator<std::string>());
        t_name = t_elements[0].c_str();

        //models, scale and position
        if(t_name == "m"){
            float t_scale = strtof((t_elements[2]).c_str(), 0);
            m_scale = t_scale;
            float t_position[3];
            t_position[0] = strtof((t_elements[3]).c_str(), 0);
            t_position[1] = strtof((t_elements[4]).c_str(), 0);
            t_position[2] = strtof((t_elements[5]).c_str(), 0);

            const char* t_path = t_elements[1].c_str();
            //load Arena
            m_arena = new Arena(t_position, t_scale, t_path);
        }
         //Create camera
        else if(t_name == "c"){
            float t_position[3];
            t_position[0] = strtof((t_elements[1]).c_str(), 0) * m_scale;
            t_position[1] = strtof((t_elements[2]).c_str(), 0) * m_scale;
            t_position[2] = strtof((t_elements[3]).c_str(), 0) * m_scale;

            float t_target[3];
            t_target[0] = strtof((t_elements[4]).c_str(), 0) * m_scale;
            t_target[1] = strtof((t_elements[5]).c_str(), 0) * m_scale;
            t_target[2] = strtof((t_elements[6]).c_str(), 0) * m_scale;

            m_engineManager->createCamera(t_position, t_target);
        }
        //light
        else if(t_name == "gl"){
            float t_position[3];
            t_position[0] = strtof((t_elements[1]).c_str(), 0);
            t_position[1] = strtof((t_elements[2]).c_str(), 0);
            t_position[2] = strtof((t_elements[3]).c_str(), 0);

            float t_direction[3];
            t_direction[0] = strtof((t_elements[4]).c_str(), 0);
            t_direction[1] = strtof((t_elements[5]).c_str(), 0);
            t_direction[2] = strtof((t_elements[6]).c_str(), 0);

            m_engineManager->createGlobalLight(t_position, t_direction);
        }
        else if(t_name == "pl"){
            float t_position[3];
            t_position[0] = strtof((t_elements[1]).c_str(), 0);
            t_position[1] = strtof((t_elements[2]).c_str(), 0);
            t_position[2] = strtof((t_elements[3]).c_str(), 0);

            float t_atenuation = strtof((t_elements[3]).c_str(), 0);

            m_engineManager->createPointLight(t_position, t_atenuation);
        }
        //music
        else if(t_name == "mu"){
            if(t_elements[1].compare("SoundID::S_FOSFOS_STADIUM") == 0)
                m_soundManager->loadBank(SoundID::S_FOSFOS_STADIUM);

            const char* t_path = t_elements[2].c_str();
            const char* t_name = t_elements[3].c_str();
            m_soundManager->loadEvents(SoundID::S_FOSFOS_STADIUM);
            m_soundManager->playMusic(t_name);
        }
        //create waypoints
        else if(t_name == "w"){
            float t_position[3];
            t_position[0] = strtof((t_elements[1]).c_str(), 0);
            t_position[1] = strtof((t_elements[2]).c_str(), 0) * m_scale;
            t_position[2] = strtof((t_elements[3]).c_str(), 0) * m_scale;

            m_pathfinding->addWaypoint(t_position);
        }
        //connets the waypoints
        else if(t_name == "wp"){
            float t_id1 = strtof((t_elements[1]).c_str(), 0);
            float t_id2 = strtof((t_elements[2]).c_str(), 0);

            m_pathfinding->connectWaypoints(t_id1, t_id2);
        }
        //spawn positions from players
        else if(t_name == "sp"){
            float t_spawnPositions[4][3];
            t_spawnPositions[0][0] = strtof((t_elements[1]).c_str(), 0) * m_scale;
            t_spawnPositions[0][1] = strtof((t_elements[2]).c_str(), 0) * m_scale;
            t_spawnPositions[0][2] = strtof((t_elements[3]).c_str(), 0) * m_scale;

            t_spawnPositions[1][0] = strtof((t_elements[4]).c_str(), 0) * m_scale;
            t_spawnPositions[1][1] = strtof((t_elements[5]).c_str(), 0) * m_scale;
            t_spawnPositions[1][2] = strtof((t_elements[6]).c_str(), 0) * m_scale;

            t_spawnPositions[2][0] = strtof((t_elements[7]).c_str(), 0) * m_scale;
            t_spawnPositions[2][1] = strtof((t_elements[8]).c_str(), 0) * m_scale;
            t_spawnPositions[2][2] = strtof((t_elements[9]).c_str(), 0) * m_scale;

            t_spawnPositions[3][0] = strtof((t_elements[10]).c_str(), 0) * m_scale;
            t_spawnPositions[3][1] = strtof((t_elements[11]).c_str(), 0) * m_scale;
            t_spawnPositions[3][2] = strtof((t_elements[12]).c_str(), 0) * m_scale;

            m_arena->setSpawnPositions(t_spawnPositions);
        }
        else if(t_name == "rp"){
            float t_respawnPosition[3];
            t_respawnPosition[0] = strtof((t_elements[1]).c_str(), 0) * m_scale;
            t_respawnPosition[1] = strtof((t_elements[2]).c_str(), 0) * m_scale;
            t_respawnPosition[2] = strtof((t_elements[3]).c_str(), 0) * m_scale;

            m_arena->setRespawnPositions(t_respawnPosition);
        }
        else if(t_name == "si"){
            float t_itemRange[3];
            t_itemRange[0] = strtof((t_elements[1]).c_str(), 0) * m_scale;
            t_itemRange[1] = strtof((t_elements[2]).c_str(), 0) * m_scale;
            t_itemRange[2] = strtof((t_elements[3]).c_str(), 0) * m_scale;

            m_arena->setItemRange(t_itemRange);
        }
        else if(t_name == "sk"){
            const char* t_skyPath[6]; 
            float t_scale = strtof((t_elements[1]).c_str(), 0);
            
            t_skyPath[0] = t_elements[2].c_str(); 
            t_skyPath[1] = t_elements[3].c_str(); 
            t_skyPath[2] = t_elements[4].c_str(); 
            t_skyPath[3] = t_elements[5].c_str(); 
            t_skyPath[4] = t_elements[6].c_str(); 
            t_skyPath[5] = t_elements[7].c_str(); 

            m_engineManager->loadSkybox(t_skyPath, t_scale);
        }
    }
}