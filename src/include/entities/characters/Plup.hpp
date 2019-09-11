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

#ifndef PLUP
#define PLUP

class Snowman;

#include "Character.hpp"
#include <SFML/System/Clock.hpp>

class Plup: public Character {
public:
    Plup(char* p_name, float p_position[3], bool p_online = false, bool p_NPC = false);
    ~Plup();
    bool    jump();
    bool    basicAttack();
    bool    specialAttackUp();
    bool    specialAttackDown();
    bool    specialAttackSide();
    bool    ultimateAttack();
    void    updatePlayer();
    int     getCurrentSnowmen();
    bool    tauntSound();
    void    deathSound();


private:
    bool        m_snowmanPlaced;
    bool        m_kalasnikov;
    bool        m_kalasnikovBulletLaunched;

    int         m_kalasnikovAmmo;
    int         m_maxSnowmen;
    Snowman*    m_snowman;

    Projectile* m_kalasnikovBullet;

    float       m_turretDuration;
    float       m_turretTime;
    float       m_atakOffset;
    float       m_atakTime;
    float       m_ultimateDuration;
    float       m_ultimateTime;
    float       m_kalasnikovOffset;
    float       m_kalasnikovTime;

    void        updateSnowman();
    void        deleteSnowman();
    void        updateDash();
    void        updateKalasnikov();
    void        randomSounds();
};

#endif