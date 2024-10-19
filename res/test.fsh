#version 460
out vec4 frag_color;
in vec3 _col;
in vec3 _pos;

uniform float _time;
void main()
{
    frag_color = vec4(_col * _col * _col * _col * _col * _col + _col * 3 * abs(_pos.x) * abs(_pos.x), 1.0)
        / 1.2 * abs(_pos.x) / abs(_pos.y);
}