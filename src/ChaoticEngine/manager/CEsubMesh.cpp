
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "../../include/ChaoticEngine/manager/CEsubMesh.hpp"

//Constructor
CEsubMesh::CEsubMesh(std::vector<Vertex> p_vertices, std::vector<GLuint> p_indices, std::vector<CEResourceTexture*> p_textures){
	m_vertices = p_vertices;
	m_indices = p_indices;
	m_textures = p_textures;

	prepareBuffers();
}

//Destructor
CEsubMesh::~CEsubMesh(){}

void CEsubMesh::subDraw(GLuint p_shaderProgram){
    // Bind appropriate textures
	GLuint diffuseNr  = 1;
	GLuint specularNr = 1;

	for(unsigned int i = 0; i < m_textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = m_textures[i]->getTextureType();
        if(name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(p_shaderProgram, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, m_textures[i]->getTextureId());
    }

	// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
	glUniform1f(glGetUniformLocation(p_shaderProgram, "Material.Shininess"), 30.0f);
	
	glUniform1f(glGetUniformLocation(p_shaderProgram, "numShades"), 7.0f);

	// Draw mesh
	glBindVertexArray(m_VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	for(unsigned int i = 0; i < m_textures.size(); i++){
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void CEsubMesh::prepareBuffers(){
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
