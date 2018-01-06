#version 330 core

layout(location = 0) in vec4 position;
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main()
{
    mat4 mvp = u_projection_matrix * u_view_matrix * u_model_matrix;
    gl_Position = mvp * position;
}
