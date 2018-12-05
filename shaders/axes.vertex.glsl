#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4	mvp_matrix;

out vec3        vS_color;

void		main()
{
    vS_color = color;
	gl_Position = mvp_matrix * vec4(position, 1.0);
}
