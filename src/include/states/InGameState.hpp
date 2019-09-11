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

#ifndef IN_GAME_STATE
#define IN_GAME_STATE

class EngineManager;
class InputManager;
class HUDManager;
class SoundManager;
class PhysicsManager;
class AICharacter;
class Arena;
class Pathfinding;
class Client;

#include "State.hpp"

class InGameState : public State{
    private:
        InGameState(Game* p_game, bool p_onlineMode = false);
        EngineManager*  m_engineManager;
        InputManager*   m_inputManager;
        HUDManager*     m_HUDManager;
        SoundManager*   m_soundManager;
        PhysicsManager* m_physicsManager;
        Arena*          m_arena;
        Pathfinding*    m_pathfinding;
        Client*         m_client;

        bool            m_onlineMode;
        float           m_scale;

        double  m_time;
        int     m_FPS;

        bool    m_changeState;

    public:
        static InGameState& instance();
        ~InGameState();
        void    initState(Game* p_game);
        void    input();
        void    update();
        void    render();
        void    nextState();
        void    createArena(const char* p_fileCgm);

        void    calculateFPS(double t_time);

};

#endif