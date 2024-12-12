#version 460
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;

uniform mat4x4 world;
uniform mat4x4 projection;
uniform mat4x4 view;
uniform float time;

out vec3 _col;
out vec3 _pos;

void main()
{
    gl_Position = vec4(pos, 1.0) * world * view * projection;
    _col = col;
    _pos = gl_Position.xyz;
}