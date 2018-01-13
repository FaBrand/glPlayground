#version 330 core

out vec4 color;
in float angle;

void main()
{
    color = vec4(0.5 + 0.5*angle);
}
