
#version 330 core

//Estructura para guardar la textura. Se guardan las propiedades difusas y especulares de la textura
struct TMaterial{
	sampler2D texture_diffuse;
	sampler2D texture_specular;

	float Shininess;
};

struct DirectionalLight{
    vec3 Direction;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

#define NR_POINT_LIGHTS 5
struct PointLight{
	vec3 Position;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

    float Attenuation;
};

//Estado de OpenGL: textura y luz de los tipos anteriores
uniform vec3 		        viewPos;
uniform TMaterial           Material;
uniform DirectionalLight 	DirLight;
uniform PointLight 	        Light[NR_POINT_LIGHTS];

//Entrada desde el vertex shader
in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;  

out vec4 FragColor;

//Functions
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 position, vec3 viewDir);

void main(){
	vec3 normal	 = normalize(Normal);
	vec3 viewDir = normalize(viewPos - Position);

	vec3 result = calcDirLight(DirLight, normal, viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; i++){
		result += calcPointLight(Light[i], normal, Position, viewDir);
	}

	FragColor = vec4(result, 1.0f);
}
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.Direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.Shininess);
    
    // combine results
    vec3 ambient  = light.Ambient  * vec3(texture(Material.texture_diffuse, TexCoords));
    vec3 diffuse  = light.Diffuse  * diff * vec3(texture(Material.texture_diffuse, TexCoords));
    vec3 specular = light.Specular * spec * vec3(texture(Material.texture_specular, TexCoords));
    
    return (ambient + diffuse + specular);
}  

// calculates the color when using a point light.
vec3 calcPointLight(PointLight light, vec3 normal, vec3 position, vec3 viewDir){
    vec3 lightDir = normalize(light.Position - position);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    vec3  reflectDir  = reflect(-lightDir, normal);
    float spec        = pow(max(dot(viewDir, reflectDir), 0.0), Material.Shininess);
    
    // combine results
    vec3 ambient  = light.Ambient  * vec3(texture(Material.texture_diffuse, TexCoords));
    vec3 diffuse  = light.Diffuse  * diff * vec3(texture(Material.texture_diffuse, TexCoords));
    vec3 specular = light.Specular * spec * vec3(texture(Material.texture_specular, TexCoords));
    
    ambient  *= light.Attenuation;
    diffuse  *= light.Attenuation;
    specular *= light.Attenuation;
    
    return (ambient + diffuse + specular);
}
