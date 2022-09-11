#shader vertex
#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoord;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 Colour1;
layout (location = 1) out int Colour2;

struct Material {
    vec4 albedoColour;
    vec4 emissiveColour;

    float metallic;
    float roughness;

    sampler2D albedoMap;
    sampler2D aoRoughnessMetallicMap;
    sampler2D normalMap;
    sampler2D emissiveMap;  
}; 

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool enabled;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int numPointLights;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform Material material;
uniform DirLight dirLight;
uniform vec3 viewPos;

uniform int entityID;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(material.normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   
// ----------------------------------------------------------------------------


vec3 CalcDirLight(DirLight light, vec3 N, vec3 V, vec3 F0, float roughness, float metallic, vec3 albedo)
{
    vec3 L = normalize(light.direction);
    vec3 H = normalize(V + L);
    vec3 radiance = light.diffuse;

    //cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);  

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;	    
             
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);    
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;
    return Lo; 
}


vec3 CalcPointLight(PointLight light,vec3 N, vec3 V, vec3 F0, float roughness, float metallic, vec3 albedo)
{
    vec3 L = normalize(light.position - FragPos);
    vec3 H = normalize(V + L);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (distance * distance);   
    vec3 radiance = light.diffuse  * attenuation;

    //cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);  

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;	    
             
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);    
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;
    return Lo; 
} 

void main()
{
    vec3 albedo     = pow(texture(material.albedoMap, TexCoords).rgb, vec3(2.2));
    float ao    = texture(material.aoRoughnessMetallicMap, TexCoords).r;
    float roughness = texture(material.aoRoughnessMetallicMap, TexCoords).g;
    float metallic  = texture(material.aoRoughnessMetallicMap, TexCoords).b;

    // input lighting data
    vec3 N = getNormalFromMap();
    vec3 V = normalize(viewPos - FragPos);
    vec3 R = reflect(-V, N); 

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0,0,0);

    if(dirLight.enabled)
    {
        // phase 1: Directional lighting
        Lo += CalcDirLight(dirLight, N, V, F0, roughness, metallic, albedo);
    }

    //Point lights
    for(int i = 0; i < numPointLights; i++)
        Lo += CalcPointLight(pointLights[i], N, V, F0, roughness, metallic, albedo);    

    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
 
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse * specular);

    vec3 color = ambient + Lo;
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    Colour1 = vec4(color, 1.0);

    int id =-1;
    if (Colour1.a >= 0.5)
        id = entityID;
    Colour2 = id;
} 