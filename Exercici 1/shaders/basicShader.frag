#version 330 core

out vec4 FragColor;

uniform vec4 TancColor;

void main() {
//     FragColor = vec4(0, 0, 0, 1);
    FragColor = TancColor;
}
