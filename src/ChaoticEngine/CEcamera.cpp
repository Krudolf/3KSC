
#include <gtc/matrix_transform.hpp> //For glm transformations

#include "../include/ChaoticEngine/CEcamera.hpp"

/*
Por defecto crea una camara con vista en perspectiva, los parametros se pueden modificar llamando al setter adecuado
*/
CECamera::CECamera() : CEEntity(){
	setPerspective(360.0f, 1.0f, 0.1f, 1000.0f);
	m_tarjet = glm::vec3(0,0,0);
}

CECamera::~CECamera(){}

/*
Cambia la vista de la camara a perspectiva
*/
void CECamera::setPerspective(float p_angle, float p_aspect, float p_near, float p_far){
	m_isPerspective = true;
	m_angle	 		= p_angle;
	m_aspect 		= p_aspect;
	m_near	 		= p_near;
	m_far 	 		= p_far;
	m_projection 	= glm::perspective(m_angle, m_aspect, m_near, m_far);
}
/*
Cambia la vista de la camara a paralela
*/
void CECamera::setParallel(float p_left, float p_right, float p_bottom, float p_top, float p_near, float p_far){
	m_isPerspective = false;
	m_left 	 		= p_left;
	m_right  		= p_right;
	m_bottom 		= p_bottom;
	m_top 	 		= p_top;
	m_near	 		= p_near;
	m_far 	 		= p_far;
	m_projection 	= glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
}

/*
Indica a donde apunta la camara cuando esta en perspectiva
*/
void CECamera::setTarjet(float p_x, float p_y, float p_z){
	m_tarjet = glm::vec3(p_x, p_y, p_z);
}

void CECamera::setTarjet(glm::vec3 p_tarjet){
	m_tarjet = p_tarjet;
}

/*
Devuelve la matriz de projeccion de la camara
*/
glm::mat4 CECamera::getMatrix(){
	return m_projection;
}
/*
Le pasa al motor la matriz de projeccion de la camara activa
*/
void CECamera::setProjectionMatrix(){
	m_projectionMatrix = m_projection;
}

/*
Le pasa al motor la matriz de vista de la camara activa
*/
void CECamera::setViewMatrix(glm::mat4 p_viewMatrix){
	if(m_isPerspective){
		glm::mat4 t_tempMatrix = glm::inverse(p_viewMatrix);
		glm::vec3 t_pos = (glm::vec3)t_tempMatrix[3];
		//			   glm::lookAt(eye	, tarjet  , up				);
		m_viewMatrix = glm::lookAt(t_pos, m_tarjet, glm::vec3(0,2,0));
	}
	else
		m_viewMatrix = p_viewMatrix;
}

void CECamera::beginDraw(){
	m_MVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
}

void CECamera::endDraw(){}
