 
#ifndef CE_SCENE_CAMERA
#define CE_SCENE_CAMERA

#include <glm.hpp>

#include "../CEcamera.hpp"
#include "../CEtransform.hpp"
#include "../CEscenenode.hpp"

class CESceneCamera{
public:
	CESceneCamera(CESceneNode* p_parent, bool p_isActive);
	~CESceneCamera();

	bool 		getActive() 			{ return m_isActive; 	}
	void 		setActive(bool active) 	{ m_isActive = active; 	}

	void 		activateCamera();

	void 		setPerspective(float p_angle, float p_aspect, float p_near, float p_far);
	void 		setParallel(float p_left, float p_right, float p_bottom, float p_top, float p_near, float p_far);
	void		lookAt(float p_x, float p_y, float p_z);

	void		setRotation(float p_x, float p_y, float p_z);
	void		setPosition(float p_x, float p_y, float p_z);

	void		setAbsoluteRotation(float p_x, float p_y, float p_z);
	void		setAbsolutePosition(float p_x, float p_y, float p_z);
	
	glm::vec3	getPosition() { return glm::vec3(glm::inverse(m_camera->getMVP())[3]); }
	
private:
	CECamera*		m_camera;
	CETransform*	m_rotate;
	CETransform*	m_translate;

	bool			m_isActive;

	glm::mat4		getViewMatrix();
};

#endif
