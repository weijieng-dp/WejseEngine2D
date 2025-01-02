#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform bool hasTexture;
uniform sampler2D ourTexture;

void main()
{
    
    if (hasTexture) {
        FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    } else {
        FragColor = vec4(ourColor, 1.0);
    }
}