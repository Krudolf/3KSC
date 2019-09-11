#ifndef CECAMERA
#define CECAMERA

#include "CEentity.hpp"

class CECamera : public CEEntity{
public:
	CECamera();
	virtual ~CECamera();

	void 		setPerspective(float p_angle, float p_aspect, float p_near = 0.1f, float p_far = 100.0f);
	void 		setParallel(float p_left, float p_right, float p_bottom, float p_top, float p_near = 0.1f, float p_far = 100.0f);

	void 		setTarjet(float p_x, float p_y, float p_z);
	void 		setTarjet(glm::vec3 p_tarjet);

	glm::mat4 	getMatrix();

	void 		beginDraw() override;
	void 		endDraw() override;

	void 		setProjectionMatrix();
	void 		setViewMatrix(glm::mat4 p_viewMatrix);

	glm::mat4 getMVP()	{ return m_MVP; }
	

private:
	glm::mat4	m_projection;
	glm::vec3   m_tarjet;
    bool		m_isPerspective;
    
    float 		m_near, m_far;
    float 		m_angle, m_aspect;
    float 		m_left, m_right, m_bottom, m_top;

	glm::mat4	m_MVP;
};

#endif