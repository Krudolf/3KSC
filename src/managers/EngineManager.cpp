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

#include <CE.hpp>
#include "../include/ChaoticEngine/CEparticlesystem.hpp"
#include "../include/managers/EngineManager.hpp"
#include "../include/managers/InputManager.hpp"
#include "../include/extra/ResolutionPresets.hpp"
#include <fstream>
#include <sstream>
#include <string>

//Returns the only instance of this class
EngineManager& EngineManager::instance(){
    static EngineManager instance;
    return instance;
}

//Constructor
EngineManager::EngineManager(){
    m_moveCamera = false;
}

//Destructor
EngineManager::~EngineManager(){}

//Creates the game window
void EngineManager::createWindow(int p_resolutionPreset, bool p_fullscreen){
    int t_width     = g_resolutionPresets[p_resolutionPreset].width,
        t_height    = g_resolutionPresets[p_resolutionPreset].height;

    m_window = new CEWindow(t_width, t_height, "3KSC", p_fullscreen);
    // m_window = new CEWindow(640, 480, "3KSC", p_fullscreen);
    m_scene  = new CEScene();
    m_scene2D= new CEScene();
}

CEPosition EngineManager::getWindowPosition(){
    return m_window->getPosition();
}

CESize EngineManager::getWindowSize(){
    return m_window->getSize();
}

bool EngineManager::isWindowActive(){
   // return m_window->isOpen();
   if(glfwGetWindowAttrib(m_window->getWindow(), GLFW_FOCUSED))
        return true;
    return false;
}

//Hides or shows the cursor
void EngineManager::setCursorVisible(bool p_visible){
    m_window->setCursorVisible(p_visible);
}

//Returns whether the device is running or not
bool EngineManager::running(){
    return m_window->isOpen();
}

void EngineManager::swapBuffers(){
    m_window->swapBuffers();
}

void EngineManager::pollEvents(){
    m_window->pollEvents();
}


//Drops the device
void EngineManager::stop(){
    m_scene->release();
    m_window->close();
}

//Creates a camera
void EngineManager::createCamera(float p_cameraPosition[3], float p_target[3]){
    m_cameraNode = m_scene->createCamera(true);
    if(m_cameraNode){
        m_resetPosition[0] = m_cameraPosition[0] = p_cameraPosition[0];
        m_resetPosition[1] = m_cameraPosition[1] = p_cameraPosition[1];
        m_resetPosition[2] = m_cameraPosition[2] = p_cameraPosition[2];
        m_resetPosition[3] = m_cameraPosition[3] = p_target[0];
        m_resetPosition[4] = m_cameraPosition[4] = p_target[1];
        m_resetPosition[5] = m_cameraPosition[5] = p_target[2];
        m_cameraNode->setAbsolutePosition(p_cameraPosition[0],p_cameraPosition[1],p_cameraPosition[2]);
        m_cameraNode->lookAt(p_target[0],p_target[1],p_target[2]);
    }
}

void EngineManager::moveCamera(float p_posX, float p_posY, float p_posZ){
    if(m_cameraNode){
        m_moveCamera = true;
        m_amountTime = 0.0;

        m_newCameraPosition[0] = p_posX;
        m_newCameraPosition[1] = p_posY+35;
        m_newCameraPosition[2] = p_posZ-75;
        m_newCameraPosition[3] = p_posX;
        m_newCameraPosition[4] = p_posY;
        m_newCameraPosition[5] = p_posZ;

        //m_cameraNode->setPosition(core::vector3df(p_posX, p_posY+35,p_posZ-75));
        //m_cameraNode->setTarget(core::vector3df(p_posX, p_posY,p_posZ));
    }
}

void EngineManager::resetCamera(){
    if(m_cameraNode){
        m_moveCamera = true;
        m_amountTime = 0.0;

        m_newCameraPosition[0] = m_resetPosition[0];
        m_newCameraPosition[1] = m_resetPosition[1];
        m_newCameraPosition[2] = m_resetPosition[2];
        m_newCameraPosition[3] = m_resetPosition[3];
        m_newCameraPosition[4] = m_resetPosition[4];
        m_newCameraPosition[5] = m_resetPosition[5];
    }
}

void EngineManager::createGlobalLight(float p_lightPosition[3], float p_lightDirection[3]){
    CESceneLight* t_light = m_scene->createDirectionalLight(p_lightDirection);
    if(t_light){
        t_light->setAbsolutePosition(p_lightPosition[0],p_lightPosition[1],p_lightPosition[2]);
    }
}

void EngineManager::createPointLight(float p_lightPosition[3], float p_lightAtenuation){
    CESceneLight* t_light = m_scene->createPointLight(p_lightAtenuation);
    if(t_light){
        t_light->setAbsolutePosition(p_lightPosition[0],p_lightPosition[1],p_lightPosition[2]);
    }
}

//Sets frame delta time of the last frame (in seconds) and prepares it for next update
void EngineManager::updateFrameDeltaTime(float p_delta){
    m_frameDeltaTime = p_delta;
}

void EngineManager::deleteEntity(int p_id){
    m_scene->remove(m_entityNodes[p_id]->getTopNode());
}

void EngineManager::deleteDebug(int p_id){
    m_scene->remove(m_debugNodes[p_id]->getTopNode());
}

void EngineManager::deleteEntityAnim(int p_id){
    m_scene->remove(m_animationNodes[p_id]->getTopNode());
}

void EngineManager::preLoadAssets(){
    float t_position[3] = {0,0,0};
    float t_scale[3] = {1,1,1};

    //Precargamos los objetos
    loadAnimations(t_position, t_scale, "assets/models/items/portal/portal_anim.anim");
    loadAnimations(t_position, t_scale, "assets/models/characters/plup/plup.anim");
    loadAnimations(t_position, t_scale, "assets/models/characters/sparky/sparky_animations.anim");
    m_scene->createMesh("assets/models/items/movement_wings.obj");
    m_scene->createMesh("assets/models/items/life_tank.obj");
    m_scene->createMesh("assets/models/items/shield.obj");
    m_scene->createMesh("assets/models/characters/plup/munyeco_plup.obj");

    cleanScene();
}

//Loads animations of 3D models
int EngineManager::loadAnimations(float p_position[3], float p_scale[3], const char* p_modelURL){

    std::ifstream t_file(p_modelURL);
    std::string t_line;
    const char* t_path;
    CESceneAnimation* t_animation = NULL;
    int t_cont = 0;
    float t_loop;


    while(std::getline(t_file, t_line)){
        if(t_line == "" || t_line[0] == '#')// Skip everything and continue with the next line
            continue;

        std::istringstream t_tokens(t_line);
        std::vector<std::string> t_elements(std::istream_iterator<std::string>{t_tokens}, std::istream_iterator<std::string>());
        t_path = t_elements[0].c_str();
        t_loop = strtof((t_elements[2]).c_str(), 0);

        if(t_animation == NULL){
            t_animation = m_scene->createAnimatedMesh(t_path, t_loop);
            t_animation->changeCurrentAnimation(0);
        }
        else{
            t_animation->loadAnimation(t_path, t_loop);
        }
        float t_number = strtof((t_elements[1]).c_str(), 0);
        if(t_number == 1)
            t_animation->changeCurrentAnimation(t_cont);

        t_cont ++;
    }
    if(t_animation){
        t_animation->setAbsolutePosition(p_position[0], p_position[1], p_position[2]);
        t_animation->setAbsoluteScale(p_scale[0], p_scale[1], p_scale[2]);

        m_animationNodes.push_back(t_animation);
        return m_animationNodes.size()-1;
    }
    return -1;
}

//change the current animations
void EngineManager::changeAnimation(int p_animation, int p_newAnimation){
    if(m_animationNodes[p_animation] != NULL){
        m_animationNodes[p_animation]->changeCurrentAnimation(p_newAnimation);
    }
}

//Loads a 3D model
int EngineManager::load3DModel(float p_position[3], float p_scale[3], const char* p_modelURL){
    CESceneMesh* t_mesh = m_scene->createMesh(p_modelURL);

    if(t_mesh){
        t_mesh->setAbsolutePosition(p_position[0], p_position[1], p_position[2]);
        t_mesh->setAbsoluteScale(p_scale[0], p_scale[1], p_scale[2]);

        m_entityNodes.push_back(t_mesh);
        return m_entityNodes.size()-1;
    }
    return -1;
}

void EngineManager::loadSkybox(const char* p_skyboxURLs[6], float t_scale){
    CESceneSkybox* t_skybox = m_scene->createSkybox(p_skyboxURLs, t_scale);
}

void EngineManager::moveEntity(Entity* p_entity){
    float* t_position = p_entity->getPosition();
    if(p_entity->getType() == 0 || p_entity->getType() == 3){
        m_animationNodes[p_entity->getModelId()]->setAbsolutePosition(t_position[0], t_position[1], t_position[2]);
    }
    else{
        m_entityNodes[p_entity->getId()]->setAbsolutePosition(t_position[0], t_position[1], t_position[2]);
    }
}

void EngineManager::setRotation(int p_id, float p_degrees){
    CESceneMesh* t_node  = m_entityNodes[p_id];
    
    t_node->setAbsoluteRotation(0.0f, p_degrees, 0.0f);
}

void EngineManager::setAnimRotation(int p_id, float p_degrees){
    CESceneAnimation* t_node  = m_animationNodes[p_id];
    
    t_node->setAbsoluteRotation(0.0f, p_degrees, 0.0f);
}

void EngineManager::scale(int p_id, float p_scale[3]){
    CESceneMesh* t_node  = m_entityNodes[p_id];

    t_node->setAbsoluteScale(p_scale[0], p_scale[1], p_scale[2]);
}

//Scene render function
void EngineManager::drawScene(){
    m_window->clear(0.047f, 0.165f, 0.549f, 1.0f);
    m_scene->draw();
}

void EngineManager::drawScene2D(){
    m_window->clear(0.047f, 0.165f, 0.549f, 1.0f);
    m_scene2D->draw2D();
}

void EngineManager::cleanScene(){
    m_scene->clean();

    m_entityNodes.clear();
    m_debugNodes.clear();
    m_systems.clear();
    m_animationNodes.clear();
}

float EngineManager::getFrameDeltaTime(){
    return (float)m_frameDeltaTime;
}

CESceneMesh* EngineManager::getEntityNode(int p_id){
    return m_entityNodes[p_id];
}

glm::vec3 EngineManager::getEntityPosition(int p_id){
    return m_entityNodes[p_id]->getPosition();
}

void EngineManager::parseOBJ(const char* p_filename){
    m_VertexX.clear();
    m_VertexY.clear();
    m_VertexZ.clear();

    bool t_newObject = false;

    float t_X    =  0.0,   t_Y    =  0.0,   t_Z    =  0.0;
    float t_maxX = -999.0, t_maxY = -999.0, t_maxZ = -999.0;
    float t_minX =  999.0, t_minY =  999.0, t_minZ =  999.0;

    std::ifstream t_file(p_filename);
    std::string t_line;
    std::string t_name;
    bool pisa = false;
    while(std::getline(t_file, t_line)){
        if(t_line == "" || t_line[0] == '#')// Skip everything and continue with the next line
            continue;

        std::istringstream t_tokens(t_line);
        std::vector<std::string> t_elements(std::istream_iterator<std::string>{t_tokens}, std::istream_iterator<std::string>());

        if(t_elements[0].compare("o") == 0){
            if(t_elements[1].compare("pisa") == 0){
                pisa = true;
                if(m_totalVertex != 0){
                    pushVertex(t_minX, t_maxX, t_minY, t_maxY, t_minZ, t_maxZ);

                    t_maxX = -999.0, t_maxY = -999.0, t_maxZ = -999.0;
                    t_minX =  999.0, t_minY =  999.0, t_minZ =  999.0;
                }
                m_totalVertex++;
            }
            else if(t_elements[1].compare("no") == 0){
                pisa = false;
            }
        }

        if(t_elements[0].compare("v") == 0 && pisa){// Vertex
            sscanf(t_line.c_str(), "%*s %f %f %f", &t_X, &t_Y, &t_Z);

            compareMaxAndMin(t_X, t_maxX, t_minX);
            compareMaxAndMin(t_Y, t_maxY, t_minY);
            compareMaxAndMin(t_Z, t_maxZ, t_minZ);
        }
    }
    pushVertex(t_minX, t_maxX, t_minY, t_maxY, t_minZ, t_maxZ);
}

void EngineManager::compareMaxAndMin(float p_value, float &p_max, float &p_min){
    if(p_value > p_max)
        p_max = p_value;

    if(p_value < p_min)
        p_min = p_value;
}

void EngineManager::pushVertex(float p_minX, float p_maxX, float p_minY, float p_maxY, float p_minZ, float p_maxZ){
    m_VertexX.push_back(p_minX);
    m_VertexX.push_back(p_maxX);

    m_VertexY.push_back(p_minY);
    m_VertexY.push_back(p_maxY);

    m_VertexZ.push_back(p_minZ);
    m_VertexZ.push_back(p_maxZ);

    
    /*std::cout <<
        "Objeto: " << m_totalVertex << "\n" <<
        "PosMin: " << p_minX << "," << p_minY << "," << p_minZ << "\n" <<
        "PosMax: " << p_maxX << "," << p_maxY << "," << p_maxZ << "\n" <<
        "---------------------------------\n";
    */
}

int EngineManager::createDebugQuad(float p_vertex[4][2]){
    CESceneQuad* t_quad = m_scene->createQuad(p_vertex);
    m_debugNodes.push_back(t_quad);
    
    return (m_debugNodes.size()-1);
}

void EngineManager::updateDebugQuad(int p_idDebug, float p_vertex[4][2]){
    m_debugNodes[p_idDebug]->updatePositions(p_vertex);
}

CESceneSprite* EngineManager::createSprite(const char* p_url, float p_width, float p_height, bool p_originCenter){
    return m_scene2D->createSprite(p_url, p_width, p_height, p_originCenter);
}

CESceneSprite* EngineManager::createHUD(const char* p_url, float p_width, float p_height, bool p_originCenter){
    return m_scene->createSprite(p_url, p_width, p_height, p_originCenter);
}

int EngineManager::createParticleSystem(const char* p_path, int p_amount, float p_x, float p_y, GLfloat p_velocity, GLfloat p_life, int p_minAngle, int p_maxAngle, bool p_explode, float p_systemLife){
    CESceneParticleSystem* t_system = m_scene->createParticleSystem(p_path, p_amount, p_x, p_y, p_velocity, p_life, p_minAngle, p_maxAngle, p_explode, p_systemLife);
    m_systems.push_back(t_system);
    // particlePosition(m_systems.size()-1, 10, 10, 1);
    //     std::cout<<"creando"<<std::endl;
    return m_systems.size()-1;

}

CESceneParticleSystem* EngineManager::getParticleSystem(int p_id)
{
    return m_systems.at(p_id);
}

void EngineManager::updateParticleSystem(){
    for (unsigned i=0; i< m_systems.size(); ++i)
    {
        if(m_systems[i])
        {
            m_systems[i]->update();
        }
        
    }
}

void EngineManager::deleteParticleSystem(int p_id){
    m_scene->remove(m_systems[p_id]->getTopNode());
    m_systems[p_id] = nullptr;
}

void EngineManager::deleteParticleSystem(CEParticleSystem* p_system){
    for(uint i = 0; i < m_systems.size(); i++)
    {
        if(m_systems[i] && m_systems[i]->getSystem() == p_system)
           deleteParticleSystem(i);
    }   
}

double EngineManager::getTime(){
    return m_window->getTimer();
}

double EngineManager::getElapsedTime(){
    return m_window->getElapsedTime();
}

void EngineManager::particlePosition(int p_id, int p_x, int p_y, int p_z)
{
    getParticleSystem(p_id)->setPosition(p_x, p_y, p_z);
}