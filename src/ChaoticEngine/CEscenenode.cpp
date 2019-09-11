
#include "../include/ChaoticEngine/CEscenenode.hpp" 
#include "../include/ChaoticEngine/CEtransform.hpp"
#include "../include/ChaoticEngine/CEmesh.hpp"
#include "../include/ChaoticEngine/CElight.hpp"

/*
Constructor para crear el nodo raiz de la escena
*/
CESceneNode::CESceneNode(){
	m_father = NULL;
	m_entity = NULL;
}

/*
Constructor para crear un nodo que va a ser hijo de otro nodo
*/
CESceneNode::CESceneNode(CESceneNode* p_father){
	m_father = p_father;
	m_father->addChild(this);
}

CESceneNode::~CESceneNode(){}

void CESceneNode::addChild(CESceneNode* p_child){
	m_childs.push_back(p_child);
	p_child->setFather(this);
}

/*
Elimina el nodo que se le pasa por parametro junto con todos sus hijos e hijos de hijos
*/
bool CESceneNode::removeChild(CESceneNode* p_child){
	for(int i = 0; i < m_childs.size(); i++){
		if(m_childs[i] == p_child){
			m_childs[i]->removeAllChilds();
			m_childs[i] = 0;
			delete m_childs[i];
			m_childs.erase(m_childs.begin()+(i));
			return true;
		}
	}
	return false;
}

/*
Elimina todos los hijos sin borrar al padre
*/
void CESceneNode::removeAllChilds(){
	for(int i = 0; i < m_childs.size(); i++){
		m_childs[i]->removeAllChilds();
		m_childs[i] = 0;
		delete m_childs[i];
	}
	m_childs.clear();
}

void CESceneNode::setEntity(CEEntity* p_entity){
	m_entity = p_entity;
}

CEEntity* CESceneNode::getEntity(){
	return m_entity;
}

CESceneNode* CESceneNode::getFather(){
	return m_father;
}

void CESceneNode::setFather(CESceneNode* p_father){
	m_father = p_father;
}

void CESceneNode::draw(){
	m_entity->beginDraw();
	
	for(int i = 0; i < m_childs.size(); i++){
		m_childs[i]->draw();
	}

	m_entity->endDraw();
}

int CESceneNode::getTotalChilds(){
	return m_childs.size();
}