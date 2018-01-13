#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;


out float angle;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main()
{

    mat4 mvp = u_projection_matrix * u_view_matrix * u_model_matrix;
    vec4 position = mvp * vec4(position.xzy, 1.0f);
    gl_Position = position;

    vec3 light_position = vec3(10,10,10);
    angle = dot(normalize(position.xyz),normalize(normals));
}
