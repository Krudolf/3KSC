 
#ifndef CE_SCENE_ANIMATION
#define CE_SCENE_ANIMATION

#include <glew.h>
#include <glfw3.h>

#include "../CEanimatedMesh.hpp"
#include "../CEtransform.hpp"
#include "../CEscenenode.hpp"

class CESceneAnimation{
public:
	CESceneAnimation(CESceneNode* p_parent, const char* p_path, GLuint p_shaderProgram, bool p_loop);
	~CESceneAnimation();

	void 	loadAnimation(const char* p_path, bool p_loop);
	void 	changeCurrentAnimation(int p_current);

	void 	setScale(float p_x, float p_y, float p_z);
	void 	setRotation(float p_x, float p_y, float p_z);
	void 	setPosition(float p_x, float p_y, float p_z);

	void 	setAbsoluteScale(float p_x, float p_y, float p_z);
	void 	setAbsoluteRotation(float p_x, float p_y, float p_z);
	void 	setAbsolutePosition(float p_x, float p_y, float p_z);

	glm::vec3	getPosition() { return glm::vec3(glm::inverse(m_animation->getMVP())[3]); }

	CESceneNode* getTopNode()	{ return m_nodeRotate; }
	CESceneNode* getNode()		{ return m_nodeMesh; }

private:
	CEAnimatedMesh*	m_animation;
	CETransform*	m_rotate;
	CETransform*	m_scale;
	CETransform*	m_translate;

	CESceneNode* 	m_nodeRotate;
	CESceneNode*	m_nodeMesh;

};

#endif
