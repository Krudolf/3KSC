
#include <iostream>
#include <glm.hpp>

#include "../../include/ChaoticEngine/fachada/CESceneSkybox.hpp"

/*
Fachada para crear un Skybox y aplicarle las transformaciones correspondientes
*/

CESceneSkybox::CESceneSkybox(CESceneNode* p_parent, const char* p_path[6], float p_scale, GLuint p_shaderProgram){
	m_rotate    = new CETransform();
	m_scale	 	= new CETransform();
	m_translate = new CETransform(); 
	m_skybox	= new CESkybox(p_scale, p_shaderProgram);
	
	m_rotate->rotate(0, 0, 0);
	m_scale->scale(1,1,1);
	m_translate->translate(0, 0, 0);
	m_skybox->loadCubemap(p_path);

	CESceneNode* t_nodeRotate 	 = new CESceneNode(p_parent);
	CESceneNode* t_nodeScale 	 = new CESceneNode(t_nodeRotate);
	CESceneNode* t_nodeTranslate = new CESceneNode(t_nodeScale);
	CESceneNode* t_nodeSkybox 	 = new CESceneNode(t_nodeTranslate);
	t_nodeRotate->setEntity(m_rotate);
	t_nodeScale->setEntity(m_scale);
	t_nodeTranslate->setEntity(m_translate);
	t_nodeSkybox->setEntity(m_skybox);
}

CESceneSkybox::~CESceneSkybox(){
	delete m_skybox;
	m_skybox = nullptr;
	
	delete m_translate;
	m_translate = nullptr;
	
	delete m_rotate;
	m_rotate = nullptr;
	
	delete m_scale;
	m_scale = nullptr;
}

void CESceneSkybox::setScale(float p_x, float p_y, float p_z){
	m_scale->scale(p_x, p_y, p_z);
}

void CESceneSkybox::setRotation(float p_x, float p_y, float p_z){
	m_rotate->rotate(p_x, p_y, p_z);
}

void CESceneSkybox::setPosition(float p_x, float p_y, float p_z){
	m_translate->translate(p_x, p_y, p_z);
}

void CESceneSkybox::setAbsoluteScale(float p_x, float p_y, float p_z){
	m_scale->loadIdentity();
	m_scale->scale(p_x, p_y, p_z);
}

void CESceneSkybox::setAbsoluteRotation(float p_x, float p_y, float p_z){
	m_rotate->loadIdentity();
	m_rotate->rotate(p_x, p_y, p_z);
}

void CESceneSkybox::setAbsolutePosition(float p_x, float p_y, float p_z){
	m_translate->loadIdentity();
	m_translate->translate(p_x, p_y, p_z);	
}