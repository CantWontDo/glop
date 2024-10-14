#version 460
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;

uniform mat4x4 world;

out vec3 _col;

void main()
{
    gl_Position = vec4(pos, 1.0) * world;
    _col = col;
}