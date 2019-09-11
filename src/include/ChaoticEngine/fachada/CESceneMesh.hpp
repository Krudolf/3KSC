 
#ifndef CE_SCENE_MESH
#define CE_SCENE_MESH

#include <glew.h>
#include <glfw3.h>

#include "../CEmesh.hpp"
#include "../CEtransform.hpp"
#include "../CEscenenode.hpp"

class CESceneMesh{
public:
	CESceneMesh(CESceneNode* p_parent, const char* p_path, GLuint p_shaderProgram);
	~CESceneMesh();
	
	void 	setScale(float p_x, float p_y, float p_z);
	void 	setRotation(float p_x, float p_y, float p_z);
	void 	setPosition(float p_x, float p_y, float p_z);

	void 	setAbsoluteScale(float p_x, float p_y, float p_z);
	void 	setAbsoluteRotation(float p_x, float p_y, float p_z);
	void 	setAbsolutePosition(float p_x, float p_y, float p_z);

	glm::vec3	getPosition() { return glm::vec3(glm::inverse(m_mesh->getMVP())[3]); }

	CESceneNode* getTopNode()	{ return m_nodeRotate; }
	CESceneNode* getNode()		{ return m_nodeMesh; }

private:
	CEMesh*			m_mesh;
	CETransform*	m_rotate;
	CETransform*	m_scale;
	CETransform*	m_translate;

	CESceneNode* 	m_nodeRotate;
	CESceneNode*	m_nodeMesh;

};

#endif
