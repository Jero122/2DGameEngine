#shader vertex
#version 330 core
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

const vec3 vertices[8] = vec3[8](
	vec3(-1.0,-1.0, 1.0),
	vec3( 1.0,-1.0, 1.0),
	vec3( 1.0, 1.0, 1.0),
	vec3(-1.0, 1.0, 1.0),

	vec3(-1.0,-1.0,-1.0),
	vec3( 1.0,-1.0,-1.0),
	vec3( 1.0, 1.0,-1.0),
	vec3(-1.0, 1.0,-1.0)
);

const int indices[36] = int[36](
	// front
	0, 1, 2, 2, 3, 0,
	// right
	1, 5, 6, 6, 2, 1,
	// back
	7, 6, 5, 5, 4, 7,
	// left
	4, 0, 3, 3, 7, 4,
	// bottom
	4, 5, 1, 1, 0, 4,
	// top
	3, 2, 6, 6, 7, 3
);

void main()
{

	int idx = indices[gl_VertexID];
	TexCoords = vertices[idx];
	vec4 pos =  projection * view * vec4(vertices[idx], 1.0);
	gl_Position = pos.xyww;
}  


#shader fragment
#version 330 core

layout (location = 0) out vec4 Colour1;
layout (location = 1) out int Colour2;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    Colour1 = texture(skybox, TexCoords);
    Colour2 = -1;
}