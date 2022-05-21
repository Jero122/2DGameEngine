#shader vertex
#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    FragPos = vec3(uModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(uModel))) * aNormal;  
    TexCoords = aTexCoord;
    
    gl_Position = uProjection * uView * vec4(FragPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform Material uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform vec3 uViewPos;


void main()
{
    // ambient
    vec3 ambient = uDirectionalLight.ambient * texture(uMaterial.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-uDirectionalLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = uDirectionalLight.diffuse * diff * texture(uMaterial.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    vec3 specular = uDirectionalLight.specular * spec * texture(uMaterial.specular, TexCoords).rgb;  


    FragColor = vec4(ambient + diffuse + specular, 1.0);   
} 