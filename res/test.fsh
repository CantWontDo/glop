#version 460
out vec4 frag_color;
in vec3 _col;
in vec3 _pos;

uniform float time;

void main()
{
    // goodbye cool effect
//    frag_color = vec4(_col * _col * _col * _col * _col * _col + _col * 3 * abs(_pos.x) * abs(_pos.x), 1.0)
//        / 1.2 * abs(_pos.x) / abs(_pos.y);
    frag_color = vec4(0.5 - _pos.y, abs(_pos.x), 0.5 + _pos.y, 1.0);
}
