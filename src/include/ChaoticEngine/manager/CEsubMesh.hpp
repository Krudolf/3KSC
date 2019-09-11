#ifndef CESUBMESH
#define CESUBMESH

#include <vector>
#include <glew.h> 
#include <glm.hpp>

#include "CEresourceTexture.hpp"

using namespace std;

struct Vertex{
	glm::vec3 Position;		// Position of vertices from the mesh
	glm::vec3 Normal;		// Normals
	glm::vec2 TexCoords;	// TexCoords
};

class CEsubMesh{
public:
	/*  Mesh Data  */
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<CEResourceTexture*> m_textures;

	CEsubMesh(std::vector<Vertex> p_vertices, std::vector<GLuint> p_indices, std::vector<CEResourceTexture*> p_textures);
	~CEsubMesh();

	void subDraw(GLuint p_shaderProgram);

private:
	/*  Render data  */
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;

	void prepareBuffers();
};

#endif