
#version 330

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

uniform sampler2D colorTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform TMaterial 	Material;
uniform DirectionalLight 	DirLight;
uniform PointLight 	        Light[NR_POINT_LIGHTS];

uniform int texture_diffuse;
uniform int texture_specular;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;

layout (location = 0) out vec4 out_color;

const int	levels = 6;
const float scaleFactor = 1.0 / levels;

void main(){
	vec3 color = texture(colorTexture, TexCoords).rgb;
	
	vec3 light 	= normalize(lightPos - Position);
	vec3 view 	= normalize(viewPos - Position);
	vec3 halfV 	= normalize(light + view);

	float amb 		= 0.5;
	float diff 		= clamp(dot(light,Normal), 0.0f, 1.0f);
	float spec 		= 0;
	if(dot(light,Normal) > 0.0)
        spec = pow(clamp(dot(halfV, Normal), 0.0f, 1.0f), Material.Shininess);

    float intensity = amb + diff + spec;
	
	float shadeIntensity = ceil(intensity * levels) / levels;

	out_color = vec4(color * shadeIntensity, 1.0f);
}