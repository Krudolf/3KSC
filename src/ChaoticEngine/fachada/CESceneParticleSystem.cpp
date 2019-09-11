
#include <iostream>
#include <glm.hpp>

#include "../../include/ChaoticEngine/fachada/CESceneParticleSystem.hpp"

/*
Fachada para crear Emisores de particulas y aplicarle las transformaciones correspondientes
*/

CESceneParticleSystem::CESceneParticleSystem(CESceneNode* p_parent, const char* p_path, int p_amount, float p_x, float p_y, GLfloat p_velocity,
		GLfloat p_life, int p_minAngle, int p_maxAngle, bool p_explode, float p_systemLife, GLuint p_shaderProgram){
	m_rotate    = new CETransform();
	m_scale	    = new CETransform();
	m_translate = new CETransform(); 
	setPosition(p_x, p_y, 0);
	m_system 	= new CEParticleSystem(p_path, p_amount, p_x, p_y, p_velocity, p_life, p_minAngle, p_maxAngle, p_explode, p_systemLife, p_shaderProgram);
	
	m_rotate->rotate(0, 0, 0);
	m_scale->scale(1, 1, 1);
	m_translate->translate(0, 0, 0);

	CESceneNode* t_nodeRotate 	 = new CESceneNode(p_parent);
	CESceneNode* t_nodeScale 	 = new CESceneNode(t_nodeRotate);
	CESceneNode* t_nodeTranslate = new CESceneNode(t_nodeScale);
	CESceneNode* t_nodeEmitter 	 = new CESceneNode(t_nodeTranslate);
	t_nodeRotate->setEntity(m_rotate);
	t_nodeScale->setEntity(m_scale);
	t_nodeTranslate->setEntity(m_translate);
	t_nodeEmitter->setEntity(m_system);

}

CESceneParticleSystem::~CESceneParticleSystem(){
	delete m_system;
	m_system = nullptr;
	
	delete m_translate;
	m_translate = nullptr;
	
	delete m_rotate;
	m_rotate = nullptr;
	
	delete m_scale;
	m_scale = nullptr;
}

void CESceneParticleSystem::processInput(GLFWwindow* p_window){
	if(glfwGetKey(p_window, GLFW_KEY_R) == GLFW_PRESS)
		setRotation(0,3,0);
	if(glfwGetKey(p_window, GLFW_KEY_W) == GLFW_PRESS)
		setPosition(0,1,0);
	if(glfwGetKey(p_window, GLFW_KEY_S) == GLFW_PRESS)
		setPosition(0,-1,0);
	if(glfwGetKey(p_window, GLFW_KEY_D) == GLFW_PRESS)
		setPosition(1,0,0);
	if(glfwGetKey(p_window, GLFW_KEY_A) == GLFW_PRESS)
		setPosition(-1,0,0);
}


void CESceneParticleSystem::update(){
	m_system->update();
}

void CESceneParticleSystem::setScale(float p_x, float p_y, float p_z){
	m_scale->scale(p_x, p_y, p_z);
}

void CESceneParticleSystem::setRotation(float p_x, float p_y, float p_z){
	m_rotate->rotate(p_x, p_y, p_z);
}

void CESceneParticleSystem::setPosition(float p_x, float p_y, float p_z){
	m_translate->translate(p_x, p_y, p_z);
}

void CESceneParticleSystem::setAbsoluteScale(float p_x, float p_y, float p_z){
	m_scale->loadIdentity();
	m_scale->scale(p_x, p_y, p_z);
}

void CESceneParticleSystem::setAbsoluteRotation(float p_x, float p_y, float p_z){
	m_rotate->loadIdentity();
	m_rotate->rotate(p_x, p_y, p_z);
}

void CESceneParticleSystem::setAbsolutePosition(float p_x, float p_y, float p_z){
	m_translate->loadIdentity();
	m_translate->translate(p_x, p_y, p_z);	
}

CEParticleSystem* CESceneParticleSystem::getSystem(){
	return m_system;
}

