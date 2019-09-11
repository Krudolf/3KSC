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

#ifndef SPARKY
#define SPARKY

#include "Character.hpp"

class Sparky: public Character {
public:
	Sparky(char* p_name, float p_position[3], bool p_online = false, bool p_NPC = false);
    ~Sparky();
    bool    jump();
    bool    basicAttack();
    bool    specialAttackUp();
    bool    specialAttackDown();
    bool    specialAttackSide();
    bool    ultimateAttack();
    void    updatePlayer();
    void    updateJump();
    bool    tauntSound();
    void    deathSound();

private:
    float           m_jumpingDuration;
    float           m_jumpingTime;

    bool            m_gravity;
    bool            m_sparkyJumping;
    bool            m_punchLaunched;
    bool            m_ultiBulletLaunched;

    float       m_atakOffset;
    float       m_atakTime;

    Projectile*     m_punch;
    Projectile*     m_ultimateBullet;

    int             m_ultimateAmmo;

    int             m_system;
    
    void    updatePunch();
    void    updateUltimate();
    void    randomSounds();

};

#endif