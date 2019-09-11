
#include <iostream>

#include "../../include/ChaoticEngine/manager/CEshader.hpp"
#include "../../include/ChaoticEngine/fachada/CEShader.hpp"

/*
Fachada para crear/cargar Shaders
*/

CEShaderProgram::CEShaderProgram(const char* p_pathVertex, const char* p_pathFragment){
	m_shader = new CEShader();

	loadShader(p_pathVertex, p_pathFragment);
	useShaderProgram(m_shaderProgram[0]);
}

CEShaderProgram::~CEShaderProgram(){}

void CEShaderProgram::loadShader(const char* p_pathVertex, const char* p_pathFragment){
	m_shaderProgram.push_back(m_shader->LoadShader(p_pathVertex, p_pathFragment));
}

GLuint CEShaderProgram::getShaderProgram(int p_id){
	return m_shaderProgram[p_id];
}

void CEShaderProgram::useShaderProgram(GLuint p_shaderProgram){
	m_shader->useShader(p_shaderProgram);
}