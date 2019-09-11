
#include <iostream>
#include <glm.hpp>

#include "../../include/ChaoticEngine/fachada/CESceneMesh.hpp"

CESceneMesh::CESceneMesh(CESceneNode* p_parent, const char* p_path, GLuint p_shaderProgram){
	m_rotate    = new CETransform();
	m_scale	    = new CETransform();
	m_translate = new CETransform(); 
	m_mesh	 	= new CEMesh(p_shaderProgram);
	
	m_rotate->rotate(0, 0, 0);
	m_scale->scale(1, 1, 1);
	m_translate->translate(0, 0, 0);
	m_mesh->loadResource(p_path);

	m_nodeRotate 	 = new CESceneNode(p_parent);
	CESceneNode* t_nodeScale 	 = new CESceneNode(m_nodeRotate);
	CESceneNode* t_nodeTranslate = new CESceneNode(t_nodeScale);
	m_nodeMesh 	 = new CESceneNode(t_nodeTranslate);
	
	m_nodeRotate->setEntity(m_rotate);
	t_nodeScale->setEntity(m_scale);
	t_nodeTranslate->setEntity(m_translate);
	m_nodeMesh->setEntity(m_mesh);
}

CESceneMesh::~CESceneMesh(){
	delete m_mesh;
	m_mesh = nullptr;
	
	delete m_translate;
	m_translate = nullptr;
	
	delete m_rotate;
	m_rotate = nullptr;
	
	delete m_scale;
	m_scale = nullptr;
}

void CESceneMesh::setScale(float p_x, float p_y, float p_z){
	m_scale->scale(p_x, p_y, p_z);
}

void CESceneMesh::setRotation(float p_x, float p_y, float p_z){
	m_rotate->rotate(p_x, p_y, p_z);
}

void CESceneMesh::setPosition(float p_x, float p_y, float p_z){
	m_translate->translate(p_x, p_y, p_z);
}

void CESceneMesh::setAbsoluteScale(float p_x, float p_y, float p_z){
	m_scale->loadIdentity();
	m_scale->scale(p_x, p_y, p_z);
}

void CESceneMesh::setAbsoluteRotation(float p_x, float p_y, float p_z){
	m_rotate->loadIdentity();
	m_rotate->rotate(p_x, p_y, p_z);
}

void CESceneMesh::setAbsolutePosition(float p_x, float p_y, float p_z){
	m_translate->loadIdentity();
	m_translate->translate(p_x, p_y, p_z);	
}
