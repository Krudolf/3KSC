 
#ifndef CE_SCENE_BILLBOARD
#define CE_SCENE_BILLBOARD

#include <glew.h>
#include <glfw3.h>

#include "../CEsprite.hpp"
#include "../CEtransform.hpp"
#include "../CEscenenode.hpp"

class CESceneBillboard{
public:
	CESceneBillboard(CESceneNode* p_parent, const char* p_path, float p_width, float p_height, GLuint p_shaderProgram);
	~CESceneBillboard();

	void 	setScale(float p_x, float p_y, float p_z);
	void 	setRotation(float p_x, float p_y, float p_z);
	void 	setPosition(float p_x, float p_y, float p_z);

	void 	setAbsoluteScale(float p_x, float p_y, float p_z);
	void 	setAbsoluteRotation(float p_x, float p_y, float p_z);
	void 	setAbsolutePosition(float p_x, float p_y, float p_z);

private:
	CESprite*		m_sprite;
	CETransform*	m_rotate;
	CETransform*	m_scale;
	CETransform*	m_translate;

};

#endif
