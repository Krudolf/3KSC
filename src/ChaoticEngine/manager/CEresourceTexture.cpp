
#include <iostream>

#include "../../include/ChaoticEngine/manager/CEresourceTexture.hpp"

//Constructor
CEResourceTexture::CEResourceTexture() : CEResource(){}

//Destructor
CEResourceTexture::~CEResourceTexture(){}

bool CEResourceTexture::loadFile(const char* p_name){

    return(textureFromFile(p_name));
}

void CEResourceTexture::draw(GLuint p_shaderProgram){
}


bool CEResourceTexture::textureFromFile(const char * p_path){

    std::string t_filename = p_path;

    m_textureData = stbi_load(t_filename.c_str(), &m_width, &m_height, &m_nrComponents, 0);
    if(m_textureData){
        glGenTextures(1, &m_textureId);
        return true;
    }else{
        std::cout << "Texture failed to load at path: " << p_path << std::endl;
        stbi_image_free(m_textureData);
        m_textureData = NULL;
    }
    return false;
}

void CEResourceTexture::glBuffersTexture(){
    if(m_textureData){
        GLenum format;
        if(m_nrComponents == 1)
            format = GL_RED;
        else if(m_nrComponents == 3)
            format = GL_RGB;
        else if(m_nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_textureData);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

void CEResourceTexture::freeTextureData(unsigned char* p_data){
    stbi_image_free(p_data);
}