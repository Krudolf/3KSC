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

#ifndef GAME
#define GAME

class EngineManager;
class InputManager;
class SoundManager;
class State;

class Game{
    private:
        static Game*    m_instance;

        EngineManager*  m_engineManager;
        InputManager*   m_inputManager;
        SoundManager*   m_soundManager;
        State*          m_state;
        
        long long       m_elapsedTotal;
        long long       m_nanoFrames;

        bool            m_quitRequest;

        //Video settings
        int             m_resolutionPreset;
        bool            m_fullscreen;

        //Audio settings
        int             m_volumes[3];           //[BGM, FX, VOICES]

        //Character settings
        bool            m_NPC[2];
        int             m_chosenPlayers[2];

        //Battle settings
        int             m_battleSettings[2];    //[ROUNDS, LIVES]     

        //Map
        int             m_map;

        int             m_winner;

    public:
        Game();
        ~Game();
        static Game* getInstance() { return m_instance; }

        void setState(State* p_state);
        void nextState();
        
        void run();
        void fixedUpdate(long long p_delta);
        void quit()                                 { m_quitRequest = true; }

        //Getters
        int         getResolutionPreset()           { return m_resolutionPreset; }
        bool        isFullscreen()                  { return m_fullscreen; }

        int         getVolBGM()                     { return m_volumes[0];  }
        int         getVolFX()                      { return m_volumes[1];  }
        int         getVolVoices()                  { return m_volumes[2];  }

        bool        isNPC(int p_player)             { return m_NPC[p_player]; }
        int         getChosenPlayer(int p_player)   { return m_chosenPlayers[p_player]; }

        int         getLives()                      { return m_battleSettings[0];   }
        int         getDifficulty()                 { return m_battleSettings[1];   }

        int         getMap()                        { return m_map;                 }
        
        int         getWinner()                     { return m_winner;              }
        
        //Setters
        void        setResolutionPreset(int p_preset);
        void        changeVideoMode();

        void        setVolume(int p_type, float p_vol);

        void        setPlayerNPC(int p_player, bool p_NPC = true)   { m_NPC[p_player] = p_NPC;                  }
        void        setChosenPlayer(int p_player, int p_character)  { m_chosenPlayers[p_player] = p_character;  }
        
        void        setBattleSettings(int p_type, int p_value)      { m_battleSettings[p_type] = p_value;       }

        void        setMap(int p_map)                               { m_map = p_map;                            }
        
        void        setWinner(int p_winner)                         { m_winner = p_winner;                      }
};

#endif