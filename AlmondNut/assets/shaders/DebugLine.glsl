#pragma once
#shader vertex
#version 330 core
layout (location=0) in vec2 v_position;
layout (location=1) in vec4 v_color;

uniform mat4 uProjection;
uniform mat4 uView;

out vec4 f_color;


void main()
{
    f_color = v_color;
    gl_Position = uProjection * uView * vec4(v_position, 0.0f, 1.0);
}

#shader fragment
#version 330 core

in vec4 f_color;
out vec4 color;

void main()
{
    color = f_color;
}