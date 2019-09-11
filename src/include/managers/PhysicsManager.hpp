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

#ifndef PHYSICS_MANAGER
#define PHYSICS_MANAGER

class EngineManager;

#include "../entities/Entity.hpp"
#include "../entities/characters/Character.hpp"
#include "../entities/items/Portal.hpp"
#include "../managers/ContactManager.hpp"
#include <vector>
#include <Box2D.h>

enum class Box{
    Player = 0,
    Platform,
    Item,
    Portal,
    Bullet,
    Other
};

class PhysicsManager{
private:
    static EngineManager*   m_engineManager;
    std::vector<b2Body*>    m_playersBody;

    b2Vec2*                 m_gravity;
    b2World*                m_world;

    int32                   m_velocityIterations;
    int32                   m_positionIterations;
    float32                 m_timeStep;

    ContactManager*         m_contactManager;

    b2Fixture*              m_portalFixture;

    short                   CATEGORY_PLAYER;
    short                   CATEGORY_ITEM;
    short                   CATEGORY_GROUND;

    float                   m_deltaTime;
    int                     m_currentPlayer;

public:
    static PhysicsManager& instance();
    PhysicsManager();
    ~PhysicsManager();
    
    void                    update(float p_delta);
    void                    clear();
    
    void                    createPhysicBox(Box p_type, int* p_id, float p_position[3], float p_dimX, float p_dimY);

    void                    setPlayerSensor(int p_id, Character* p_character);
    void                    createPhysicBoxPlatform(int* p_id, float p_position[3], bool p_debugMode = false);
    void                    createPhysicBoxPortal(int* p_id, float p_position[3], float p_dimX, float p_dimY);
    void                    addDataToPortal(Portal* p_portal);

    void                    moveBody(int p_idBody, float p_x, float p_y);

    float                   RaycastBetween(b2Vec2 p_p1, b2Vec2 p_p2);
    Character*              getClosestCharacter(b2Vec2 p_p1);
    float                   getDistanceToClosestCharacter(b2Vec2 p_p1);
    float                   getDistanceBetween(b2Vec2 p_p1, b2Vec2 p_p2);

    b2World*                getWorld();
    b2Body*                 getBody(int p_id);
    void                    destroyBody(int p_id);
    int32                   getVelocityIterations();
    int32                   getPositionIterations();
    float32                 getTimeStep();

    void                    applyKnockback(int p_idBody, int t_side, float p_knockPower, int p_HP, int p_maxHP);
    void                    applyKnockback(b2Body* p_body, int t_side, float p_knockPower, int p_HP, int p_maxHP);

    bool                    checkCollisionSimple(b2Body* p_body, bool p_stun, float p_damage, float p_knockPower);
    void                    checkCollisionMultiple(b2Body* p_body, b2Body* p_ignoreBody, float p_damage, float p_knockPower);

    bool                    fixtureCollide(b2Fixture& fixtureA, b2Fixture& fixtureB);

    void                    shockwaveBox(int p_idBody, float p_damage, float p_knockPower);
    void                    sparkyJump(int p_idBody);
    void                    fastGravity(int p_idBody);
    void                    machineGun(int p_idBody, int p_orientation, float p_damage, float p_knockPower, bool p_horizontalSide);
    int                     getTotalFixtures(int p_idBody);
    void                    getPosition(int p_idBody);
    void                    move(int p_idBody, float p_moveX, float p_moveY);
    void                    jump(int p_idBody, float p_force);
    void                    resetVelocity(int p_idBody);
    void                    dash(int p_idBody, int t_side);

    int                     createBodyDebug(b2Fixture* p_fiture);
    int                     createBodyDebug(int p_idBody);
    void                    updateBodyDebug(int p_idBody, int p_idDebug);

};

#endif