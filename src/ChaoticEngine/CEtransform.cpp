
#include <gtc/matrix_transform.hpp> //For glm transformations
#include <stack>
#include <iostream>
#include <iomanip>

#include "../include/ChaoticEngine/CEtransform.hpp"

std::stack<glm::mat4> CEEntity::m_matrixStack;

glm::mat4 CEEntity::m_modelMatrix;
glm::mat4 CEEntity::m_viewMatrix;
glm::mat4 CEEntity::m_projectionMatrix;

//Constructor
CETransform::CETransform() : CEEntity(){
    loadIdentity();
}

//Destructor
CETransform::~CETransform(){}

//Loads identity matrix
void CETransform::loadIdentity(){
    m_matrix = glm::mat4();
}

//Loads custom matrix
void CETransform::loadMatrix(glm::mat4 p_matrix){
    m_matrix = p_matrix;
}

//Prints the matrix in the console
void CETransform::showMatrix(glm::mat4 p_matrix){   
    std::cout << std::fixed;
    std::cout << std::setprecision(6);

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            std::cout << p_matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

//Trasposes the transform matrix
void CETransform::transpose(){
    m_matrix = glm::transpose(m_matrix);
}

//Trasposes the given matrix
glm::mat4 CETransform::transpose(glm::mat4 p_matrix){
    return glm::transpose(p_matrix);
}

//Applies a traslation to the matrix
//Receives the offset for each axis
void CETransform::translate(float p_tx, float p_ty, float p_tz){
    m_matrix = glm::translate(m_matrix, glm::vec3(-p_tx, p_ty, p_tz));
}

//Applies a rotation to the matrix
//Receives the rotation for each axis
void CETransform::rotate(float p_rx, float p_ry, float p_rz){
    m_matrix = glm::rotate(m_matrix, glm::radians(p_rx), glm::vec3(1, 0, 0));
    m_matrix = glm::rotate(m_matrix, glm::radians(p_ry), glm::vec3(0, 1, 0));
    m_matrix = glm::rotate(m_matrix, glm::radians(p_rz), glm::vec3(0, 0, 1));
}

//Applies an scale to the matrix
//Receives the ratio for each axis
void CETransform::scale(float p_sx, float p_sy, float p_sz){
    m_matrix = glm::scale(m_matrix, glm::vec3(p_sx, p_sy, p_sz));
}

glm::mat4 CETransform::getMatrix(){
    return m_matrix;
}

void CETransform::beginDraw(){
    m_matrixStack.push(m_modelMatrix);
    m_modelMatrix = m_matrix * m_modelMatrix;
}

void CETransform::endDraw(){
    m_modelMatrix = m_matrixStack.top();
    m_matrixStack.pop();
}