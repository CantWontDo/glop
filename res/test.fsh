#version 460
out vec4 frag_color;
in vec3 _col;

void main()
{
    frag_color = vec4(_col, 1.0);
}