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

#ifndef CHARACTER_LOCAL_SCREEN
#define CHARACTER_LOCAL_SCREEN

class Game;

#include "MenuScreen.hpp"
#include <vector>

class CharacterLocalScreen : public MenuScreen{
    private:
        CharacterLocalScreen(MenuState* p_menu);
        
        static CharacterLocalScreen*    m_instance;
        Game*                           m_game;

        bool            m_joystickConnected;    
        int             m_nowChoosing;          //Current player choosing
        int             m_chosen[2];            //Chosen characters
        bool            m_NPC[2];               //Playable or NPC

        void    init();
        void    chooseCharacter();
        int     getRandomCharacter();
        void    colorizeBackground();
        void    colorizeBorder();

    public:
        static CharacterLocalScreen* instance();
        ~CharacterLocalScreen();
        
        void update()       override;

        void left()         override;
        void right()        override;
        void select()       override;
        void back()         override;
        void save()         override;
        void toggleNPC()    override;
};

#endif