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

#ifndef MENU_SCREEN
#define MENU_SCREEN

#include "../states/MenuState.hpp"
#include <CESceneSprite.hpp>

class EngineManager;
class SoundManager;
class InputManager;

struct MenuNode{
    //Attributes
    MenuNode*           up;
    MenuNode*           down;
    MenuNode*           left;
    MenuNode*           right;
    Screen              target;
    CESceneSprite*      element;

    //Methods
    MenuNode(CESceneSprite* p_element){
        up = down = left = right = nullptr;
        target      = Screen::Undefined;
        element     = p_element;
    }
    ~MenuNode(){}

    void    setUp(MenuNode* p_node)     { this->up      = p_node;    p_node->down   = this; }
    void    setDown(MenuNode* p_node)   { this->down    = p_node;    p_node->up     = this; }
    void    setLeft(MenuNode* p_node)   { this->left    = p_node;    p_node->right  = this; }
    void    setRight(MenuNode* p_node)  { this->right   = p_node;    p_node->left   = this; }
    void    setTarget(Screen p_screen)  { this->target  = p_screen;                         }
};

class MenuScreen{
    protected:
        MenuScreen(MenuState* p_menu);

        MenuState*          m_menu;
        EngineManager*      m_engineManager;
        SoundManager*       m_soundManager;
        InputManager*       m_inputManager;
        
        CESceneSprite*              m_screenName;
        std::vector<MenuNode*>      m_nodes;  
        std::vector<CESceneSprite*> m_sprites;
        MenuNode*                   m_selectedNode;
        CESceneSprite*              m_selector;
        CESceneSprite*              m_controls;
        std::vector<CESceneSprite*> m_extras;

        Screen              m_prev;             //Previous screen
        Screen              m_next;             //Next screen (save)
        Screen              m_settings;        //Settings screen for that menu



        void    createFromFile(const char* p_url);
        int     getSelectedIndex(); 
        
        //Return the width/height of the viewport. Optionally, they can return part of it.
        float   getViewportWidth(float p_factor = 1);
        float   getViewportHeight(float p_factor = 1);
        
        void createControlsBar(int p_variant);

        virtual void doActions();

    public:
        virtual ~MenuScreen();

        void showElements();
        void hideElements();

        void setPrev(Screen p_prev)                 { m_prev = p_prev; }

        virtual void input();
        virtual void update();
        virtual void render();
        
        virtual void up();
        virtual void down();
        virtual void left();
        virtual void right();
        virtual void select();
        virtual void back();
        virtual void save();
        virtual void settings();
        virtual void toggleNPC();
};

#endif