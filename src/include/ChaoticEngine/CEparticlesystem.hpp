#ifndef CE_PARTICLE_SYSTEM
#define CE_PARTICLE_SYSTEM

#include <glew.h>
#include <vector>

#include "CEentity.hpp"
#include "manager/CEresourceTexture.hpp"

struct Particle {
    glm::vec2   Position, Velocity;
    glm::vec4   Color;
    GLfloat     Life;
    Particle() 
        : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


class CEParticleSystem : public CEEntity{
public:
    CEParticleSystem(const char* p_path, int p_amount, float p_x, float p_y, GLfloat p_velocity,
        GLfloat p_life, int p_minAngle, int p_maxAngle, bool p_explode, float p_systemLife, GLuint p_shaderProgram);
    void update(GLfloat dt = 0.025f);
    void beginDraw();
    void endDraw();
    void loadResource(const char* p_urlSource);

    void setAmount(int p_amount);
    int  getAmount()  { return m_amount; }  

    void setNewParticles(int p_newParticles)    { m_newParticles = p_newParticles; }
    int  getNewParticles()  { return m_newParticles; }

    void setParticleLife(int p_particleLife)    { m_particleLife = p_particleLife; }
    int  getParticleLife()  { return m_particleLife; }
    void place(Particle &particle);
    glm::vec3 getPosition()  { return glm::vec3(glm::inverse(m_MVP)[3]); }

    int id;

private:
    std::vector<Particle>   m_particles;
    GLuint                  m_amount;
    GLuint                  m_shaderProgram;
    CEResourceTexture*      m_texture;
    GLuint                  m_VAO;
    GLuint                  m_VBO;
    GLuint                  m_EBO;

    int                     m_newParticles;
    float                   m_particleLife;

    glm::mat4   m_MVP;
    glm::vec3   m_position;

    GLfloat                 m_velocity;
    GLfloat                 m_velocityVariation;
    GLfloat                 m_life;
    GLfloat                 m_lifeVariation;
    int                     m_minAngle;
    int                     m_maxAngle;
    bool                    m_explode;
      

    void 	init();
    int 	firstUnusedParticle();
    void 	respawnParticle(Particle &particle);

    GLfloat                 m_systemLife;
};

#endif