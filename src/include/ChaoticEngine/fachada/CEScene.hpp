 
#ifndef CE_SCENE
#define CE_SCENE

#include <vector>

#include "../CEscenenode.hpp"
#include "../manager/CEresourceManager.hpp"

class CESceneCamera;
class CESceneLight;
class CESceneMesh;
class CESceneAnimation;
class CESceneQuad;
class CESceneSprite;
class CESceneParticleSystem;
class CESceneSkybox;
class CESceneBillboard;
class CEShaderProgram;

class CEScene{
public:
	CEScene();
	~CEScene();

	CESceneCamera*			createCamera(bool p_isActive = false);
	CESceneLight*			createDirectionalLight(float p_direction[3]);
	CESceneLight*			createPointLight(float p_lightAtenuation);
	CESceneMesh*			createMesh(const char* p_path);
	CESceneAnimation*		createAnimatedMesh(const char* p_path, bool p_loop);
	CESceneQuad*			createQuad(float p_vertex[4][2]);
	CESceneSprite*			createSprite(const char* p_path, float p_width, float p_height, bool p_originCenter = true);
	CESceneBillboard*		createBillboard(const char* p_path, float p_width, float p_height, CESceneNode* p_parent = NULL);
  	CESceneParticleSystem*  createParticleSystem(const char* p_path, int p_amount, float p_x, float p_y, GLfloat p_velocity, GLfloat p_life, int p_minAngle, int p_maxAngle, bool p_explode,float p_systemLife); 
	CESceneSkybox*			createSkybox(const char* p_path[6], float p_scale);

	void 	setActiveCamera(CESceneCamera* p_camera);
	void 	calculateLights();

	void 	draw();
    void    draw2D();
	void 	release();
    void    clean()     { m_root->removeAllChilds(); }
	void 	remove(CESceneNode* p_node);

private:
	CESceneNode*		m_root;
	CEResourceManager*	m_resourceManager;
	CEShaderProgram* 	m_shaderProgram;

	CESceneCamera*		m_activeCamera;
	CESceneLight* 		m_directionalLight;

	std::vector<CESceneCamera*>	m_cameras;
	std::vector<CESceneLight*>	m_lights;
};

#endif
