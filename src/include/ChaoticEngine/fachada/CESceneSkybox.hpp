 
#ifndef CE_SCENE_SKYBOX
#define CE_SCENE_SKYBOX

#include <glew.h>
#include <glfw3.h>

#include "../CEskybox.hpp"
#include "../CEtransform.hpp"
#include "../CEscenenode.hpp"

class CESceneSkybox{
public:
	CESceneSkybox(CESceneNode* p_parent, const char* p_path[6], float p_scale, GLuint p_shaderProgram);
	~CESceneSkybox();

	void 	setScale(float p_x, float p_y, float p_z);
	void 	setRotation(float p_x, float p_y, float p_z);
	void 	setPosition(float p_x, float p_y, float p_z);

	void 	setAbsoluteScale(float p_x, float p_y, float p_z);
	void 	setAbsoluteRotation(float p_x, float p_y, float p_z);
	void 	setAbsolutePosition(float p_x, float p_y, float p_z);

private:
	CESkybox* 		m_skybox;
	CETransform*	m_rotate;
	CETransform*	m_scale;
	CETransform*	m_translate;

};

#endif
