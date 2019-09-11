
#include <iostream>
#include <gtc/matrix_transform.hpp> //For glm transformations

#include "../../include/ChaoticEngine/fachada/CESceneCamera.hpp"

/*
Fachada para crear Camaras y aplicarle las transformaciones correspondientes
*/

CESceneCamera::CESceneCamera(CESceneNode* p_parent, bool p_isActive){
	m_isActive = p_isActive;

	m_rotate    = new CETransform();
	m_translate = new CETransform(); 
	m_camera 	= new CECamera();
	
	m_rotate->rotate(0, 0, 0);
	m_translate->translate(0, 0, 0.0f);

	CESceneNode* t_nodeRotate 	 = new CESceneNode(p_parent);
	CESceneNode* t_nodeTranslate = new CESceneNode(t_nodeRotate);
	CESceneNode* t_nodeCamera 	 = new CESceneNode(t_nodeTranslate);
	t_nodeRotate->setEntity(m_rotate);
	t_nodeTranslate->setEntity(m_translate);
	t_nodeCamera->setEntity(m_camera);
}

CESceneCamera::~CESceneCamera(){
	delete m_camera;
	m_camera = nullptr;
	
	delete m_translate;
	m_translate = nullptr;
	
	delete m_rotate;
	m_rotate = nullptr;
}

/*
Si la camara se crea o se cambia su estado a activa este metodo hace que se le pasen todos los datos al motor para sus calculos
*/
void CESceneCamera::activateCamera(){
	m_camera->setProjectionMatrix();
	m_camera->setViewMatrix(this->getViewMatrix());
}

void CESceneCamera::setPerspective(float p_angle, float p_aspect, float p_near, float p_far){
	m_camera->setPerspective(p_angle, p_aspect, p_near, p_far);
	m_camera->setProjectionMatrix();
}

void CESceneCamera::setParallel(float p_left, float p_right, float p_bottom, float p_top, float p_near, float p_far){
	m_camera->setParallel(p_left, p_right, p_bottom, p_top, p_near, p_far);
	m_camera->setProjectionMatrix();
}

void CESceneCamera::lookAt(float p_x, float p_y, float p_z){
	m_camera->setTarjet(p_x, p_y, p_z);
	m_camera->setProjectionMatrix();
	m_camera->setViewMatrix(this->getViewMatrix());
}

glm::mat4 CESceneCamera::getViewMatrix(){
	glm::mat4 m_tempMatrix;

	m_tempMatrix = m_rotate->getMatrix() * m_translate->getMatrix();
	m_tempMatrix = glm::inverse(m_tempMatrix);

	return m_tempMatrix;
}

void CESceneCamera::setRotation(float p_x, float p_y, float p_z){
	m_rotate->rotate(p_x, p_y, p_z);
	m_camera->setViewMatrix(this->getViewMatrix());
}

void CESceneCamera::setPosition(float p_x, float p_y, float p_z){
	m_translate->translate(p_x, p_y, p_z);
	m_camera->setViewMatrix(this->getViewMatrix());
}

void CESceneCamera::setAbsoluteRotation(float p_x, float p_y, float p_z){
	m_rotate->loadIdentity();
	m_rotate->rotate(p_x, p_y, p_z);
	m_camera->setViewMatrix(this->getViewMatrix());
}

void CESceneCamera::setAbsolutePosition(float p_x, float p_y, float p_z){
	m_translate->loadIdentity();
	m_translate->translate(p_x, p_y, p_z);
	m_camera->setViewMatrix(this->getViewMatrix());
}