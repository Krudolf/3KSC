 
#ifndef CE_SCENE_PARTICLE_SYSTEM
#define CE_SCENE_PARTICLE_SYSTEM

#include <glew.h>
#include <glfw3.h>

#include "../CEparticlesystem.hpp"
#include "../CEtransform.hpp"
#include "../CEscenenode.hpp"

class CESceneParticleSystem{
public:
	CESceneParticleSystem(CESceneNode* p_parent, const char* p_path, int p_amount, float p_x, float p_y, GLfloat p_velocity, 
    GLfloat p_life, int p_minAngle, int p_maxAngle, bool p_explode, float p_systemLife,GLuint p_shaderProgram); 
	~CESceneParticleSystem();

	void	processInput(GLFWwindow* p_window);

	void	update();

	void 	setScale(float p_x, float p_y, float p_z);
	void 	setRotation(float p_x, float p_y, float p_z);
	void 	setPosition(float p_x, float p_y, float p_z);

	void 	setAbsoluteScale(float p_x, float p_y, float p_z);
	void 	setAbsoluteRotation(float p_x, float p_y, float p_z);
	void 	setAbsolutePosition(float p_x, float p_y, float p_z);

	glm::vec3	getPosition()	{ return m_system->getPosition(); }

	CESceneNode* getTopNode()	{ return m_nodeRotate; }
	CESceneNode* getNode() 		{ return m_nodeEmitter;}

	void 	setAmount(int p_amount) { m_system->setAmount(p_amount); }
    int  	getAmount()  { return m_system->getAmount(); }  

    void 	setNewParticles(int p_newParticles)    { m_system->setNewParticles(p_newParticles); }
    int  	getNewParticles()  { return m_system->getNewParticles(); }

    void 	setParticleLife(int p_particleLife)    {m_system->setParticleLife(p_particleLife); }
    int  	getParticleLife()  { return m_system->getParticleLife(); }

	CEParticleSystem* getSystem();

private:
	CETransform*	m_rotate;
	CETransform*	m_scale;
	CETransform*	m_translate;
	
	CEParticleSystem*		m_system;

	CESceneNode* m_nodeRotate;
	CESceneNode* m_nodeEmitter;

};

#endif
