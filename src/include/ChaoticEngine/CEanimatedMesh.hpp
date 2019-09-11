
#ifndef CEANIMATEDMESH
#define CEANIMATEDMESH

#include <glew.h>

#include "CEentity.hpp"
#include "manager/CEresourceAnimation.hpp"

class CEAnimatedMesh : public CEEntity{
public:
	CEAnimatedMesh(GLuint p_shaderProgram);
	virtual ~CEAnimatedMesh();

	void loadResource(const char* p_urlSource, bool p_loop);
	void setCurrentAnimation(int p_current);

	void beginDraw() override;
	void endDraw()   override;

	glm::mat4 getMVP()	{ return m_MVP; }

private:
	std::vector<CEResourceAnimation*> m_animations;
	CEResourceAnimation* m_currentAnimation;
	GLuint m_shaderProgram;

	glm::mat4	m_MVP;

	double m_lastTime;
	float m_frameTime;
 	int m_currentFrame;
};

#endif