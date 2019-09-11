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

#ifndef CHARACTER
#define CHARACTER

class AICharacter;

#include <SFML/System.hpp>

#include "../Entity.hpp"
#include "../Projectile.hpp"
#include <Client.hpp>

struct ActionMapping;
class Game;
class HUDManager;

class Character : public Entity{
public:
    Character(char* p_name, float p_position[3], int p_HP, int p_MP, float p_velocity, const char* p_modelURL, bool p_online = false, bool p_NPC = false);
    virtual ~Character();

    void            getRespawnPosition();
	
    void            receiveAttack(int p_damage, bool p_block, float p_knockPower, int p_knockSide = 0, bool p_checked = 0);
    virtual void    changeHP(int p_variation);
    void            checkAlive();
    void            addMP(int p_MP);
    bool            useMP(int p_MP);
    void            die();
    void            setRespawnPosition(float p_respawnPosition[3]);
    void            respawn();
    void            shield();
    void            wings();
    void            removeWings();
    void            lookLeft();
    void            lookRight();
    void            input();
    void            update();
    
    bool            isNPC();
    int             getType();
    int             getDamage();
    int             getIndex();
    char*           getName();
    int             getHP();
    int             getMaxHP() { return m_maxHP; }
    int             getMP();
    int             getOrientation();
    bool            getAlive()  {   return m_alive; }
    void            setStunned(float p_time = 0);
    bool            isJumping();
    void            onTouchGround();
    void            onLeaveGround();
    void            setUltimateCharged();
    bool            getUltimateCharged();
    void            onPortal();
    void            leavePortal();
    
    //Actions
    bool            left();
    bool            right();
    bool            jump();
    bool            run();
    bool            block();
    bool            pick();
	virtual bool    basicAttack();
    virtual bool    specialAttackUp();
    virtual bool    specialAttackDown();
    virtual bool    specialAttackSide();
    virtual bool    ultimateAttack();
    virtual bool    tauntSound();
    virtual void    deathSound();
    bool            leave();
    bool            toggleAI();

    virtual int     getCurrentSnowmen();
    bool            moveToPath(float p_position[2]);

    void            setKnockback();
    void            knockback(int p_orientation, float p_knockPower);

    virtual void    updatePlayer() = 0;

    int             getValidation();

protected:
    Game*           m_game;
    HUDManager*     m_HUDManager;

    static int      m_playerCount;
    int             m_playerIndex;
    bool            m_NPC;
    AICharacter*    m_AI;

    int             m_type;                 //0-5: {Kira, Luka, Miyagi, Plup, Rawr, Sparky}
    char*           m_name;
    int             m_lives;
    int             m_HP;
    int             m_MP;     
    int             m_maxHP;
    int             m_maxMP;
    int             m_damage;
    float           m_velocity;
    bool            m_ultimateMode;
    float           m_attackPosition[3];
    float           m_attackTarget[3];
    float           m_respawnPosition[3];
    int             m_orientation;
    bool            m_stunned;
    bool            m_blocking;
    bool            m_shielded;
    bool            m_winged;
    bool            m_alive;
    bool            m_respawning;
    bool            m_onGround;
    bool            m_ultimateCharged;
    float           m_frameDeltaTime;       //For movement
    float           m_runningFactor;
    float           m_currentTime;

    //Jumps
    int             m_maxJumps;
    bool            m_jumping;              // Controls if the jump is still going

    ActionMapping*  m_actions;
    void            mapActions();

    //Debug*          m_playerDebug;
    bool            m_debugMode;

    float           m_stopDuration;
    float           m_stopTime;
    float           m_knockbackDuration;
    float           m_knockbackTime;
    float           m_dashDuration;
    float           m_dashTime;
    float           m_stunDuration;
    float           m_stunTime;
    float           m_wingsDuration;
    float           m_wingsTime;
    float           m_shieldDuration;
    float           m_shieldTime;

    int             m_sideKnockback;
    bool            m_knockback;
    bool            m_dashing;
    float           m_stunnedTime;
    int             m_validation;
    bool            m_online;

    float           m_moveAmmount;
    float           m_damageBasic;
    float           m_damageSide;
    float           m_damageUp;
    float           m_damageDown;
    float           m_damageUlti;
    float           m_knockbackBasic;
    float           m_knockbackSide;
    float           m_knockbackUp;
    float           m_knockbackDown;
    float           m_knockbackUlti;

private:
    bool            m_waitRelease;
    bool            m_keepWaiting;
    bool            m_AIEnabled;
    bool            m_flagAIJump = false;
    Client*         m_client;

    void            doActions();
};

#endif