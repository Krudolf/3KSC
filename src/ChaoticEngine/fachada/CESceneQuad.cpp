
#include <iostream>
#include <glm.hpp>

#include "../../include/ChaoticEngine/fachada/CESceneQuad.hpp"

/*
Fachada para crear Quads y aplicarles las transformaciones correspondientes
*/

CESceneQuad::CESceneQuad(CESceneNode* p_parent, float p_vertex[4][2], GLuint p_shaderProgram){
	m_translate = new CETransform(); 
	m_quad	 	= new CEQuad(p_vertex, p_shaderProgram);
	
	m_translate->translate(0, 0, 0);

	m_nodeTranslate 			 = new CESceneNode(p_parent);
	CESceneNode* t_nodeQuad 	 = new CESceneNode(m_nodeTranslate);
	m_nodeTranslate->setEntity(m_translate);
	t_nodeQuad->setEntity(m_quad);
}

CESceneQuad::~CESceneQuad(){
	delete m_quad;
	m_quad = nullptr;
	
	delete m_translate;
	m_translate = nullptr;
}

void CESceneQuad::setPosition(float p_x, float p_y, float p_z){
	m_translate->translate(p_x, p_y, p_z);
}

void CESceneQuad::setAbsolutePosition(float p_x, float p_y, float p_z){
	m_translate->loadIdentity();
	m_translate->translate(p_x, p_y, p_z);	
}
