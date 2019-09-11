/********************************************************************************
*********************************************************************************
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
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You can contact Chaotic Games at: chaoticgamesdev@gmail.com
*********************************************************************************
*********************************************************************************/


#include "../../include/entities/characters/Character.hpp"
#include"../../include/AI/AICharacter.hpp"
#include "../../include/managers/EngineManager.hpp"
#include "../../include/managers/InputManager.hpp"
#include "../../include/managers/PhysicsManager.hpp"
#include "../../include/managers/HUDManager.hpp"
#include "../../include/entities/Arena.hpp"
#include "../../include/extra/Actions.hpp"

#include "../../include/Game.hpp"
#include "../../include/states/MenuState.hpp"

struct ActionMapping{
    Action  action;                     //Action to map
    bool    (Character::*function)();   //Function for the action
    bool    onlyOnce;                   //Wait for input release?
    bool    enabled;                    //Enabled or not
};

//Static members
int Character::m_playerCount = 0;

Arena*          m_arena             = 0;

Character::Character(char* p_name, float p_position[3], int p_HP, int p_MP, float p_velocity, const char* p_modelURL, bool p_online, bool p_NPC) : Entity(p_position, 0.5f, p_modelURL){
    m_game                  = Game::getInstance();
    m_arena                 = Arena::getInstance();
    m_HUDManager            = HUDManager::instance();
    m_client                = &Client::instance();


    m_playerCount           = m_playerCount % 2;
    m_playerIndex           = Character::m_playerCount;
    m_playerCount++;
    m_NPC                   = p_NPC;
    m_AI                    = nullptr;      //Each character will create it if NPC
    m_AIEnabled             = false;
    
    m_name                  = p_name;
    m_lives                 = m_game->getLives();
    m_HP                    = m_maxHP = p_HP;
    m_MP                    = m_maxMP = p_MP;
    m_velocity              = p_velocity;
    m_stunned               = false;  
    m_blocking              = false;
    m_shielded              = false;
    m_winged                = false;    
    m_alive                 = true;
    m_respawning            = false;
    m_knockback             = false;
    m_dashing               = false;
    m_onGround              = false;
    m_ultimateCharged       = false;
    m_runningFactor         = 1.0f;
    m_orientation           = 1;

    m_stopDuration          = 0.15;
    m_stopTime              = 0;
    m_knockbackDuration     = 0.15;
    m_knockbackTime         = 0;
    m_dashDuration          = 0.25;
    m_dashTime              = 0;
    m_stunDuration          = 1.0;
    m_stunTime              = 0;
    m_wingsDuration         = 10.0;
    m_wingsTime             = 0;
    m_shieldDuration        = 5.0;
    m_shieldTime            = 0;

    m_online                = p_online;

    m_moveAmmount = 0;
    m_maxJumps    = 2;
    mapActions();
    setRespawnPosition(m_arena->getRespawnPosition());

    m_waitRelease           = false;
    m_keepWaiting           = false;

    switch(m_playerIndex){
        case 0:
            lookRight();
            break;
        case 1:
            lookLeft();
            break;
    }

    m_validation = 123;

    m_physicsManager->setPlayerSensor(m_idBody, this);
    //createDebug();

    m_damageBasic    = 0;
    m_damageSide     = 0;
    m_damageUp       = 0;
    m_damageDown     = 0;
    m_damageUlti     = 0;
    m_knockbackBasic = 0;
    m_knockbackSide  = 0;
    m_knockbackUp    = 0;
    m_knockbackUlti  = 0;
}

Character::~Character(){
    if(m_AI){
        delete m_AI;
        m_AI = nullptr;
    }

   delete[] m_actions;
   m_actions = nullptr;
}

void Character::setRespawnPosition(float p_respawnPosition[3]){
    for(int i = 0; i < 3; i++){
        m_respawnPosition[i] = p_respawnPosition[i];
    }
}

//Initializes actions mapping for this player
void Character::mapActions(){
    m_actions = new ActionMapping[(int) Action::Count + 1];
                      //Action                    //Function                      //onlyOnce  //Enabled
    m_actions[0]    = {Action::Left               , &Character::left              , false     , false};
    m_actions[1]    = {Action::Right              , &Character::right             , false     , false};
    m_actions[2]    = {Action::Jump               , &Character::jump              , true      , false};
    m_actions[3]    = {Action::Block              , &Character::block             , false     , false};
    m_actions[4]    = {Action::Pick               , &Character::pick              , true      , false};
    m_actions[5]    = {Action::BasicAttack        , &Character::basicAttack       , true      , false};
    m_actions[6]    = {Action::SpecialAttackUp    , &Character::specialAttackUp   , true      , false};
    m_actions[7]    = {Action::SpecialAttackDown  , &Character::specialAttackDown , true      , false};
    m_actions[8]    = {Action::SpecialAttackSide  , &Character::specialAttackSide , true      , false};
    m_actions[9]    = {Action::UltimateAttack     , &Character::ultimateAttack    , true      , false};
    m_actions[10]   = {Action::Taunt              , &Character::tauntSound        , true      , false};
    m_actions[11]   = {Action::Leave              , &Character::leave             , true      , false};
    m_actions[12]   = {Action::ToggleAI           , &Character::toggleAI          , true      , false};
    m_actions[13]   = {Action::Count              , 0                             , false     , false};
}

//Receives an attack from other player
//Parameters: damage, can you block it?
void Character::receiveAttack(int p_damage, bool p_block, float p_knockPower, int p_knockSide, bool p_checked){
    if(m_online && !p_checked){
        if(m_client->getPlayer() == m_playerIndex)
            m_client->attacked(p_damage, p_block, p_knockPower, p_knockSide);
        else return;  //ignorar ataques que no sean de tu jugador
    }

    if((p_block && m_actions[(int) Action::Block].enabled) || m_shielded)
        changeHP(-p_damage/2);
    else
        changeHP(-p_damage);

    if(p_knockSide == 2) //knockback sin direccion
        setKnockback();
    else if(p_knockSide != 0)
        knockback(p_knockSide, p_knockPower);
}

//Increases or decreases life
void Character::changeHP(int p_variation){
    m_HP += p_variation;

    if(m_HP > m_maxHP)
        m_HP = m_maxHP;

    if(m_stunned){
        m_stunned = false;
        m_HUDManager->setStun(m_playerIndex, false);
    }

    //HUD Stuff
    m_HUDManager->setHP(m_playerIndex, m_HP, m_maxHP);
}

void Character::checkAlive(){
    if(m_HP <= 0 && m_lives > 0)
        die();
}

//Increases or decreases magic
void Character::addMP(int p_MP){
    m_MP += p_MP;

    if(m_MP > m_maxMP)
        m_MP = m_maxMP;

    //HUD Stuff
    m_HUDManager->setMP(m_playerIndex, m_MP, m_maxMP);
}

//Check if we can do an action. If we can, substract the MP and return true, if not, return false.
bool Character::useMP(int p_MP){
    //We have enough MP for doing the action
    if(m_MP >= p_MP){
        m_MP -= p_MP;
        m_HUDManager->setMP(m_playerIndex, m_MP, m_maxMP);
        return true;
    }

    return false;
}

//Activates shield
void Character::shield(){
    m_shielded = true;
    m_shieldTime = m_inputManager->getMasterClock() + m_shieldDuration;
    m_HUDManager->setShield(m_playerIndex, true);
}

//Activates wings, if not already active
void Character::wings(){
    if(!m_winged){
        m_runningFactor = 1.5f;
        m_winged = true;
        m_HUDManager->setWings(m_playerIndex, true);
    }

    m_wingsTime = m_inputManager->getMasterClock() + m_wingsDuration;
}

void Character::removeWings(){
    if(m_winged){
        m_runningFactor = 1.0f;
        m_winged = false;
        m_HUDManager->setWings(m_playerIndex, false);
    }
}

//Decreases number of lives
void Character::die(){
    m_lives--;
    
    //HUD Stuff
    m_HUDManager->setLives(m_playerIndex, m_lives);

    m_HP = 0;
    m_knockback = false;
    m_stunned   = false;
    m_shielded = false;

    m_HUDManager->setStun(m_playerIndex, false);
    m_HUDManager->setShield(m_playerIndex, false);

    deathSound();
    removeWings();

    if(m_lives > 0)
        respawn(); 
    else{
        m_alive = false;

        int t_winner = (m_playerIndex == 0 ? 1 : 0);
        m_game->setWinner(t_winner);
    }
}

void Character::lookLeft(){
    m_orientation = -1;
    this->rotate(0);
}

void Character::lookRight(){
    m_orientation = 1;
    this->rotate(180);
}

bool Character::isJumping(){
    return m_actions[(int) Action::Jump].enabled;
}

//Calls action functions when they are active
void Character::doActions(){
    ActionMapping* t_iterator = m_actions;

    while(t_iterator->function){
        //We call the function, it'll return false when action finishes
        if(t_iterator->enabled)
            t_iterator->enabled = (this->*(t_iterator->function))();
        
        ++t_iterator;
    }
}

void Character::input(){
    //NPCs don't need input
    if (m_NPC) return;

    m_inputManager->updatePlayerActions(m_playerIndex);
    
    //For movement
    m_frameDeltaTime = m_engineManager->getFrameDeltaTime();

    //Block
    m_actions[(int) Action::Block].enabled = m_inputManager->checkPlayerAction(Action::Block, m_playerIndex);

    //Input blocked if stunned, blocking or dead
    if(!m_stunned && !m_actions[(int) Action::Block].enabled && m_alive){
        ActionMapping* t_iterator   = m_actions;
        
        //Loop through actions to enable them
        while(t_iterator->function){    
            if (t_iterator->onlyOnce){
                if (m_inputManager->checkPlayerAction(t_iterator->action, m_playerIndex)){
                    m_keepWaiting = true;

                    if (!m_waitRelease){
                        t_iterator->enabled = true;
                        m_waitRelease = true;
                    }
                }
            }

            else{
                t_iterator->enabled = m_inputManager->checkPlayerAction(t_iterator->action, m_playerIndex);
            }

            m_inputManager->setAction(t_iterator->action, m_playerIndex, false);
            ++t_iterator;
        }

        m_waitRelease = m_keepWaiting;
        m_keepWaiting = false;
    }    
}

//Update state of player
void Character::update(){
    m_currentTime = m_inputManager->getMasterClock();

    //Update AI if exists and is enabled
    if(m_AI && m_AIEnabled && !m_stunned)
        m_AI->update();

    //Specific update for each character
    updatePlayer();
    m_moveAmmount = 0;

    if(m_winged && m_currentTime >= m_wingsTime)
        removeWings();

    if(m_shielded && m_currentTime >= m_shieldTime){
        m_shielded = false;
        m_HUDManager->setShield(m_playerIndex, false);
    }

    if(m_stunned && m_currentTime > m_stunTime){
        m_stunDuration = 1.0;
        m_stunned      = false;
        m_HUDManager->setStun(m_playerIndex, false);
    }    
    else
        doActions();

    if(m_knockback && m_currentTime >= m_knockbackTime)
        m_knockback = false;

    if(m_respawning)
        m_respawning = false;
    
    updatePosition();
    
    if((getY() < -25 || getY() > 25 || getX() < -25 || getX() > 25) && m_lives > 0)
        die();

    checkAlive();
}

//Returns the type of the player
int Character::getType(){
    return m_type;
}

//Returns if the player is an NPC
bool Character::isNPC(){
    return m_NPC;
}

//Returns the damage of the player
int Character::getDamage(){
    return m_damage;
}

//Returns the index of the player
int Character::getIndex(){
    return m_playerIndex;
}

//Returns the name of the player
char* Character::getName(){
    return m_name;
}

//Returns the HP of the player
int Character::getHP(){
    return m_HP;
}

//Returns the mp of the player
int Character::getMP(){
    return m_MP;
}

int Character::getOrientation(){
    return m_orientation;
}

void Character::setStunned(float p_time){
    if(p_time != 0)
        m_stunDuration = p_time;
    else
        m_stunDuration = m_stunDuration/2;
    
    m_stunned = true;
    m_HUDManager->setStun(m_playerIndex, true);

    m_stunTime = m_inputManager->getMasterClock() + m_stunDuration;
}

void Character::respawn(){
    m_physicsManager->resetVelocity(m_idBody);
    m_respawning = true;
    m_HP = m_maxHP;
    m_MP = m_maxMP;

    moveTo(m_respawnPosition);
    m_HUDManager->setHP(m_playerIndex, m_HP, m_maxHP);
    m_HUDManager->setMP(m_playerIndex, m_MP, m_maxMP);
}

void Character::onTouchGround(){
    m_onGround = true;
    m_maxJumps = 2;
}

void Character::onLeaveGround(){
    m_onGround = false;
    m_maxJumps = 1;
}

void Character::setUltimateCharged(){
    m_ultimateCharged = true;
}

bool Character::getUltimateCharged(){
    return m_ultimateCharged;
}

bool Character::moveToPath(float p_position[2]){
    b2Vec2 t_position = b2Vec2(m_position[0], m_position[1]);
    Character* t_closestPlayer = m_physicsManager->getClosestCharacter(t_position);
    m_flagAIJump = !m_flagAIJump;
    // Move
    m_frameDeltaTime = m_engineManager->getFrameDeltaTime();
    if(p_position[0] > this->getX()){
        this->right();
    }
    else{
        this->left();
    }

    // Jump if enemy is above
    if(p_position[1] > (this->getY() + 1.5f)){
        if(m_flagAIJump){
            if(t_closestPlayer!=0){
                if(t_closestPlayer->getY() > m_position[1]){
                    jump();
                }
            }
        }
    }
    return false;
}

void Character::setKnockback(){
    m_knockbackTime = m_inputManager->getMasterClock() + m_knockbackDuration;
    m_knockback = true;
    setStunned(0.15);
}

void Character::knockback(int p_orientation, float p_knockPower){
    if(!m_knockback){
        m_sideKnockback = p_orientation;
        setKnockback();
    }
    m_physicsManager->applyKnockback(m_idBody, p_orientation, p_knockPower, m_HP, m_maxHP);
}

int  Character::getCurrentSnowmen(){}

void Character::onPortal(){}

void Character::leavePortal(){}

int Character::getValidation(){
    return m_validation;
}










/* ****************************** ACTIONS ****************************** */
bool Character::left(){
    if(!m_stunned){
        lookLeft();
        if(!m_ultimateMode)
            m_engineManager->changeAnimation(m_modelId, 1);
        
        m_moveAmmount = m_velocity * m_frameDeltaTime * m_runningFactor * -1;
    m_physicsManager->move(m_idBody, m_moveAmmount, 0);
    }
    return false;
}

bool Character::right(){
    if(!m_stunned){
        lookRight();
        if(!m_ultimateMode)
            m_engineManager->changeAnimation(m_modelId, 1);

        m_moveAmmount = m_velocity * m_frameDeltaTime * m_runningFactor * 1;
    m_physicsManager->move(m_idBody, m_moveAmmount, 0);
    }
    return false;
}

bool Character::jump(){
    if(m_maxJumps > 0){
        m_maxJumps--;
        if(!m_ultimateMode)
            m_engineManager->changeAnimation(m_modelId, 2);

        m_physicsManager->jump(m_idBody, 300);
    }
}

bool Character::block(){
    m_actions[(int) Action::Block].enabled = true;

    return false;
}

bool Character::pick(){
    m_arena->catchItem(m_playerIndex, m_position);
    
    return false;
}

bool Character::basicAttack(){}
bool Character::specialAttackUp(){}
bool Character::specialAttackDown(){}
bool Character::specialAttackSide(){}
bool Character::ultimateAttack(){}
bool Character::tauntSound(){}
void Character::deathSound(){}

bool Character::leave(){

    return false;
}

bool Character::toggleAI(){
    m_AIEnabled = !m_AIEnabled;

    return false;
}