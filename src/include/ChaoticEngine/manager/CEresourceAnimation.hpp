 
#ifndef CERESOURCEANIMATION
#define CERESOURCEANIMATION

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "CEresource.hpp"
#include "CEresourceMesh.hpp"

class CEResourceAnimation: public CEResource{
public:
	CEResourceAnimation();
	~CEResourceAnimation();

	bool loadFile(const char* p_name);
	void draw(GLuint p_shaderProgram, int p_frame);
	int getNumFrames(){return m_meshes.size();};

	bool getHaveLoop(){ return m_loop; }
	void setHaveLoop(bool p_loop){ m_loop = p_loop; }

private:
	std::vector<CEResourceMesh*> m_meshes;
    void meshList();
    bool m_loop;
};

#endif