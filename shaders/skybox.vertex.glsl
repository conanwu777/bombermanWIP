#version 410

layout(location = 0) in vec3 position;

uniform mat4	mvp_matrix;

out vec3	skybox_tex_coord;

void		main()
{
	skybox_tex_coord = position;
    gl_Position = mvp_matrix * vec4(position, 1.0);
}
