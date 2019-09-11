
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <glm.hpp>

#include "../../include/ChaoticEngine/fachada/CEScene.hpp"
#include "../../include/ChaoticEngine/fachada/CESceneCamera.hpp"
#include "../../include/ChaoticEngine/fachada/CESceneLight.hpp"
#include "../../include/ChaoticEngine/fachada/CESceneMesh.hpp"
#include "../../include/ChaoticEngine/fachada/CESceneAnimation.hpp"
#include "../../include/ChaoticEngine/fachada/CESceneQuad.hpp"
#include "../../include/ChaoticEngine/fachada/CESceneSprite.hpp"
#include "../../include/ChaoticEngine/fachada/CESceneParticleSystem.hpp"
#include "../../include/ChaoticEngine/fachada/CESceneSkybox.hpp"
#include "../../include/ChaoticEngine/fachada/CESceneBillboard.hpp"
#include "../../include/ChaoticEngine/fachada/CEShader.hpp"
#include "../../include/ChaoticEngine/CEtransform.hpp"

/*
Esta es la clase principal de la fachada del Chaotic Engine.
Para crear el arbol de la escena necesitas crear un objeto de tipo CEScene y luego llamar a los respectivos metodos de create para añadir nodos a la escena.
*/

/*
Crea un nodo raiz y precarga todos los respectivos shaders
*/
CEScene::CEScene(){
	m_root = new CESceneNode();
	
	CETransform* rootEntity = new CETransform();
	m_root->setEntity(rootEntity);

	m_resourceManager = new CEResourceManager();

	//[ 0 ] - Main shader
	m_shaderProgram = new CEShaderProgram("src/ChaoticEngine/shader/CEvertex.vert", "src/ChaoticEngine/shader/CEfragment.frag");
	//[ 1 ] - Shader for the debug quads
	m_shaderProgram->loadShader("src/ChaoticEngine/shader/CEvertQuad.vert", "src/ChaoticEngine/shader/CEfragQuad.frag");
	//[ 2 ] - Shader for the sprites
	m_shaderProgram->loadShader("src/ChaoticEngine/shader/CEvertSprite.vert", "src/ChaoticEngine/shader/CEfragSprite.frag");
	//[ 3 ] - Shader for the particle system
	m_shaderProgram->loadShader("src/ChaoticEngine/shader/CEvertParticle.vert", "src/ChaoticEngine/shader/CEfragParticle.frag");
	//[ 4 ] - Shader for the skybox
	m_shaderProgram->loadShader("src/ChaoticEngine/shader/CEvertSkybox.vert", "src/ChaoticEngine/shader/CEfragSkybox.frag");	
	//[ 5 ] - Shader for the cartoon
	m_shaderProgram->loadShader("src/ChaoticEngine/shader/CEvertex.vert", "src/ChaoticEngine/shader/CEfragCartoon.frag");
}

CEScene::~CEScene(){
	delete m_root;
	m_root = nullptr;

	delete m_resourceManager;
	m_resourceManager = nullptr;

	delete m_shaderProgram;
	m_shaderProgram = nullptr;
}

/*
Creamos una camara e indicamos si la queremos como camara activa o no
*/
CESceneCamera* CEScene::createCamera(bool p_isActive){
	CESceneCamera* CEcamera = new CESceneCamera(m_root, p_isActive);
	CEcamera->setPosition(0,2,5);

	m_cameras.push_back(CEcamera);

	if(p_isActive){
		m_activeCamera = CEcamera;
		m_activeCamera->activateCamera();
	}

	return CEcamera;
}

/*
Creamos una luz direccional (siempre debe de existir una para que funcionen bien los shaders)
*/
CESceneLight* CEScene::createDirectionalLight(float p_direction[3]){
	glm::vec3 t_direction(p_direction[0], p_direction[1], p_direction[2]);

	m_directionalLight = new CESceneLight(m_root, t_direction, m_shaderProgram->getShaderProgram(0));

	return m_directionalLight;
}

/*
Creamos una luz puntual
*/
CESceneLight* CEScene::createPointLight(float p_lightAtenuation){
	float 		t_attenuation = p_lightAtenuation;

	CESceneLight* CElight = new CESceneLight(m_root, t_attenuation, m_shaderProgram->getShaderProgram(0));
	
	m_lights.push_back(CElight);

	return CElight;	
}

/*
Creamos una malla con la ruta que se indica por paramtro
*/
CESceneMesh* CEScene::createMesh(const char* p_path){
	CESceneMesh* CEmesh = new CESceneMesh(m_root, p_path, m_shaderProgram->getShaderProgram(5));

	return CEmesh;	
}

/*
Creamos una malla animada con sus correspondientes frames con la ruta que se le pasa por parametro
*/
CESceneAnimation* CEScene::createAnimatedMesh(const char* p_path, bool p_loop){
	CESceneAnimation* CEanimMesh = new CESceneAnimation(m_root, p_path, m_shaderProgram->getShaderProgram(5), p_loop);

	return CEanimMesh;	
}

/*
Creamos un quad (solo se usan para el modo debug)
*/
CESceneQuad* CEScene::createQuad(float p_vertex[4][2]){
	CESceneQuad* CEquad = new CESceneQuad(m_root, p_vertex, m_shaderProgram->getShaderProgram(1));

	return CEquad;	
}

/*
Creamos un sprite 2D
*/
CESceneSprite* CEScene::createSprite(const char* p_path, float p_width, float p_height, bool p_originCenter){
	CESceneSprite* CEsprite = new CESceneSprite(m_root, p_path, p_width, p_height, m_shaderProgram->getShaderProgram(2), p_originCenter);

	return CEsprite;
}

/*
Creamos un billboard. Puede tener una posicion fija o puede asignarse a un nodo para que le siga.
*/
CESceneBillboard* CEScene::createBillboard(const char* p_path, float p_width, float p_height, CESceneNode* p_parent){
	CESceneBillboard* CEbillboard;
	if(p_parent == NULL)
		CEbillboard = new CESceneBillboard(m_root, p_path, p_width, p_height, m_shaderProgram->getShaderProgram(2));
	else
		CEbillboard = new CESceneBillboard(p_parent, p_path, p_width, p_height, m_shaderProgram->getShaderProgram(2));

	return CEbillboard;
}

/*
Creamos un emisor de particulas con los parametros que se le indican por paramtro
*/
CESceneParticleSystem* CEScene::createParticleSystem(const char* p_path, int p_amount, float p_x, float p_y, GLfloat p_velocity, GLfloat p_life, int p_minAngle,int p_maxAngle, bool p_explode, float p_systemLife){
	CESceneParticleSystem* CEemitter = new CESceneParticleSystem(m_root, p_path, p_amount, p_x, p_y,p_velocity, p_life, p_minAngle, p_maxAngle, p_explode, p_systemLife,m_shaderProgram->getShaderProgram(3));

	return CEemitter;
}

/*
Creamos un skybox con las rutas que se le pasan por parametro y una escala para dicho skybox
*/
CESceneSkybox* CEScene::createSkybox(const char* p_path[6], float p_scale){
	CESceneSkybox* CEskybox = new CESceneSkybox(m_root, p_path, p_scale, m_shaderProgram->getShaderProgram(4));

	return CEskybox;
}

/*
La camara pasada por parametro se convierte en la camara activa
*/
void CEScene::setActiveCamera(CESceneCamera* p_camera){
	m_activeCamera = p_camera;
	m_activeCamera->activateCamera();
}

/*
En cada llamada a draw de la escena calculamos las luces y se las pasamos a los shaders
*/
void CEScene::calculateLights(){
	GLuint t_shaderProgram = m_shaderProgram->getShaderProgram(0);
	glUseProgram(m_shaderProgram->getShaderProgram(0));

	std::string t_light		= "Light[";
	std::string t_position	= "].Position";
	std::string t_ambient	= "].Ambient";
	std::string t_diffuse	= "].Diffuse";
	std::string t_specular	= "].Specular";
	std::string t_atten		= "].Attenuation";
	std::string t_result;

	glUniform3fv(glGetUniformLocation(t_shaderProgram, "viewPos"), 1, glm::value_ptr(m_activeCamera->getPosition()));
	glUniform3fv(glGetUniformLocation(t_shaderProgram, "lightPos"), 1, glm::value_ptr(m_directionalLight->getPosition()));

	glUniform3fv(glGetUniformLocation(t_shaderProgram, "DirLight.Direction"), 1, glm::value_ptr(m_directionalLight->getDirection()));
	glUniform3fv(glGetUniformLocation(t_shaderProgram, "DirLight.Ambient"  ), 1, glm::value_ptr(m_directionalLight->getAmbient()));
	glUniform3fv(glGetUniformLocation(t_shaderProgram, "DirLight.Diffuse"  ), 1, glm::value_ptr(m_directionalLight->getDiffuse()));
	glUniform3fv(glGetUniformLocation(t_shaderProgram, "DirLight.Specular" ), 1, glm::value_ptr(m_directionalLight->getSpecular()));

	for(int i = 0; i < m_lights.size(); i++){
		if(m_lights[i]->getActive()){
			t_result = t_light + std::to_string(i);
		    glUniform3fv(glGetUniformLocation(t_shaderProgram, (t_result + t_position).c_str()), 1, glm::value_ptr(m_lights[i]->getPosition()));
		    
		    glUniform3fv(glGetUniformLocation(t_shaderProgram, (t_result + t_ambient).c_str() ), 1, glm::value_ptr(m_lights[i]->getAmbient()));
		    glUniform3fv(glGetUniformLocation(t_shaderProgram, (t_result + t_diffuse).c_str() ), 1, glm::value_ptr(m_lights[i]->getDiffuse()));
		    glUniform3fv(glGetUniformLocation(t_shaderProgram, (t_result + t_specular).c_str()), 1, glm::value_ptr(m_lights[i]->getSpecular()));

			glUniform1f(glGetUniformLocation(t_shaderProgram, (t_result + t_atten).c_str()),  m_lights[i]->getAttenuation());
		}
	}
}

/*
Dibujamos el arbol de la escena
*/
void CEScene::draw(){
	calculateLights();
	m_root->draw();
}

/*
Dibujamos el arbol de la escena (usado para dibujar solo los menus de 3KSC)
*/
void CEScene::draw2D(){
	m_root->draw();
}

/*
Limpiamos el arbol y liberamos memoria
*/
void CEScene::release(){
	m_root->removeAllChilds();
	m_resourceManager->deleteResources();

	delete m_resourceManager;
	delete m_root;
}

void CEScene::remove(CESceneNode* p_node){
	m_root->removeChild(p_node);
}