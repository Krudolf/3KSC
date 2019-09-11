 
#ifndef CEENTITY
#define CEENTITY

#include <stack>
#include <glm.hpp>

/*Virtual class*/
class CEEntity{
public:
	virtual ~CEEntity() = default;
	virtual void beginDraw() = 0;
	virtual void endDraw() = 0;

	glm::mat4	getModelMatrix()	{ return m_modelMatrix;	};

protected:
	static std::stack<glm::mat4> m_matrixStack;

	static glm::mat4 m_modelMatrix;
	static glm::mat4 m_viewMatrix;
	static glm::mat4 m_projectionMatrix;
	static glm::mat4 m_normalMatrix;

private:

};

#endif