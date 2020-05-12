#version 330 core
out vec4 FragColor;


uniform float u_code;

void main()
{
    FragColor = vec4(u_code / 255.0, 0.0, 0.0, 0.0);
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
