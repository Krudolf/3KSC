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

#include "include/Game.hpp"
#include "include/managers/EngineManager.hpp"
#include "include/managers/InputManager.hpp"
#include "include/managers/SoundManager.hpp"
#include "include/states/MenuState.hpp"
#include "include/states/InGameState.hpp"

#include <stdio.h>
#include <sys/time.h>
#include <chrono>

//Instance initialization
Game* Game::m_instance = nullptr;

//Constructor
Game::Game(){
    Game::m_instance    = this;
    m_engineManager     = &EngineManager::instance();
    m_inputManager      = &InputManager::instance();
    m_soundManager      = &SoundManager::instance();
    
    const int FPS = 170;
    m_nanoFrames = 1000000000/FPS;
    m_elapsedTotal = 0;

    m_quitRequest       = false;

    m_resolutionPreset  = 1;
    m_fullscreen        = false;

    m_volumes[0]        = 10;
    m_volumes[1]        = 10;
    m_volumes[2]        = 10;

    for (int i = 0; i < 2; i++){
        m_NPC[i]            = false;
        m_chosenPlayers[i]  = 0;
    }

    m_battleSettings[0] = 2;
    m_battleSettings[1] = 3;

    m_map               = 0;

    m_winner            = 0;
    
    m_engineManager->createWindow(m_resolutionPreset, false);
    m_state = &MenuState::instance();
    //m_state = &InGameState::instance();
}

//Destructor
Game::~Game(){
    Game::m_instance = nullptr;
}

//Changes to an specified state
void Game::setState(State* p_state){
    m_state = p_state;
}

//Changes to the next stage
void Game::nextState(){
    m_state->nextState();
}

//Main loop of the game
void Game::run(){
    auto t_now      = std::chrono::high_resolution_clock::now();
    auto t_elapsed  = std::chrono::high_resolution_clock::now() - t_now;

    while(m_engineManager->running() && !m_quitRequest){
        t_elapsed = std::chrono::high_resolution_clock::now() - t_now;
        m_elapsedTotal += std::chrono::duration_cast<std::chrono::nanoseconds>(t_elapsed).count();

        m_engineManager->pollEvents();

        // while(m_elapsedTotal > m_nanoFrames){
            fixedUpdate(m_elapsedTotal);
            m_elapsedTotal  -= m_nanoFrames;
            m_engineManager->swapBuffers();
            // std::cout << m_elapsedTotal << "\t\t" << m_nanoFrames << std::endl;
        // }

        t_now = std::chrono::high_resolution_clock::now();
        m_inputManager->updateMasterClock();
    }

    m_engineManager->stop();
}

void Game::fixedUpdate(long long p_delta){
    m_state->setDeltaTime(p_delta);
    m_state->input();
    m_state->update();
    m_state->render();
}

 void Game::setVolume(int p_type, float p_vol){
    switch(p_type){
        case 0:
            m_soundManager->setMusicVolume(p_vol);
        break;

        case 1:
            m_soundManager->setEffectVolume(p_vol);
            m_soundManager->modifyParameter("menu_sounds", 0.15f, "option");
            m_soundManager->playSound("menu_sounds");
        break;
    }
    m_volumes[p_type] = p_vol/0.05;
 }