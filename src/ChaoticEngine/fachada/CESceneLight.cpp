
#include <iostream>

#include "../../include/ChaoticEngine/fachada/CESceneLight.hpp"

/*
Fachada para crear Luces y aplicarle las transformaciones correspondientes
*/

CESceneLight::CESceneLight(CESceneNode* p_parent, float p_attenuation, GLuint p_shaderProgram, bool p_isActive){
	m_isActive	  = p_isActive;

	m_rotate    = new CETransform();
	m_translate = new CETransform();
	m_light 	= new CELight(p_attenuation, p_shaderProgram);
	
	m_rotate->rotate(0, 0, 0);
	m_translate->translate(0, 0, 0);

	CESceneNode* t_nodeRotate 	 = new CESceneNode(p_parent);
	CESceneNode* t_nodeTranslate = new CESceneNode(t_nodeRotate);
	CESceneNode* t_nodeLight 	 = new CESceneNode(t_nodeTranslate);
	t_nodeRotate->setEntity(m_rotate);
	t_nodeTranslate->setEntity(m_translate);
	t_nodeLight->setEntity(m_light);
}

CESceneLight::CESceneLight(CESceneNode* p_parent, glm::vec3 p_direction, GLuint p_shaderProgram, bool p_isActive){
	m_isActive	  = p_isActive;

	m_rotate    = new CETransform();
	m_translate = new CETransform();
	m_light 	= new CELight(p_direction, p_shaderProgram);
	
	m_rotate->rotate(0, 0, 0);
	m_translate->translate(0, 0, 0);

	CESceneNode* t_nodeRotate 	 = new CESceneNode(p_parent);
	CESceneNode* t_nodeTranslate = new CESceneNode(t_nodeRotate);
	CESceneNode* t_nodeLight 	 = new CESceneNode(t_nodeTranslate);
	t_nodeRotate->setEntity(m_rotate);
	t_nodeTranslate->setEntity(m_translate);
	t_nodeLight->setEntity(m_light);
}

CESceneLight::~CESceneLight(){
	delete m_light;
	m_light = nullptr;
	
	delete m_translate;
	m_translate = nullptr;
	
	delete m_rotate;
	m_rotate = nullptr;
}

void CESceneLight::setRotation(float p_x, float p_y, float p_z){
	m_rotate->rotate(p_x, p_y, p_z);
}

void CESceneLight::setPosition(float p_x, float p_y, float p_z){
	m_translate->translate(p_x, p_y, p_z);
}

void CESceneLight::setAbsoluteRotation(float p_x, float p_y, float p_z){
	m_rotate->loadIdentity();
	m_rotate->rotate(p_x, p_y, p_z);
}

void CESceneLight::setAbsolutePosition(float p_x, float p_y, float p_z){
	m_translate->loadIdentity();
	m_translate->translate(p_x, p_y, p_z);	
}