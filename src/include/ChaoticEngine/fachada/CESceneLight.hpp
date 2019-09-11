 
#ifndef CE_SCENE_LIGHT
#define CE_SCENE_LIGHT

#include <glm.hpp>
#include <glew.h>
#include <glfw3.h>

#include "../CElight.hpp"
#include "../CEtransform.hpp"
#include "../CEscenenode.hpp"

class CESceneLight{
public:
	CESceneLight(CESceneNode* p_parent, float p_attenuation, GLuint p_shaderProgram, bool p_isActive = false);
	CESceneLight(CESceneNode* p_parent, glm::vec3 p_direction, GLuint p_shaderProgram, bool p_isActive = false);
	~CESceneLight();

	bool 		getActive() 			{ return m_isActive; 	}
	void 		setActive(bool active) 	{ m_isActive = active; 	}

	void		setRotation(float p_x, float p_y, float p_z);
	void		setPosition(float p_x, float p_y, float p_z);

	void		setAbsoluteRotation(float p_x, float p_y, float p_z);
	void		setAbsolutePosition(float p_x, float p_y, float p_z);

	glm::vec3	getPosition()	{ return glm::vec3(glm::inverse(m_light->getMVP())[3]); }

	/**********************************************/

	glm::vec3	getAmbient()	{ return m_light->getAmbient();		}
	glm::vec3	getDiffuse()	{ return m_light->getDiffuse();		}
	glm::vec3	getSpecular()	{ return m_light->getSpecular();	}
	glm::vec3 	getDirection()	{ return m_light->getDirection();	}
	float 		getAttenuation(){ return m_light->getAttenuation();	}

	void 		setAmbient(float p_x, float p_y, float p_z)		{ m_light->setAmbient(p_x, p_y, p_z);	}
	void 		setDiffuse(float p_x, float p_y, float p_z)		{ m_light->setDiffuse(p_x, p_y, p_z);	}
	void 		setSpecular(float p_x, float p_y, float p_z)	{ m_light->setSpecular(p_x, p_y, p_z);	}
	void		setDirection(float p_x, float p_y, float p_z)	{ m_light->setDirection(p_x, p_y, p_z);	}
	void		setAttenuation(float p_attenuation)				{ m_light->setAttenuation(p_attenuation);}

private:
	CELight*		m_light;
	CETransform*	m_rotate;
	CETransform*	m_translate;

	bool			m_isActive;
};

#endif
