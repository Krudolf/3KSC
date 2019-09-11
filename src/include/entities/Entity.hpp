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

#ifndef ENTITY
#define ENTITY

class EngineManager;
class PhysicsManager;
class InputManager;
class SoundManager;
//class Debug;

class Entity {
public:
    Entity(float p_position[3], float p_scale, const char* p_modelURL, int p_type = 0);
    virtual ~Entity();
    void        moveTo(float p_position[3]);
    void        moveTo(float p_y, float p_x);
    void        moveX(float p_variation);
    void        moveY(float p_variation);
    void        moveZ(float p_variation);
    void        moveXY(float p_variationX, float p_variationY);
    bool        checkCloseness(float* p_point, float p_range);
    int         getId();
    int         getType(){ return m_type;}
    int         getModelId();
    float*      getPosition();
    float*      getElapsedPosition();
    float       getX();
    float       getY();
    float       getZ();
    float       getVX();
    void        setX(float p_position);
    void        setY(float p_position);
    void        setVX(float x);
    void        rotate(float degrees);
    void        updatePosition();
    static int  getEntityCount();


    void        createDebug();
    void        updateDebug();

protected:
    EngineManager*   m_engineManager;
    PhysicsManager*  m_physicsManager;
    InputManager*    m_inputManager;
    SoundManager*    m_soundManager;

    static void             resetCount();
    static int              m_entityCount;
    int                     m_id;
    int                     m_idBody;
    int                     m_modelId; 
    int                     m_type;
    
    float                   m_position[3]; // [0] for x, [1] for y, [2] for z
    float                   m_lastPosition[3]; // [0] for x, [1] for y, [2] for z
    float                   m_elapsed[3];

    const char*             m_modelURL;
    float                   m_weight;
    
    bool                    m_debugMode;
    int                     m_totalFixtures;
    int                     m_idDebug;
    //Debug*                  m_entityDebug[4];
};

#endif