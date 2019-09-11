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

#ifndef MENU_STATE
#define MENU_STATE

class EngineManager;
class SoundManager;
class MenuScreen;

struct MenuActionMapping;
struct ScreenMapping;

#include "State.hpp"
#include "../extra/Screens.hpp"
#include <vector>

class MenuState : public State{
    friend class MenuScreen;
    friend class BattleSettingsScreen;
    friend class CharacterLocalScreen;
    friend class CharacterOnlineScreen;
    friend class GameSettingsScreen;
    friend class MainScreen;
    friend class MapScreen;
    friend class OnlineCreateScreen;
    friend class OnlineJoinScreen;
    friend class OnlineModeScreen;

    private:
        MenuState(Game* p_game);
        static MenuState*           m_instance;
        EngineManager*              m_engineManager;
        SoundManager*               m_soundManager;
        std::vector<MenuScreen*>    m_screens;
        MenuScreen*                 m_currentScreen;


        MenuActionMapping*  m_actions;
        ScreenMapping*      m_screenMaps;

        void mapActions();
        void mapScreens();

        bool m_waitRelease;
        bool m_keepWaiting;

    public:
        ~MenuState();
        static MenuState* getInstance();
        static MenuState& instance();
        void initializeScreens();

        void    nextState();
        void    setScreen(Screen p_screen);
        void    goToMainScreen();

        
        void    input();
        void    update();
        void    render();
};

#endif