#version 410

uniform sampler2D	tex;

               // All in world coordinates:
in vec3				vS_position;
in vec2             vS_UV;
in vec3				vS_normal;

out vec4			out_fragment;

void main()
{
    vec4    surface_color = texture(tex, vS_UV);

    out_fragment = surface_color;
//    out_fragment = vec4(1.0);
}