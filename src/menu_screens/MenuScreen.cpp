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

#include "../include/menu_screens/MenuScreen.hpp"
#include "../include/managers/EngineManager.hpp"
#include "../include/managers/InputManager.hpp"
#include "../include/managers/SoundManager.hpp"
#include "../include/extra/MenuActions.hpp"

#include <fstream>
#include <sstream>
#include <string>

//Struct containing menu actions and its properties
struct MenuActionMapping{
    MenuAction  action;                     //Action to map
    bool        (MenuScreen::*function)();  //Function for the action
    bool        onlyOnce;                   //Wait for input release?
    bool        enabled;                    //Enabled or not
};

//Maps a string to a Screen
struct ScreenMapping{
    std::string string;
    Screen      screen;
};

//Constructor
MenuScreen::MenuScreen(MenuState* p_menu){
    m_menu              = p_menu;
    m_engineManager     = &EngineManager::instance();
    m_inputManager      = &InputManager::instance();
    m_soundManager      = &SoundManager::instance();

    m_screenName        = nullptr;
    m_selectedNode      = nullptr;
    m_selector          = nullptr;
    m_controls          = nullptr;

    m_prev              = Screen::Undefined;
    m_next              = Screen::Undefined;
    m_settings          = Screen::Undefined;
}

MenuScreen::~MenuScreen(){
    m_selectedNode  = nullptr;    

    if  (m_screenName)                          { delete m_screenName;  m_screenName = nullptr; }
    if  (m_selector)                            { delete m_selector;    m_selector = nullptr;   }
    if  (m_controls)                            { delete m_controls;    m_controls = nullptr;   }
    for (MenuNode* t_node : m_nodes)            { delete t_node;    }   m_nodes.clear();
    for (CESceneSprite* t_sprite : m_sprites)   { delete t_sprite;  }   m_sprites.clear();
    for (CESceneSprite* t_sprite : m_extras)    { delete t_sprite;  }   m_extras.clear();
}

//Reads a CGS file and creates the screen from it
void MenuScreen::createFromFile(const char* p_url){
    std::ifstream   t_file(p_url);
    std::string     t_line;
    std::string     t_tag;

    //Loop through file
    while(std::getline(t_file, t_line)){
        //Ignore empty lines and comments
        if (t_line == "" || t_line == "#") continue;

        //Separate string by spaces
        std::istringstream          t_tokens(t_line);
        std::vector<std::string>    t_elements(std::istream_iterator<std::string>{t_tokens}, std::istream_iterator<std::string>());

        t_tag = t_elements[0].c_str();

        //Screen name
        if (t_tag == "sn"){
            const char* t_url   = t_elements[1].c_str();
            float t_x           = strtof(t_elements[2].c_str(), nullptr);
            float t_y           = strtof(t_elements[3].c_str(), nullptr);
            float t_z           = (t_elements.size() == 7 ? strtof(t_elements[6].c_str(), nullptr) : 0);
            float t_width       = strtof(t_elements[4].c_str(), nullptr);
            float t_height      = strtof(t_elements[5].c_str(), nullptr);

            m_screenName = m_engineManager->createSprite(t_url, t_width, t_height);
            m_screenName->setAbsolutePosition(t_x, -t_y, t_z);
        }

        //Menu nodes
        else if (t_tag == "n"){
            const char* t_url   = t_elements[2].c_str();
            float t_x           = strtof(t_elements[3].c_str(), nullptr);
            float t_y           = strtof(t_elements[4].c_str(), nullptr);
            float t_z           = (t_elements.size() == 8 ? strtof(t_elements[7].c_str(), nullptr) : 0);
            float t_width       = strtof(t_elements[5].c_str(), nullptr);
            float t_height      = strtof(t_elements[6].c_str(), nullptr);

            CESceneSprite* t_sprite = m_engineManager->createSprite(t_url, t_width, t_height);
            t_sprite->setAbsolutePosition(t_x, -t_y, t_z);
            
            MenuNode* t_node        = new MenuNode(t_sprite); 
            m_nodes.push_back(t_node);
            m_sprites.push_back(t_sprite);
        }

        //Extra textures
        else if (t_tag == "t"){
            int t_index         = (int) strtof(t_elements[1].c_str(), nullptr);
            const char* t_url   = t_elements[2].c_str();

            m_nodes[t_index]->element->addTexture(t_url);
        }

        //Selector (hand cursor)
        else if (t_tag == "s"){
            const char* t_url   = t_elements[1].c_str();
            float t_width       = strtof(t_elements[2].c_str(), nullptr);
            float t_height      = strtof(t_elements[3].c_str(), nullptr);

            m_selector = m_engineManager->createSprite(t_url, t_width, t_height);
        }

        //Menu nodes horizontal links
        else if (t_tag == "hl"){
            int t_nodeA = (int) strtof(t_elements[1].c_str(), nullptr);
            int t_nodeB = (int) strtof(t_elements[2].c_str(), nullptr);

            m_nodes[t_nodeA]->setRight(m_nodes[t_nodeB]);
        }

        //Menu nodes vertical links
        else if (t_tag == "vl"){
            int t_nodeA = (int) strtof(t_elements[1].c_str(), nullptr);
            int t_nodeB = (int) strtof(t_elements[2].c_str(), nullptr);

            m_nodes[t_nodeA]->setDown(m_nodes[t_nodeB]);            
        }

        //Menu nodes target links
        else if (t_tag == "tl"){
            int t_index = (int) strtof(t_elements[1].c_str(), nullptr);
            ScreenMapping* t_iterator = m_menu->m_screenMaps;

            //Check if the string is equal to a type of Screen
            while(t_iterator->screen != Screen::Undefined){
                if (t_elements[2] == t_iterator->string){
                    m_nodes[t_index]->target = t_iterator->screen;
                    break;
                }

                ++t_iterator;
            }
        }

        //Previous, nexts and settings screens
        else if (t_tag == "ps" || t_tag == "ns" || t_tag == "ss"){
            ScreenMapping* t_iterator = m_menu->m_screenMaps;

            //Check if the string is equal to a type of Screen
            while(t_iterator->screen != Screen::Undefined){
                if (t_elements[1] == t_iterator->string){
                    
                    //Check which type of link is (previous, next or settings)
                    if      (t_tag == "ps"  ) { m_prev      = t_iterator->screen;   }
                    else if (t_tag == "ns"  ) { m_next      = t_iterator->screen;   }
                    else if (t_tag == "ss"  ) { m_settings  = t_iterator->screen;   }

                    break;
                }

                ++t_iterator;
            }
        }

        //Controls tutorial
        else if (t_tag == "c"){
            const char* t_tex1   = t_elements[1].c_str();
            const char* t_tex2   = t_elements[2].c_str();

            m_controls = m_engineManager->createSprite(t_tex1, 1024, 60);
            m_controls->setAbsolutePosition(0, -354, 0);
            m_controls->addTexture(t_tex2);

            //Switch to keyboard controls if no joystick connected
            if (!m_inputManager->isConnected(0))    { m_controls->setTexture(1);    }
        }

        //Extra sprites
        else if (t_tag == "x"){
            const char* t_url   = t_elements[1].c_str();
            float t_x           = strtof(t_elements[2].c_str(), nullptr);
            float t_y           = strtof(t_elements[3].c_str(), nullptr);
            float t_z           = (t_elements.size() == 7 ? strtof(t_elements[6].c_str(), nullptr) : 0);
            float t_width       = strtof(t_elements[4].c_str(), nullptr);
            float t_height      = strtof(t_elements[5].c_str(), nullptr);

            CESceneSprite* t_sprite = m_engineManager->createSprite(t_url, t_width, t_height);
            t_sprite->setAbsolutePosition(t_x, -t_y, t_z);

            m_extras.push_back(t_sprite);
        }
    }

    m_selectedNode = m_nodes[0];
}

int MenuScreen::getSelectedIndex(){
    for (int i = 0; i < m_nodes.size(); i++){
        if (m_selectedNode == m_nodes[i]){
            return i;
        }
    }

    return -1;
}

//Shows all elements in the screen
void MenuScreen::showElements(){
    for (CESceneSprite* t_sprite : m_sprites){
        t_sprite->setVisible(true);
    }

    if (m_screenName)   m_screenName->setVisible(true);
    if (m_selector)     m_selector->setVisible(true);
    if (m_controls)     m_controls->setVisible(true);

    for (CESceneSprite* t_sprite : m_extras){
        t_sprite->setVisible(true);
    }
}

//Hides all elements in the screen
void MenuScreen::hideElements(){
    for (CESceneSprite* t_sprite : m_sprites){
        t_sprite->setVisible(false);
    }

    if (m_screenName)   m_screenName->setVisible(false);
    if (m_selector)     m_selector->setVisible(false);
    if (m_controls)     m_controls->setVisible(false);

    for (CESceneSprite* t_sprite : m_extras){
        t_sprite->setVisible(false);
    }
}


float MenuScreen::getViewportWidth(float p_factor){
    return m_engineManager->getWindowSize().width * p_factor;
}

float MenuScreen::getViewportHeight(float p_factor){
    return m_engineManager->getWindowSize().height * p_factor;
}

void MenuScreen::createControlsBar(int p_variant){
   
}

//Calls action functions when they are active
void MenuScreen::doActions(){
    MenuActionMapping* t_iterator = m_menu->m_actions;

    while(t_iterator->function){
        //We call the function, it'll return false when action finishes
        if(t_iterator->enabled){
            (this->*(t_iterator->function))();
            t_iterator->enabled = false;
        }
        
        ++t_iterator;
    }
}

void MenuScreen::input(){
    m_inputManager->updateMenuActions();

    MenuActionMapping* t_iterator   = m_menu->m_actions;

    //Loop through actions to enable them
    while(t_iterator->function){    
        if (t_iterator->onlyOnce){
            if (m_inputManager->checkMenuAction(t_iterator->action)){
                m_menu->m_keepWaiting = true;

                if (!m_menu->m_waitRelease){
                    t_iterator->enabled = true;
                    m_menu->m_waitRelease = true;
                }
            }
        }

        else{
            t_iterator->enabled = m_inputManager->checkMenuAction(t_iterator->action);
        }

        ++t_iterator;
    }

    m_menu->m_waitRelease = m_menu->m_keepWaiting;
    m_menu->m_keepWaiting = false;
}

void MenuScreen::update(){
    doActions();
}

void MenuScreen::render(){
    m_engineManager->drawScene2D();
}










/* ****************************** ACTIONS ****************************** */
void MenuScreen::up(){
    if (m_selectedNode && m_selectedNode->up){
        m_selectedNode = m_selectedNode->up;
    }
}

void MenuScreen::down(){
    if (m_selectedNode && m_selectedNode->down){
        m_selectedNode = m_selectedNode->down;
    }
}

void MenuScreen::left(){    
    if (m_selectedNode && m_selectedNode->left){
        m_selectedNode = m_selectedNode->left;
    }
}

void MenuScreen::right(){    
    if (m_selectedNode && m_selectedNode->right){
        m_selectedNode = m_selectedNode->right;
    }
}

void MenuScreen::select(){
    m_soundManager->modifyParameter("menu_sounds", 0.9f, "option");
    m_soundManager->playSound("menu_sounds");
    if (m_selectedNode && m_selectedNode->target != Screen::Undefined){
        m_soundManager->modifyParameter("menu_sounds", 0.15f, "option");
        m_soundManager->playSound("menu_sounds");
        m_menu->setScreen(m_selectedNode->target);
    }
}

void MenuScreen::back(){
    m_soundManager->modifyParameter("menu_sounds", 0.4f, "option");
    m_soundManager->playSound("menu_sounds");
    m_menu->setScreen(m_prev);
}

void MenuScreen::save(){
    m_soundManager->modifyParameter("menu_sounds", 0.15f, "option");
    m_soundManager->playSound("menu_sounds");
    m_menu->setScreen(m_next);
}

void MenuScreen::settings(){
    m_soundManager->modifyParameter("menu_sounds", 0.65f, "option");
    m_soundManager->playSound("menu_sounds");
    m_menu->setScreen(m_settings);
}

void MenuScreen::toggleNPC(){}
