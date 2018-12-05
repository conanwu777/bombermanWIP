#version 410

layout(location = 0) in vec4 position;

uniform mat4	mvp_matrix;

out vec2 UV;

void main()
{
    gl_Position = mvp_matrix * vec4(position.xy, 0.0f, 1.0f);
    UV = position.zw;
}