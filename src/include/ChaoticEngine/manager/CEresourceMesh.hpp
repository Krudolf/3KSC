 
#ifndef CERESOURCEMESH
#define CERESOURCEMESH

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "CEresource.hpp"
#include "CEsubMesh.hpp"

class CEResourceMesh: public CEResource{
public:
	CEResourceMesh();
	~CEResourceMesh();

	bool loadFile(const char* p_name);
	void draw(GLuint p_shaderProgram);

private:
	std::vector<CEsubMesh> m_meshes;
    std::string m_directory;
	
	void processNode(aiNode* p_node, const aiScene* p_scene);
	CEsubMesh processMesh(aiMesh* p_mesh, const aiScene* p_scene);
	std::vector<CEResourceTexture*> loadMaterialTextures(aiMaterial* p_mat, aiTextureType p_type, std::string p_typeName);

};

#endif