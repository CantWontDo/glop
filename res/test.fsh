#version 460
out vec4 frag_color;
in vec3 _col;
in vec3 _pos;

void main()
{
    frag_color = vec4(_col * _col * _col * _col * _col * _col + _col * 5, 1.0)
        / 1.2 * abs(_pos.x) / abs(_pos.y);
}