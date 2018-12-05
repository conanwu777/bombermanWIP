#version 410

uniform samplerCube		tex;

in vec3					skybox_tex_coord;

out vec4				out_fragment;

void main()
{
	vec4 color_skybox;

	color_skybox = texture(tex, skybox_tex_coord);
	out_fragment = vec4(color_skybox.rgb, 1.0f);
//	out_fragment = vec4(1.0);
}
