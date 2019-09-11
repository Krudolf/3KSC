
#ifndef CEQUAD
#define CEQUAD

#include <glew.h>

#include "CEentity.hpp"

class CEQuad : public CEEntity{
public:
	CEQuad(float p_vertex[4][2], GLuint p_shaderProgram);
	virtual ~CEQuad();

	void updatePositions(float p_vertex[4][2]);

	void beginDraw() override;
	void endDraw()   override;

private:
	GLuint m_shaderProgram;
    
    float 		 m_vertices[12];
    unsigned int m_indices[6];

    unsigned int m_VBO;
    unsigned int m_VAO;
    unsigned int m_EBO;
};

#endif