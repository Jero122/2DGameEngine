#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
//layout (location = 3) in int aTexIndex;

out vec4 ourColor;
out vec2 TexCoord;

uniform mat4 model= mat4(1.0);
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * model * view * vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}



#shader fragment
#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D ourTexture;

void main()
{

	//FragColor = texture(texture1, TexCoord);
	vec4 texColor = texture(ourTexture, TexCoord);

	 if(texColor.a < 0.1)
        discard;
    FragColor = texColor;


	//FragColor = ourColor;
}