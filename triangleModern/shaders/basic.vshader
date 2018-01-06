#version 330 core

out vec4 fragment_color;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 vertex_color;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main()
{
    mat4 mvp = u_projection_matrix * u_view_matrix * u_model_matrix;
    gl_Position = mvp * position;
    fragment_color = vec4(vertex_color.rgb, 1.f);
}
