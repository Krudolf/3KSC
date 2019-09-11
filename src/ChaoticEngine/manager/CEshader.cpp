
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>

#include "../../include/ChaoticEngine/manager/CEshader.hpp"

CEShader::CEShader(){}
CEShader::~CEShader(){}

std::string CEShader::readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint CEShader::LoadShader(const char* vertex_path, const char* fragment_path) {
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shaders
    std::string vertShaderStr = readFile(vertex_path);
    std::string fragShaderStr = readFile(fragment_path);
    if(vertShaderStr == "" || fragShaderStr == "")
        exit(EXIT_FAILURE);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint resultV = 0, resultF = 0, resultL = 0;
    int logLength;

    // Compile vertex shader
    //std::cout << "Compiling vertex shader." << std::endl;
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    // Check vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &resultV);
    if(!resultV){
        std::cout << "Compiling error in " << getFileError(vertex_path) << std::endl;
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
            glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
            std::cout << &vertShaderError[0] << std::endl;
    }

    // Compile fragment shader
    //std::cout << "Compiling fragment shader." << std::endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &resultF);
    if(!resultF){
        std::cout << "Compiling error in " << getFileError(fragment_path) << std::endl;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);
            glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
            std::cout << &fragShaderError[0] << std::endl;
    }

    if(resultV == GL_FALSE || resultF == GL_FALSE)
        exit(EXIT_FAILURE);

    //std::cout << "Linking program" << std::endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &resultL);
    if(!resultL){
        std::cout << "Error while linking " << getFileError(vertex_path) << " and " << getFileError(fragment_path) << std::endl;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> programError( (logLength > 1) ? logLength : 1 );
            glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
            std::cout << &programError[0] << std::endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}

std::string CEShader::getFileError(const char* p_path){
    size_t pos = 0;
    std::string t_path = p_path;    
    std::string t_file;
    std::string t_delimiter = "/";
    while ((pos = t_path.find(t_delimiter)) != std::string::npos) {
        t_file = t_path.substr(0, pos);
        t_path.erase(0, pos + t_delimiter.length());
    }
    
    return t_path;
}

void CEShader::useShader(GLuint p_shader){
    glUseProgram(p_shader);
}
