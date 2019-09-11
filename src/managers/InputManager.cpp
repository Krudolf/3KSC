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

#include "../include/managers/InputManager.hpp"
#include "../include/managers/EngineManager.hpp"
#include <SFML/Window/Mouse.hpp>

#include <cstring> //For std::memcpy()

//Returns the only instance of this class
InputManager& InputManager::instance(){
    static InputManager instance;
    return instance;
}

//Constructor
InputManager::InputManager(){
    m_engineManager = &EngineManager::instance();

    m_masterTime = m_masterClock.getElapsedTime().asSeconds();
    m_bindings = 0;

    //Event handling
    m_window    = new sf::Window();
    m_event     = new sf::Event();

    //Key list
    sf::Keyboard::Key t_keys[101] = {sf::Keyboard::A, sf::Keyboard::B, sf::Keyboard::C, sf::Keyboard::D, sf::Keyboard::E, sf::Keyboard::F, sf::Keyboard::G, sf::Keyboard::H, sf::Keyboard::I, sf::Keyboard::J, sf::Keyboard::K, sf::Keyboard::L, sf::Keyboard::M, sf::Keyboard::N, sf::Keyboard::O, sf::Keyboard::P, sf::Keyboard::Q, sf::Keyboard::R, sf::Keyboard::S, sf::Keyboard::T, sf::Keyboard::U, sf::Keyboard::V, sf::Keyboard::W, sf::Keyboard::X, sf::Keyboard::Y, sf::Keyboard::Z, sf::Keyboard::Num0, sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4, sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7, sf::Keyboard::Num8, sf::Keyboard::Num9, sf::Keyboard::Escape, sf::Keyboard::LControl, sf::Keyboard::LShift, sf::Keyboard::LAlt, sf::Keyboard::LSystem , sf::Keyboard::RControl, sf::Keyboard::RShift, sf::Keyboard::RAlt, sf::Keyboard::RSystem , sf::Keyboard::Menu, sf::Keyboard::LBracket, sf::Keyboard::RBracket, sf::Keyboard::SemiColon , sf::Keyboard::Comma, sf::Keyboard::Period, sf::Keyboard::Quote, sf::Keyboard::Slash, sf::Keyboard::BackSlash, sf::Keyboard::Tilde, sf::Keyboard::Equal, sf::Keyboard::Dash, sf::Keyboard::Space, sf::Keyboard::Return, sf::Keyboard::BackSpace, sf::Keyboard::Tab, sf::Keyboard::PageUp, sf::Keyboard::PageDown, sf::Keyboard::End, sf::Keyboard::Home, sf::Keyboard::Insert, sf::Keyboard::Delete, sf::Keyboard::Add, sf::Keyboard::Subtract, sf::Keyboard::Multiply, sf::Keyboard::Divide, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Numpad0, sf::Keyboard::Numpad1, sf::Keyboard::Numpad2, sf::Keyboard::Numpad3, sf::Keyboard::Numpad4, sf::Keyboard::Numpad5, sf::Keyboard::Numpad6, sf::Keyboard::Numpad7, sf::Keyboard::Numpad8, sf::Keyboard::Numpad9, sf::Keyboard::F1, sf::Keyboard::F2, sf::Keyboard::F3, sf::Keyboard::F4, sf::Keyboard::F5, sf::Keyboard::F6, sf::Keyboard::F7, sf::Keyboard::F8, sf::Keyboard::F9, sf::Keyboard::F10, sf::Keyboard::F11, sf::Keyboard::F12, sf::Keyboard::F13, sf::Keyboard::F14, sf::Keyboard::F15, sf::Keyboard::Pause};
    std::memcpy(m_keys, t_keys, 101 * sizeof(sf::Keyboard::Key));

    //Axis list
    sf::Joystick::Axis t_axis[8] = {sf::Joystick::X, sf::Joystick::Y, sf::Joystick::Z, sf::Joystick::R, sf::Joystick::U, sf::Joystick::V, sf::Joystick::PovX, sf::Joystick::PovY};
    std::memcpy(m_axis, t_axis, 8 * sizeof(sf::Joystick::Axis));

    //Devices initialization
    autoassignDevices();
    m_isOnline = false;

    //Initialize action booleans
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < (int) Action::Count; j++){
            m_playerActions[i][j] = false;
        }
    }
}

//Destructor
InputManager::~InputManager(){}

//Catches events and returns true if an event happened
bool InputManager::eventHandler(){
    bool t_eventReceived = false;

    while (m_window->pollEvent(*m_event)){
        switch (m_event->type){
            //New Joystick connected
            case sf::Event::JoystickConnected:{

                t_eventReceived = true;
                break;
            }

            //Lost Joystick connection
            case sf::Event::JoystickDisconnected:{

                t_eventReceived = true;
                break;
            }
        }
    }

    return t_eventReceived;
}

//Checks which devices are connected and assigns devices for everyone
void InputManager::autoassignDevices(){
    bool t_keyboardAssigned = false;
    updateJoysticks();

    for (int i = 0; i < 2; i++){
        if(isConnected(i))
            m_inputDevices[i] = i;
        else{
            if(!t_keyboardAssigned){
                m_inputDevices[i] = -1;
                t_keyboardAssigned = true;
            }else
                m_inputDevices[i] = -2;
        }
    }
}

//Specific Key press handler
void InputManager::onKeyPressed(int p_key){

}

//Returns whether key with code p_key is pressed or not
bool InputManager::isKeyPressed(Key p_key){
    bool t_result = sf::Keyboard::isKeyPressed(m_keys[(int) p_key]);
    return t_result;
}

//Returns whether mouse button p_button is pressed or not
bool InputManager::isMousePressed(){
    return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

}

//Returns the X coordinate of the mouse (relative to window)
int InputManager::getMouseX(){
    return sf::Mouse::getPosition().x - m_engineManager->getWindowPosition().x;
}

//Returns the X coordinate of the mouse (relative to window)
int InputManager::getMouseY(){
    return sf::Mouse::getPosition().y - m_engineManager->getWindowPosition().y;
}

//Checks if controller with index p_index is connected
bool InputManager::isConnected(int p_joystick){
    sf::Joystick::update();
    return sf::Joystick::isConnected(p_joystick);
}

//Returns wether p_button in p_joystic is pressed or not
bool InputManager::isButtonPressed(int p_joystick, Button p_button){
    return sf::Joystick::isButtonPressed(p_joystick, (int) p_button);
}

//Returns the position of the given Axis, in range [-100, 100]
float InputManager::getAxisPosition(int p_joystick, Axis p_axis){
    return sf::Joystick::getAxisPosition(p_joystick, m_axis[(int) p_axis]);
}

//Updates the state of all joysticks
void InputManager::updateJoysticks(){
    sf::Joystick::update();
}

//Assing input device to player
void InputManager::assignDevice(int p_device, int p_player){
    m_inputDevices[p_player] = p_device;
    //Only change device of player 2 for now
    // if (p_player == 1){
    //     m_inputDevices[p_player] = p_device;
    //     std::cout << "Player " << p_player << ": Device " << m_inputDevices[p_player] << std::endl;
    // }
    //COMENTADO PARA EL ONLINE
}

void InputManager::onlineMode(){
    m_client = &Client::instance();
    m_isOnline = true;
    for (int i = 0; i < sizeof(m_inputDevices) / sizeof(int); ++i)
        m_inputDevices[i] = -3; //Unassign devices so server assigns one
}

void InputManager::setOnlineControl(int p_player){
    m_inputDevices[p_player] = -1;
}

void InputManager::setNetPlayer(int p_player, bool p_actions[12]){
    m_playerActions[p_player][(int) Action::Left]                 = p_actions[0]; 
    m_playerActions[p_player][(int) Action::Right]                = p_actions[1];             
    m_playerActions[p_player][(int) Action::Jump]                 = p_actions[2];
    m_playerActions[p_player][(int) Action::Block]                = p_actions[3];
    m_playerActions[p_player][(int) Action::Pick]                 = p_actions[4];
    m_playerActions[p_player][(int) Action::BasicAttack]          = p_actions[5];
    m_playerActions[p_player][(int) Action::SpecialAttackUp]      = p_actions[6];
    m_playerActions[p_player][(int) Action::SpecialAttackDown]    = p_actions[7];
    m_playerActions[p_player][(int) Action::SpecialAttackSide]    = p_actions[8];
    m_playerActions[p_player][(int) Action::UltimateAttack]       = p_actions[9];
}

// Enables an action for AI
void InputManager::setAction(Action p_action, int p_player, bool p_bool){
//    m_playerActions[p_player][(int)p_action] = p_bool;
}

//Returns the input device for the specified player
int InputManager::getInputDevice(int p_player){
    return m_inputDevices[p_player];
}

//Returns the number of players with assigned input device
int InputManager::getDeviceCount(){
    int t_result = 0;

    for (int i = 0; i < 4; i++){
        if (m_inputDevices[i] != -2 && m_inputDevices[i] != -3){
            t_result++;
        }
    }

    return t_result;
}

void InputManager::updateOnlineInput(int p_player){
    bool t_actions[12];
    bool t_flag = false;
    uint i;
    for(i = 0; i < 10; i++){
        if(m_playerActions[p_player][i])
            t_actions[i] = true;
        else
            t_actions[i] = false;

        if(t_actions[i] != m_lastActions[i]){
            m_lastActions[i] = t_actions[i];
            t_flag = true;
        }
    }
    if(t_flag)
        sendOnlineInput();
}

void InputManager::sendOnlineInput(){
    m_client->sendAction(m_lastActions);
}

void InputManager::updateMasterClock(){
    m_masterTime = m_masterClock.getElapsedTime().asSeconds();
}

void InputManager::resetMasterClock(){
    m_masterClock.restart();
}

float InputManager::getMasterClock(){
    return m_masterTime;
}










/* ****************************** MENU ACTIONS ****************************** */
//Calls functions to update menu actions booleans
void InputManager::updateMenuActions(){
    switch(m_inputDevices[0]){
        case 0: case 1: case 2: case 3: {menuInputJoystick();   break;}
        case -1:                        {menuInputKeyboard();   break;}
        case -3:                        {menuInputOnline();     break;}
    }
}

//Updates menu actions (Joystick input)
void InputManager::menuInputJoystick(){
    if(!m_engineManager->isWindowActive()) return;

    //Update joysticks state first
    updateJoysticks();

    m_menuActions[(int) MenuAction::Up]         = getAxisPosition(0, Axis::Y) <= -75 || getAxisPosition(0, Axis::PovY) == -100;
    m_menuActions[(int) MenuAction::Down]       = getAxisPosition(0, Axis::Y) >=  75 || getAxisPosition(0, Axis::PovY) ==  100;
    m_menuActions[(int) MenuAction::Left]       = getAxisPosition(0, Axis::X) <= -75 || getAxisPosition(0, Axis::PovX) == -100;
    m_menuActions[(int) MenuAction::Right]      = getAxisPosition(0, Axis::X) >=  75 || getAxisPosition(0, Axis::PovX) ==  100;
    m_menuActions[(int) MenuAction::Select]     = isButtonPressed(0, Button::A);
    m_menuActions[(int) MenuAction::Back]       = isButtonPressed(0, Button::B);
    m_menuActions[(int) MenuAction::Save]       = isButtonPressed(0, Button::Start);
    m_menuActions[(int) MenuAction::Settings]   = isButtonPressed(0, Button::Y);
    m_menuActions[(int) MenuAction::ToggleNPC]  = isButtonPressed(0, Button::X);
}

//Updates menu actions (Keyboard input)
void InputManager::menuInputKeyboard(){
    if(!m_engineManager->isWindowActive()) return;

    m_menuActions[(int) MenuAction::Up]         = isKeyPressed(Key::Up);
    m_menuActions[(int) MenuAction::Down]       = isKeyPressed(Key::Down);
    m_menuActions[(int) MenuAction::Left]       = isKeyPressed(Key::Left);
    m_menuActions[(int) MenuAction::Right]      = isKeyPressed(Key::Right);
    m_menuActions[(int) MenuAction::Select]     = isKeyPressed(Key::Return);
    m_menuActions[(int) MenuAction::Back]       = isKeyPressed(Key::Escape);
    m_menuActions[(int) MenuAction::Save]       = isKeyPressed(Key::Return);
    m_menuActions[(int) MenuAction::Settings]   = isKeyPressed(Key::Tab);
    m_menuActions[(int) MenuAction::ToggleNPC]  = isKeyPressed(Key::A);
}

//Updates menu actions (for online players)
void InputManager::menuInputOnline(){

}

//Returns true if the asked action's input is enabled
bool InputManager::checkMenuAction(MenuAction p_action){
    return m_menuActions[(int) p_action];
}










/* ****************************** PLAYER ACTIONS ****************************** */
//Calls functions to update player actions booleans
void InputManager::updatePlayerActions(int p_player){
    switch(m_inputDevices[p_player]){
        case 0: case 1: case 2: case 3: {playerInputJoystick(p_player);     break;}
        case -1:                        {playerInputKeyboard(p_player);     break;}
        case -2:                        {playerInputNPC(p_player);          break;}
        case -3:                        {playerInputOnline(p_player);       break;}
    }
    if(m_isOnline && p_player == m_client->getPlayer()){
        updateOnlineInput(p_player);
        m_isOnline = true;
    }
}

//Updates player actions (Joystick input)
void InputManager::playerInputJoystick(int p_player){
    if(!m_engineManager->isWindowActive()) return;

    //Update joysticks state first
    updateJoysticks();

    int t_joystick = m_inputDevices[p_player];

    bool t_up       = getAxisPosition(t_joystick, Axis::Y) <= -75 || getAxisPosition(t_joystick, Axis::PovY) == -100;
    bool t_down     = getAxisPosition(t_joystick, Axis::Y) >=  75 || getAxisPosition(t_joystick, Axis::PovY) ==  100;
    bool t_left     = getAxisPosition(t_joystick, Axis::X) <= -75 || getAxisPosition(t_joystick, Axis::PovX) == -100;
    bool t_right    = getAxisPosition(t_joystick, Axis::X) >=  75 || getAxisPosition(t_joystick, Axis::PovX) ==  100;
    bool t_special  = isButtonPressed(t_joystick, Button::B);
        
    m_playerActions[p_player][(int) Action::Left]   = t_left; 
    m_playerActions[p_player][(int) Action::Right]  = t_right;

    m_playerActions[p_player][(int) Action::Jump]   = isButtonPressed(t_joystick, Button::A);
    m_playerActions[p_player][(int) Action::Block]  = isButtonPressed(t_joystick, Button::RB);
    m_playerActions[p_player][(int) Action::Pick]   = isButtonPressed(t_joystick, Button::Y);

    m_playerActions[p_player][(int) Action::BasicAttack]        = isButtonPressed(t_joystick, Button::X);
    m_playerActions[p_player][(int) Action::SpecialAttackUp]    = t_special && t_up;
    m_playerActions[p_player][(int) Action::SpecialAttackDown]  = t_special && t_down;
    m_playerActions[p_player][(int) Action::SpecialAttackSide]  = t_special && (t_left || t_right);
    m_playerActions[p_player][(int) Action::UltimateAttack]     = getAxisPosition(t_joystick, Axis::Z) >= 0 && getAxisPosition(t_joystick, Axis::R) >= 0;
    
    m_playerActions[p_player][(int) Action::Taunt]              = isButtonPressed(t_joystick, Button::LB);
    m_playerActions[p_player][(int) Action::Leave]              = isButtonPressed(t_joystick, Button::Back);
    m_playerActions[p_player][(int) Action::ToggleAI]           = isButtonPressed(t_joystick, Button::LS);
}

//Updates player actions (Keyboard input)
void InputManager::playerInputKeyboard(int p_player){
    if(!m_engineManager->isWindowActive()) return;

    bool t_up       = isKeyPressed(Key::Up);
    bool t_down     = isKeyPressed(Key::Down);
    bool t_left     = isKeyPressed(Key::Left);
    bool t_right    = isKeyPressed(Key::Right);
    bool t_special  = isKeyPressed(Key::S);

    m_playerActions[p_player][(int) Action::Left]   = t_left;
    m_playerActions[p_player][(int) Action::Right]  = t_right;

    m_playerActions[p_player][(int) Action::Jump]   = isKeyPressed(Key::Space);
    m_playerActions[p_player][(int) Action::Block]  = isKeyPressed(Key::E);
    m_playerActions[p_player][(int) Action::Pick]   = isKeyPressed(Key::D);      

    m_playerActions[p_player][(int) Action::BasicAttack]        = isKeyPressed(Key::A);
    m_playerActions[p_player][(int) Action::SpecialAttackUp]    = t_special && t_up;
    m_playerActions[p_player][(int) Action::SpecialAttackDown]  = t_special && t_down;
    m_playerActions[p_player][(int) Action::SpecialAttackSide]  = t_special && (t_left || t_right);
    m_playerActions[p_player][(int) Action::UltimateAttack]     = isKeyPressed(Key::W);

    m_playerActions[p_player][(int) Action::Taunt]              = isKeyPressed(Key::R);
    m_playerActions[p_player][(int) Action::Leave]              = isKeyPressed(Key::Escape);
    m_playerActions[p_player][(int) Action::ToggleAI]           = isKeyPressed(Key::O);
}

//Updates player actions (for NPC)
void InputManager::playerInputNPC(int p_player){
    for (int i = 0; i < (int) Action::Count; i++){
        //m_playerActions[p_player][i] = false;
    }
}

//Updates player actions (for online players)
void InputManager::playerInputOnline(int p_player){

}

//Returns true if the asked action's input is enabled
bool InputManager::checkPlayerAction(Action p_action, int p_player){
    return m_playerActions[p_player][(int) p_action];
}