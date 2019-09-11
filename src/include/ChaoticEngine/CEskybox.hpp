
#ifndef CESKYBOX
#define CESKYBOX

#include <glew.h> 

#include "CEentity.hpp"
#include "manager/CEresourceTexture.hpp"

class CESkybox : public CEEntity{
public:
	CESkybox(float p_scale, GLuint p_shaderProgram);
	virtual ~CESkybox();

	void 	beginDraw() override;
	void 	endDraw() 	override;
	void	loadCubemap(const char* p_texturesPath[6]);//0->right, 1->left, 2->top, 3->bottom, 4->front, 5->back

private:
	CEResourceTexture* m_texturesFaces[6]; //0->right, 1->left, 2->top, 3->bottom, 4->front, 5->back
	GLuint 		 m_shaderProgram;
	unsigned int m_skyboxVAO;
	unsigned int m_skyboxVBO;
	unsigned int m_textureID;
};

#endif