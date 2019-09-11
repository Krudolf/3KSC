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

#ifndef CHARACTER_ONLINE_SCREEN
#define CHARACTER_ONLINE_SCREEN

class Game;

#include "MenuScreen.hpp"

class CharacterOnlineScreen : public MenuScreen{
    private:
        CharacterOnlineScreen(MenuState* p_menu);
        
        static CharacterOnlineScreen*   m_instance;
        Game*                           m_game;

    public:
        static CharacterOnlineScreen* instance();
        ~CharacterOnlineScreen();

        void input();
        void update();

        void select()               override;
};

#endif