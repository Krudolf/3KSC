/*
    3KSC: A fighting game
    Copyright (C);

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

#ifndef BATTLE_SETTINGS_SCREEN
#define BATTLE_SETTINGS_SCREEN

class Game;

#include "MenuScreen.hpp"

class BattleSettingsScreen : public MenuScreen{
    private:
        BattleSettingsScreen(MenuState* p_menu);
        
        static BattleSettingsScreen*    m_instance;
        Game*                           m_game;

        int                             m_battleSettings[2];    //[ROUNDS, LIVES]

        void                            init();

    public:
        static BattleSettingsScreen* instance();
        ~BattleSettingsScreen();

        void update()   override;

        void left()                 override;
        void right()                override;
        void select()               override;
};

#endif