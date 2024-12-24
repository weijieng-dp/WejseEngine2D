#version 330 core
out vec4 UFragColor;

uniform vec3 UpickColor;

void main() {
    UFragColor = vec4(UpickColor, 1.0);
}
