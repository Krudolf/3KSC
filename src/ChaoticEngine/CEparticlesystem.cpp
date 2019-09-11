
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include "../include/ChaoticEngine/CEparticlesystem.hpp"
#include "../include/ChaoticEngine/manager/CEresourceManager.hpp"
#include "../include/managers/EngineManager.hpp"
#define PI 3.14159265

CEParticleSystem::CEParticleSystem(const char* p_path, int p_amount, float p_x, float p_y, GLfloat p_velocity,
    GLfloat p_life, int p_minAngle, int p_maxAngle, bool p_explode, float p_systemLife, GLuint p_shaderProgram) : CEEntity(){
    m_shaderProgram     = p_shaderProgram;
    m_amount            = p_amount;
    m_newParticles      = 1; //??
    m_explode           = p_explode;
    m_velocity          = p_velocity;
    m_life              = p_life;
    m_minAngle          = p_minAngle;
    m_maxAngle          = p_maxAngle;
    m_explode           = p_explode;
    m_lifeVariation     = 0.001;
    m_velocityVariation = 0.6;
    m_systemLife        = p_systemLife;

    m_newParticles = 100;
    m_particleLife = 2.0f;

    loadResource(p_path);
    init();
}


void CEParticleSystem::init(){
    GLfloat t_vertices[] = { 
        // Pos      // Tex
        -0.5f, -0.5f, 0.07f, 0.07f, 
        -0.5f,  0.5f, 0.07f, 0.93f,
         0.5f,  0.5f, 0.93f, 0.93f,
         0.5f, -0.5f, 0.93f, 0.07f
    };

    unsigned int m_indices[] = {
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(t_vertices), t_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);

    //Create m_amount default particle instances
    for(GLuint i = 0; i < m_amount; i++)
    {
        m_particles.push_back(Particle());
    }
    m_lifeVariation = 0.5; //after first particles
}


//Render all particles
void CEParticleSystem::beginDraw(){
    
    glUseProgram(m_shaderProgram);
    glm::mat4 t_projection = glm::ortho(512.0f, -512.0f, -384.0f, 384.0f, -15.0f, 100.0f);
    //glm::mat4 t_projection = glm::ortho(20.0f, -20.0f, -20.0f, 20.0f, -15.0f, 100.0f);
    m_MVP = t_projection * m_viewMatrix * m_modelMatrix;
    m_position = getPosition();
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_MVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
    m_position = getPosition();

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    for(Particle particle : m_particles){
        if(particle.Life > 0.0f){
            glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(t_projection));
            glUniform1f(glGetUniformLocation(m_shaderProgram, "scale2"), 20);
            glUniform2f(glGetUniformLocation(m_shaderProgram, "offset"), particle.Position.x, particle.Position.y);
            glUniform4f(glGetUniformLocation(m_shaderProgram, "color"), particle.Color.x, particle.Color.y, particle.Color.z, particle.Color.w);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texture->getTextureId());
            glDepthMask (GL_FALSE); //arreglo para las transparencias
            glBindVertexArray(m_VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
    glDepthMask (GL_TRUE);
}

void CEParticleSystem::endDraw(){}

void CEParticleSystem::update(GLfloat dt){
    //Add new particles 
    for(GLuint i = 0; i < m_newParticles; i++){
        int unusedParticle = firstUnusedParticle();
        if(unusedParticle == -1) 
        {
            auto  m_engineManager = &EngineManager::instance();
            for(GLuint i = 0; i < m_amount; i++)
            {
                m_particles[i].Life = 0;
            }
            m_engineManager->deleteParticleSystem(this);
            break;
        }
        respawnParticle(m_particles[unusedParticle]);
    }

    m_systemLife -= dt;
    //Update all particles
    for(GLuint i = 0; i < m_amount; i++){
        Particle &p = m_particles[i];
        p.Life -= dt;
        if(p.Life > 0.0f){  //particle is alive
            p.Position -= p.Velocity * dt; 
        }
    }
}

//Stores the index of the last particle used (for quick access to next dead particle)
int lastUsedParticle = 0;
int CEParticleSystem::firstUnusedParticle(){
    if(m_explode && m_systemLife < 0)
    {   
        return -1;
    }
    //First search from last used particle, this will usually return almost instantly
    for(int i = lastUsedParticle; i < m_amount; i++){
        if(m_particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    //Otherwise, do a linear search
    for(int i = 0; i < lastUsedParticle; i++){
        if(m_particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    //All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void CEParticleSystem::respawnParticle(Particle &particle){
    int  t_randAngle    = rand() % (m_maxAngle - m_minAngle + 1) + m_minAngle;
    float t_angle       = (t_randAngle + 90) * (PI/180);
    float t_sin         = std::sin(t_angle);
    float t_cos         = std::cos(t_angle);
    float t_velocity    = (m_velocity * m_velocityVariation) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(m_velocity - (m_velocity * m_velocityVariation))));
    GLfloat t_life      = (m_life * m_lifeVariation) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(m_life - (m_life * m_lifeVariation))));
    particle.Position   = glm::vec2(m_position.x, m_position.y);
    //particle.Color      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); 
    particle.Life       = t_life; 
    particle.Velocity   = glm::vec2(t_velocity * t_sin, t_velocity* t_cos);
}

void CEParticleSystem::loadResource(const char* p_urlSource){
    CEResourceManager* t_manager = CEResourceManager::instance();
    CEResourceTexture* t_resource = (CEResourceTexture*)&t_manager->getResource(p_urlSource);
    if(t_resource != 0){
        m_texture = t_resource;
        m_texture->glBuffersTexture();
    }
}

void CEParticleSystem::setAmount(int p_amount){
    m_amount = p_amount; 
    m_particles.clear();
    //Create m_amount default particle instances
    for(GLuint i = 0; i < m_amount; i++)
        m_particles.push_back(Particle());    
}


