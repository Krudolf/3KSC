
#include <glm.hpp>
#include <iostream>

#include "../../include/ChaoticEngine/manager/CEresourceManager.hpp"
#include "../../include/ChaoticEngine/manager/CEresourceMesh.hpp"

//Constructor
CEResourceMesh::CEResourceMesh() : CEResource(){}

//Destructor
CEResourceMesh::~CEResourceMesh(){}

bool CEResourceMesh::loadFile(const char* p_name){
	Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(p_name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if(!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return false;
	}

	std::string path = p_name;
	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
	return true;
}

void CEResourceMesh::processNode(aiNode* p_node, const aiScene* p_scene){
	// Process each mesh located at the current node
	for(unsigned int i = 0; i < p_node->mNumMeshes; i++){
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data
		aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, p_scene));
	}
	// then do the same for each of its children
    for(unsigned int i = 0; i < p_node->mNumChildren; i++){
        processNode(p_node->mChildren[i], p_scene);
    }
}

CEsubMesh CEResourceMesh::processMesh(aiMesh* p_mesh, const aiScene* p_scene){
	// Data to fill
	std::vector<Vertex>  vertices;
	std::vector<GLuint>  indices;
	std::vector<CEResourceTexture*> textures;

	// Walk through each of the mesh's vertices
	for(unsigned int i = 0; i < p_mesh->mNumVertices; i++){
		Vertex vertex;
		glm::vec3 vector; 	//assimp dont uses vec3 class so we transfer the data first.
		// Positions
		if (p_mesh->HasPositions()) {
			vector.x = p_mesh->mVertices[i].x;
			vector.y = p_mesh->mVertices[i].y;
			vector.z = p_mesh->mVertices[i].z;
			vertex.Position = vector;
		}
		// Normals
		if(p_mesh->HasNormals()){
			vector.x = p_mesh->mNormals[i].x;
			vector.y = p_mesh->mNormals[i].y;
			vector.z = p_mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		// Texture Coordinates
		if(p_mesh->mTextureCoords[0]){
			glm::vec2 vec;
			vec.x = p_mesh->mTextureCoords[0][i].x;
			vec.y = p_mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// Now wak through each of the mesh's faces
	for(unsigned int i = 0; i < p_mesh->mNumFaces; i++){
		aiFace face = p_mesh->mFaces[i];

		// Retrieve all indices of the face and store them in the indices vector
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	// Process materials
	if(p_mesh->mMaterialIndex >= 0){
		aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN

		// 1. diffuse maps
        vector<CEResourceTexture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<CEResourceTexture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	// Return a mesh object created from the extracted mesh data
	return CEsubMesh(vertices, indices, textures);
}

void CEResourceMesh::draw(GLuint p_shaderProgram){
	for(GLuint i = 0; i < m_meshes.size(); i++){
			m_meshes[i].subDraw(p_shaderProgram);
	}
}

std::vector<CEResourceTexture*> CEResourceMesh::loadMaterialTextures(aiMaterial * p_mat, aiTextureType p_type, string p_typeName){
    CEResourceManager* t_manager = CEResourceManager::instance();
	vector<CEResourceTexture*> t_textures;
    for(unsigned int i = 0; i < p_mat->GetTextureCount(p_type); i++){
        aiString str;
        p_mat->GetTexture(p_type, i, &str);
        std::string t_path = m_directory + '/' + str.C_Str();
        CEResourceTexture* t_texture = (CEResourceTexture*)&t_manager->getResource(t_path.c_str());
        t_texture->glBuffersTexture();
        t_texture->setTextureType(p_typeName);
        t_texture->setTexturePath(str.C_Str());
        t_textures.push_back(t_texture);
    }
	return t_textures;
}