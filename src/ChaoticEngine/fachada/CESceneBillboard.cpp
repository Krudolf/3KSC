
#include <iostream>
#include <glm.hpp>

#include "../../include/ChaoticEngine/fachada/CESceneBillboard.hpp"

/*
Fachada para crear Billboars y aplicarle las transformaciones correspondientes
*/

CESceneBillboard::CESceneBillboard(CESceneNode* p_parent, const char* p_path, float p_width, float p_height, GLuint p_shaderProgram){
	m_rotate    = new CETransform();
	m_scale	    = new CETransform();
	m_translate = new CETransform(); 
	m_sprite 	= new CESprite(p_path, p_width, p_height, p_shaderProgram);
	
	m_rotate->rotate(0, 0, 180);
	m_scale->scale(1, 1, 1);
	m_translate->translate(0, 2, 0);
	m_sprite->loadResource(p_path);

	CESceneNode* t_nodeRotate 	 = new CESceneNode(p_parent);
	CESceneNode* t_nodeScale 	 = new CESceneNode(t_nodeRotate);
	CESceneNode* t_nodeTranslate = new CESceneNode(t_nodeScale);
	CESceneNode* t_nodeSprite 	 = new CESceneNode(t_nodeTranslate);
	t_nodeRotate->setEntity(m_rotate);
	t_nodeScale->setEntity(m_scale);
	t_nodeTranslate->setEntity(m_translate);
	t_nodeSprite->setEntity(m_sprite);
}

CESceneBillboard::~CESceneBillboard(){
	delete m_sprite;
	m_sprite = nullptr;
	
	delete m_translate;
	m_translate = nullptr;
	
	delete m_rotate;
	m_rotate = nullptr;
	
	delete m_scale;
	m_scale = nullptr;
}

void CESceneBillboard::setScale(float p_x, float p_y, float p_z){
	m_scale->scale(p_x, p_y, p_z);
}

void CESceneBillboard::setRotation(float p_x, float p_y, float p_z){
	m_rotate->rotate(p_x, p_y, p_z);
}

void CESceneBillboard::setPosition(float p_x, float p_y, float p_z){
	m_translate->translate(p_x, p_y, p_z);
}

void CESceneBillboard::setAbsoluteScale(float p_x, float p_y, float p_z){
	m_scale->loadIdentity();
	m_scale->scale(p_x, p_y, p_z);
}

void CESceneBillboard::setAbsoluteRotation(float p_x, float p_y, float p_z){
	m_rotate->loadIdentity();
	m_rotate->rotate(p_x, p_y, p_z);
}

void CESceneBillboard::setAbsolutePosition(float p_x, float p_y, float p_z){
	m_translate->loadIdentity();
	m_translate->translate(p_x, p_y, p_z);	
}