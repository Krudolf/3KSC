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

#ifndef INPUT_MANAGER
#define INPUT_MANAGER

class EngineManager;

#include "../extra/MenuActions.hpp"
#include "../extra/Actions.hpp"
#include "../extra/Inputs.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>
#include <Client.hpp>

class InputManager{
public:
    static InputManager& instance();
    InputManager();
    ~InputManager();
    bool    eventHandler();
    void    onKeyPressed(int p_key);
    bool    isKeyPressed(Key p_key);
    bool    isMousePressed();
    int     getMouseX();
    int     getMouseY();
    bool    isConnected(int p_joystick);
    bool    isButtonPressed(int p_joystick, Button p_button);
    float   getAxisPosition(int p_joystick, Axis p_axis);
    void    updateJoysticks();
    void    assignDevice(int p_device, int p_player);
    void    autoassignDevices();
    void    onlineMode();
    void    setOnlineControl(int p_player);
    void    setAction(Action p_action, int p_player, bool p_bool = true); 
    void    setNetPlayer(int p_player, bool p_actions[12]);     

    void    updateActions(int p_player);
    
    int     getInputDevice(int p_player);
    int     getDeviceCount();
    void    updateOnlineInput(int p_player);
    void    sendOnlineInput();

    void    updateMasterClock();
    void    resetMasterClock();
    float   getMasterClock();

    //Menu Actions
    void    updateMenuActions();
    bool    checkMenuAction(MenuAction p_action);

    //Actions
    void    updatePlayerActions(int p_player);
    bool    checkPlayerAction(Action p_action, int p_player);

private:
    EngineManager*          m_engineManager;
    int**                   m_bindings;   
    sf::Keyboard::Key       m_keys[101];
    sf::Joystick::Axis      m_axis[8];
    Client*                 m_client;
    int                     m_onlinePlayer;
    bool                    m_lastActions[11] = {false};
    bool                    m_isOnline;
    float                   m_masterTime;
    sf::Clock               m_masterClock;

    //Event handling
    sf::Window*             m_window;             
    sf::Event*              m_event;

    //Input device for each player [0-3]: Joysticks, -1: Keyboard, -2: NPC
    int     m_inputDevices[4];

    //Menu Actions
    bool    m_menuActions[(int) MenuAction::Count];

    void    menuInputJoystick();
    void    menuInputKeyboard();
    void    menuInputOnline();

    //Actions
    bool    m_playerActions[2][(int) Action::Count];

    void    playerInputJoystick(int p_player);
    void    playerInputKeyboard(int p_player);
    void    playerInputNPC(int p_player);
    void    playerInputOnline(int p_player);
};

#endif