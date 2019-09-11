#ifndef CELIGHT
#define CELIGHT

#include <glew.h>

#include "CEentity.hpp"

class CELight : public CEEntity{
public:
	CELight(float m_attenuation, GLuint p_shaderProgram);
	CELight(glm::vec3 m_direction, GLuint p_shaderProgram);
	virtual ~CELight();

	void beginDraw() override;
	void endDraw() 	 override;

	void		setComponents();

	glm::mat4 	getMVP()		{ return m_MVP; 		}
	float 		getAttenuation(){ return m_attenuation; }
	glm::vec3 	getAmbient()	{ return m_ambient;		}
	glm::vec3 	getDiffuse()	{ return m_diffuse;		}
	glm::vec3 	getSpecular()	{ return m_specular;	}
	glm::vec3 	getDirection()	{ return m_direction;	}
	
	void 		setAttenuation(float p_attenuation)				{ m_attenuation = p_attenuation; }
	void 		setAmbient(float p_x, float p_y, float p_z)		{ m_ambient		= glm::vec3(p_x, p_y, p_z);	}
	void 		setDiffuse(float p_x, float p_y, float p_z)		{ m_diffuse		= glm::vec3(p_x, p_y, p_z);	}
	void 		setSpecular(float p_x, float p_y, float p_z)	{ m_specular	= glm::vec3(p_x, p_y, p_z);	}
	void		setDirection(float p_x, float p_y, float p_z)	{ m_direction	= glm::vec3(p_x, p_y, p_z);	}

private:
    GLuint 		m_shaderProgram;

	glm::mat4	m_MVP;

	glm::vec3	m_ambient;
	glm::vec3	m_diffuse;
	glm::vec3	m_specular;
	
	glm::vec3	m_direction;

    float 		m_attenuation;
};

#endif