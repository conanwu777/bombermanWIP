#version 410

uniform sampler2D tex;
uniform vec3 color;

in vec2 UV;

out vec4 out_fragment;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, UV).r);
    out_fragment = vec4(color, 1.0) * sampled;
//    out_fragment = sampled;
//    out_fragment = vec4(1.0);
}