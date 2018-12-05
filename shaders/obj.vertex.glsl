#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 normal;

uniform mat4	mvp_matrix, model_matrix;

out vec3	vS_position;
out vec2	vS_UV;
out vec3	vS_normal;

void		main()
{
	vS_UV = UV;

    // Get fragment position in world coordinates:
	vS_position = vec3(model_matrix * vec4(position, 1.0));

	// Calculate normal in world coordinates if != 1, 1, 1 scaling and rotation
   	vS_normal = normalize(transpose(inverse(mat3(model_matrix))) * normal);

	gl_Position = mvp_matrix * vec4(position, 1.0);
}
