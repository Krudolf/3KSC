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

#include "../include/entities/Entity.hpp"
#include "../include/managers/EngineManager.hpp"
#include "../include/managers/PhysicsManager.hpp"
#include "../include/managers/InputManager.hpp"
#include "../include/managers/SoundManager.hpp"
#include <cstring> //For std::memcpy()

//Entity count initialization
int Entity::m_entityCount = 0;

//Create entity with model (proportional scale)
Entity::Entity(float p_position[3], float p_scale, const char* p_modelURL, int p_type){
    m_engineManager     = &EngineManager::instance();
    m_physicsManager    = &PhysicsManager::instance();
    m_inputManager      = &InputManager::instance();
    m_soundManager      = &SoundManager::instance();

    m_idDebug = -1;

    m_idBody = m_entityCount++;    
    float t_scale[3] = {p_scale, p_scale, p_scale};
    for(int i = 0; i < 3; i++){
        m_position[i] = p_position[i];
        m_lastPosition[i] = p_position[i];
    }

    m_id      = -1;
    m_modelId = -1;
    m_type = p_type;

    if(p_type == 0 || p_type == 3)
        m_modelId = m_engineManager->loadAnimations(p_position, t_scale, p_modelURL);
    else
        m_id = m_engineManager->load3DModel(p_position, t_scale, p_modelURL);
    
    moveTo(p_position);

    m_debugMode = false;
    switch(p_type){
        //PLAYER
        case 0:
            m_physicsManager->createPhysicBox(Box::Player, &m_idBody, p_position, 0.5, 0.6);
            break;
        //ARENA
        case 1:
            m_engineManager->parseOBJ(p_modelURL);
            m_physicsManager->createPhysicBoxPlatform(&m_idBody, p_position, m_debugMode);
            break;
        //ITEM
        case 2:
            m_physicsManager->createPhysicBox(Box::Item, &m_idBody, p_position, 0.5, 0.5);
            break;
        //PORTAL
        case 3:
            m_physicsManager->createPhysicBoxPortal(&m_idBody, p_position, 1.4, 0.75);
            break;
        //PROJECTILE
        case 4:
            m_physicsManager->createPhysicBox(Box::Bullet, &m_idBody, p_position, 0.5, 0.5);
            break;
        //SNOWMAN
        case 5:
            m_physicsManager->createPhysicBox(Box::Other, &m_idBody, p_position, 0.5, 0.5);
            break;
    }

    if(m_debugMode)
        createDebug();
}

Entity::~Entity(){
    if(m_debugMode)
        m_engineManager->deleteDebug(m_idDebug);
    
    if(m_modelId != -1)
        m_engineManager->deleteEntityAnim(m_modelId);
    else
        m_engineManager->deleteEntity(m_id);

    m_physicsManager->destroyBody(m_idBody);
}

void Entity::resetCount(){
    m_entityCount = 0;
}

void Entity::updatePosition(){
    if(m_debugMode)
        updateDebug();

    m_lastPosition[0] = m_position[0];
    m_lastPosition[1] = m_position[1];

    //Set to the entity the new position of the body
    m_position[0] = m_physicsManager->getBody(m_idBody)->GetPosition().x;
    m_position[1] = m_physicsManager->getBody(m_idBody)->GetPosition().y;

    m_engineManager->moveEntity(this);
}

void Entity::moveTo(float p_position[3]){
    m_physicsManager->moveBody(m_idBody, p_position[0], p_position[1]);
}

void Entity::moveTo(float p_x, float p_y){
    float t_position[3] = {p_x, p_y, getZ()};
    moveTo(t_position);
}

void Entity::moveX(float p_variation){
    moveTo(getX() + p_variation, getY());
}

void Entity::moveY(float p_variation){
    moveTo(getX(), getY() + p_variation);
}

void Entity::moveZ(float p_variation){
    m_lastPosition[2] = m_position[2];
    m_position[2] += p_variation;
}

void Entity::moveXY(float p_variationX, float p_variationY){
    moveTo(getX() + p_variationX, getY() + p_variationY);
}

//Checks if an entity is close to a certain point (in specified range)
bool Entity::checkCloseness(float* p_point, float p_range){  
    //X axis
    if(p_point[0] >= m_position[0] - p_range && p_point[0] <= m_position[0] + p_range){
        //Y axis
        if(p_point[1] >= m_position[1] - p_range && p_point[1] <= m_position[1] + p_range)
            return true;
    }   
    return false;
}

int Entity::getId(){
    return m_id;
}

int Entity::getModelId(){
    return m_modelId;
}

float* Entity::getElapsedPosition(){
    m_elapsed[0] = m_position[0] - m_lastPosition[0];
    m_elapsed[1] = m_position[1] - m_lastPosition[1];
    m_elapsed[2] = m_position[2] - m_lastPosition[2];

    return m_elapsed;
}

float* Entity::getPosition(){
    return m_position;
}

float Entity::getX(){
    return m_physicsManager->getBody(m_idBody)->GetPosition().x;
}

float Entity::getVX(){
    return m_physicsManager->getBody(m_idBody)->GetLinearVelocity().x;
}

void Entity::setVX(float x){
    m_physicsManager->move(getId(), x, 0);
}

float Entity::getY(){
    return m_physicsManager->getBody(m_idBody)->GetPosition().y;
}

float Entity::getZ(){
    return m_position[2];
}

int Entity::getEntityCount(){
    return Entity::m_entityCount;
}

void Entity::rotate(float p_degrees){
    if(m_type == 0)
        m_engineManager->setAnimRotation(this->getModelId(), p_degrees);
    else
        m_engineManager->setRotation(this->getId(), p_degrees);
}

void Entity::setX(float p_position){
    moveTo(p_position, getY());
}

void Entity::setY(float p_position){
    moveTo(getX(), p_position);
}

void Entity::createDebug(){
    m_idDebug = m_physicsManager->createBodyDebug(m_idBody);
}
void Entity::updateDebug(){
    m_physicsManager->updateBodyDebug(m_idBody, m_idDebug);
}