#shader vertex
#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aTranslate;
layout (location=2) in vec3 aScale;
layout (location=3) in float aRotate;

layout (location=4) in vec4 aColor;
layout (location=5) in vec2 aTexCoords;
layout (location=6) in float aTexId;

uniform mat4 uProjection = mat4(1.0f);
uniform mat4 uView = mat4(1.0f);

out vec4 fColor;
out vec2 fTexCoords;
out float fTexId;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

mat4 scaleMatrix(vec3 scale)
{
 return mat4(scale.x,0,0,0,
             0,scale.y,0,0,
             0,0,1,      0,
             0,0,0,1
 );
}

mat4 translationMatrix(vec3 translate)
{
 return mat4(1          ,0          ,0          ,0,
             0          ,1          ,0          ,0,
             0          ,0          ,1          ,0,
             translate.x,translate.y,translate.z,1
 );
}

void main()
{
    
    fColor = aColor;
    fTexCoords = aTexCoords;
    fTexId = aTexId;

    gl_Position = uProjection * uView * mat4(1) * translationMatrix(aTranslate) * rotationMatrix(vec3(0,0,1), aRotate) *scaleMatrix(aScale) * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core

in vec4 fColor;
in vec2 fTexCoords;
in float fTexId;

uniform sampler2D uTextures[32];

out vec4 color;

void main()
{

    if (fTexId > 0) {
        int id = int(fTexId);
        color = texture(uTextures[id], fTexCoords);
    } else {
        color = fColor;
    }
}