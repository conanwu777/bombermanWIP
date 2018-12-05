#version 410

in  vec3            vS_color;

out vec4			out_fragment;

void main()
{
    out_fragment = vec4(vS_color.r, vS_color.g, vS_color.b, 1.0);
//    out_fragment = vec4(0.5);
}